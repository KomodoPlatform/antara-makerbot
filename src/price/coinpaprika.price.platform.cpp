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

#include <exception>
#include <nlohmann/json.hpp>
#include <restclient-cpp/restclient.h>
#include "coinpaprika.price.platform.hpp"

namespace antara::mmbot
{
    st_price coinpaprika_price_platform::get_price(antara::pair currency_pair)
    {
        if (this->coin_id_translation_.find(currency_pair.base.value()) != this->coin_id_translation_.end()) {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            std::string path = "/tickers/" + this->coin_id_translation_.at(currency_pair.base.value());
            if (!currency_pair.quote.value().empty()) {
                path += "?quotes=" + currency_pair.quote.value();
            }
            auto final_uri = mmbot_config_.prices_registry.at("coinpaprika").price_endpoint.value() + path;
            DVLOG_F(loguru::Verbosity_INFO, "request: %s", final_uri.c_str());
            auto response = RestClient::get(final_uri);
            DVLOG_F(loguru::Verbosity_INFO, "response: %s\nstatus: %d", response.body.c_str(), response.code);
            if (response.code == 200) {
                auto resp_json = nlohmann::json::parse(response.body);
                auto price = st_price{resp_json["quotes"][currency_pair.quote.value()]["price"].get<double>()};
                return price;
            } else {
                DVLOG_F(loguru::Verbosity_ERROR, "http error: %d", response.code);
            }
        } else {
            DVLOG_F(loguru::Verbosity_ERROR, "base: %s not found", currency_pair.base.value().c_str());
        }
        return st_price{0.0};
    }
}