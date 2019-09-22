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
#include "utils/antara.utils.hpp"
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
                    this->coin_id_translation_.at(currency_pair.quote.symbol.value()) +
                    "&quote_currency_id=" + this->coin_id_translation_.at(currency_pair.base.symbol.value()) +
                    "&amount=1";
            const auto &mmbot_config = get_mmbot_config();
            auto final_uri = mmbot_config.price_registry.at("coinpaprika").price_endpoint.value() + path;
            DVLOG_F(loguru::Verbosity_INFO, "request: %s", final_uri.c_str());
            auto response = RestClient::get(final_uri);
            DVLOG_F(loguru::Verbosity_INFO, "response: %s\nstatus: %d", response.body.c_str(), response.code);
            if (response.code == 200) {
                antara::my_json_sax sx;
                nlohmann::json::sax_parse(response.body, &sx);
                auto price = generate_st_price_from_api_price(mmbot_config, currency_pair.quote.symbol,
                                                              sx.float_as_string);
                return price;
            } else if (response.code == 429 && nb_try_in_a_row < 10) {
                DVLOG_F(loguru::Verbosity_WARNING, "got a 429 (api rate limits) retrying in one seconds");
                using namespace std::chrono_literals;
                std::this_thread::sleep_for(1s);
                ++nb_try_in_a_row;
                return get_price(currency_pair, nb_try_in_a_row);
            } else {
                DVLOG_F(loguru::Verbosity_ERROR, "http error: %d", response.code);
            }
        } else {
            DVLOG_F(loguru::Verbosity_ERROR, "base: %s not found or quote: %s",
                    currency_pair.base.symbol.value().c_str(), currency_pair.quote.symbol.value().c_str());
        }
        return st_price{0};
    }
}
