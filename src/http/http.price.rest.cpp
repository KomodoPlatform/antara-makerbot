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

#include <price/exceptions.price.platform.hpp>
#include <utils/antara.utils.hpp>
#include "http.price.rest.hpp"

namespace antara::mmbot::http::rest
{
    price::price(const config& cfg, price_service_platform &price_service) noexcept : price_service_(price_service), mmbot_config_(cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    price::~price() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    restinio::request_handling_status_t
    price::get_price(const restinio::request_handle_t& req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/getprice");
        const auto qp = restinio::parse_query(req->header().query());
        if (qp.size() != 2) {
            DVLOG_F(loguru::Verbosity_ERROR,
                    "Not enough parameters, require base_currency and quote_currency parameters");
            return req->create_response(restinio::status_bad_request()).done();
        }
        if (!qp.has("base_currency") || !qp.has("quote_currency")) {
            DVLOG_F(loguru::Verbosity_ERROR, "Wrong parameters, require base_asset and quote_asset parameters");
            return req->create_response(restinio::status_unprocessable_entity()).done();
        }
        antara::pair currency_pair{asset{st_symbol{std::string(qp["quote_currency"])}},
                                   asset{st_symbol{std::string(qp["base_currency"])}}};
        st_price price{0ull};
        nlohmann::json answer_json;
        try {
            price = price_service_.get_price(currency_pair);
            answer_json = {"price", get_price_as_string_decimal(mmbot_config_, currency_pair.quote.symbol, price)};
        }
        catch (const antara::mmbot::errors::pair_not_available& e) {
            nlohmann::json error_json = {"errors", e.what()};
            req->create_response(restinio::status_internal_server_error()).set_body(error_json.dump()).done();
        }
        return req->create_response(restinio::status_ok()).set_body(answer_json.dump()).done();
    }
}