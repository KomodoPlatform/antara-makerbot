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
        std::atomic_size_t nb_calls_succeed = 0u;
        absl::uint128 price = 0;
        std::mutex mutex;
        auto price_functor = [&nb_calls_succeed, &mutex, &price, &currency_pair](auto &&current_platform_price) {
            auto &&[platform_name, platform_ptr] = current_platform_price;
            auto current_price = platform_ptr->get_price(currency_pair, 0u).value();
            if (current_price > 0) {
                ++nb_calls_succeed;
            }
            {
                auto const lck = std::lock_guard(mutex);
                price = price + current_price;
            }
        };

        if (!registry_platform_price_.empty()) {
            antara::par_for_each(begin(registry_platform_price_), end(registry_platform_price_), price_functor);
        }

        if (nb_calls_succeed == 0) {
            throw errors::pair_not_available();
        }
        return st_price{price / nb_calls_succeed.load()};
    }

    nlohmann::json price_service_platform::get_all_price_pairs_of_given_coin(const antara::asset &asset)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        nlohmann::json json_data = nlohmann::json::object();
        json_data[asset.symbol.value()] = nlohmann::json::array();
        std::mutex mutex;
        auto functor = [&asset, this, &json_data, &mutex](auto &&current_coin) {
            std::scoped_lock lock(mutex);
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
        antara::par_for_each(begin(coins_to_track), end(coins_to_track), functor);
        DVLOG_F(loguru::Verbosity_INFO, "json result: %s", json_data.dump().c_str());
        return json_data;
    }

    nlohmann::json price_service_platform::fetch_all_price()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        nlohmann::json json_data = nlohmann::json::array();
        std::mutex mutex;
        antara::par_for_each(begin(coins_to_track), end(coins_to_track), [&json_data, &mutex, this](auto &&current_asset) {
            std::scoped_lock lock(mutex);
            json_data.push_back(get_all_price_pairs_of_given_coin(antara::asset{st_symbol{current_asset}}));
        });
        DVLOG_F(loguru::Verbosity_INFO, "json result: %s", json_data.dump().c_str());
        return json_data;
    }

}