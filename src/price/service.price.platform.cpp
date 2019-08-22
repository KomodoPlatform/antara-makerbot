/******************************************************************************
 * Copyright © 2013-2019 The Komodo Platform Developers.                      *
 *                                                                            *
 * See the AUTHORS, DEVELOPER-AGREEMENT and LICENSE files at                  *
 * the top-level directory of this distribution for the individual copyright  *
 * holder information and the developer policies on copyright and licensing.  *
 *                                                                            *
 * Unless otherwise agreed in a custom licensing agreement, no part of the    *
 * Komodo Platform software, including this file may be copied, modified,     *
 * propagated or distributed except according to the terms contained in the   *
 * LICENSE file                                                               *
 *                                                                            *
 * Removal or modification of this copyright notice is prohibited.            *
 *                                                                            *
 ******************************************************************************/

#include <numeric>
#include <fmt/format.h>
#include "utils/antara.utils.hpp"
#include "utils/antara.algorithm.hpp"
#include "exceptions.price.platform.hpp"
#include "service.price.platform.hpp"

namespace antara::mmbot
{
    price_service_platform::price_service_platform() noexcept
    {
        const auto &cfg = antara::mmbot::get_mmbot_config();
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        for (auto[platform_name, platform_cfg]: cfg.price_registry) {
            auto current_price_platform_ptr = factory_price_platform::create(platform_name);
            if (current_price_platform_ptr != nullptr) {
                registry_platform_price_.emplace(platform_name, std::move(current_price_platform_ptr));
            }
        }
    }

    st_price price_service_platform::get_price(antara::pair currency_pair) const
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        struct TransformResult
        {
            absl::uint128 price = 0;
            size_t calls_succeeded = 0;
        };
        TransformResult result{};

        tf::Taskflow taskflow;
        taskflow.transform_reduce(
                begin(registry_platform_price_), end(registry_platform_price_), result,

                // reduce
                [](TransformResult a, TransformResult b) {
                    return TransformResult{
                            a.price + b.price,
                            a.calls_succeeded + b.calls_succeeded
                    };
                },

                // transform
                [&currency_pair](auto &&current_platform_price) {
                    auto &&platform_ptr = current_platform_price.second;
                    auto current_price = platform_ptr->get_price(currency_pair, 0u).value();
                    if (current_price > 0) {
                        return TransformResult{current_price, 1};
                    }

                    return TransformResult{};
                }
        );

        tf::Executor{}.run(taskflow).wait();

        if (result.calls_succeeded == 0) {
            throw errors::pair_not_available();
        }
        return st_price{result.price / result.calls_succeeded};
    }

    nlohmann::json price_service_platform::get_all_price_pairs_of_given_coin(const antara::asset &asset)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        nlohmann::json json_data = nlohmann::json::object();
        json_data[asset.symbol.value()] = nlohmann::json::array();
        auto functor = [&asset, this, &json_data](auto &&current_coin) {
            if (current_coin != asset.symbol.value()) {
                nlohmann::json current_data = nlohmann::json::object();
                antara::pair current_pair{antara::asset{st_symbol{current_coin}}, asset};
                try {
                    auto current_price = this->get_price(current_pair);
                    auto current_price_str = antara::get_price_as_string_decimal(get_mmbot_config(), asset.symbol,
                                                                                 current_pair.quote.symbol,
                                                                                 current_price);

                    current_data[asset.symbol.value() + "/" + current_coin] = current_price_str;
                    json_data[asset.symbol.value()].push_back(current_data);
                }
                catch (const antara::mmbot::errors::pair_not_available &e) {
                    VLOG_F(loguru::Verbosity_WARNING, "%s", e.what());
                }
            }
        };
        std::for_each(begin(coins_to_track_), end(coins_to_track_), functor);
        DVLOG_F(loguru::Verbosity_INFO, "json result: %s", json_data.dump().c_str());
        return json_data;
    }

    nlohmann::json price_service_platform::fetch_all_price()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        nlohmann::json json_data = nlohmann::json::array();
        std::for_each(begin(coins_to_track_), end(coins_to_track_), [&json_data, this](auto &&current_asset) {
            json_data.push_back(get_all_price_pairs_of_given_coin(antara::asset{st_symbol{current_asset}}));
        });
        DVLOG_F(loguru::Verbosity_INFO, "json result: %s", json_data.dump().c_str());
        return json_data;
    }

    price_service_platform::~price_service_platform() noexcept
    {
        this->keep_thread_alive_ = false;

        if (price_service_fetcher_.joinable()) {
            price_service_fetcher_.join();
        }
    }

    void price_service_platform::enable_price_service_thread()
    {
        price_service_fetcher_ = std::thread([this]() {
            loguru::set_thread_name("price sv thread");
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            using namespace std::literals;
            DVLOG_F(loguru::Verbosity_INFO, "%s", "fetching price begin");
            auto json_data = this->fetch_all_price();
            {
                std::scoped_lock lock(this->price_service_mutex_);
                this->price_registry_ = json_data;
            }
            DVLOG_F(loguru::Verbosity_INFO, "%s", "fetching price finished");
            while(this->keep_thread_alive_) {
                std::this_thread::sleep_for(30s);
                DVLOG_F(loguru::Verbosity_INFO, "%s", "fetching price begin");
                json_data = this->fetch_all_price();
                std::scoped_lock lock(this->price_service_mutex_);
                this->price_registry_ = json_data;
                DVLOG_F(loguru::Verbosity_INFO, "%s", "fetching price finished");
            }
        });
    }

    nlohmann::json price_service_platform::get_price_registry() noexcept
    {
        std::scoped_lock lock(this->price_service_mutex_);
        auto copy_json = this->price_registry_;
        return copy_json;
    }

}