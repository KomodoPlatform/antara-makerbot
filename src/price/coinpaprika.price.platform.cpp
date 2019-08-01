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

#include <thread>
#include <nlohmann/json.hpp>
#include <restclient-cpp/restclient.h>
#include <thread>
#include "coinpaprika.price.platform.hpp"

namespace antara::mmbot
{
    st_price coinpaprika_price_platform::get_price(antara::pair currency_pair, std::size_t nb_try_in_a_row) const
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        if (this->coin_id_translation_.find(currency_pair.base.symbol.value()) != this->coin_id_translation_.end() &&
            this->coin_id_translation_.find(currency_pair.quote.symbol.value()) != this->coin_id_translation_.end()) {
            std::string path =
                    "/price-converter?base_currency_id=" +
                    this->coin_id_translation_.at(currency_pair.base.symbol.value()) +
                    "&quote_currency_id=" + this->coin_id_translation_.at(currency_pair.quote.symbol.value()) +
                    "&amount=1";
            auto final_uri = mmbot_config_.price_registry.at("coinpaprika").price_endpoint.value() + path;
            DVLOG_F(loguru::Verbosity_INFO, "request: %s", final_uri.c_str());
            auto response = RestClient::get(final_uri);
            DVLOG_F(loguru::Verbosity_INFO, "response: %s\nstatus: %d", response.body.c_str(), response.code);
            if (response.code == 200) {
                auto resp_json = nlohmann::json::parse(response.body);
                auto price = st_price{static_cast<std::uint64_t>(resp_json["price"].get<double>() * g_factor)};
                return price;
            } else if (response.code == 429 && nb_try_in_a_row < 10) {
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1s);
                ++nb_try_in_a_row;
                return get_price(currency_pair, nb_try_in_a_row);
            } else {
                DVLOG_F(loguru::Verbosity_ERROR, "http error: %d", response.code);
            }
        } else {
            DVLOG_F(loguru::Verbosity_ERROR, "base: %s not found", currency_pair.base.symbol.value().c_str());
        }
        return st_price{0};
    }
}
