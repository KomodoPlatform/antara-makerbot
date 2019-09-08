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

#include "http.mm2.rest.hpp"

namespace antara::mmbot::http::rest
{
    mm2::mm2(mm2_client &mm2_client) noexcept : mm2_client_(mm2_client)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    mm2::~mm2() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    restinio::request_handling_status_t
    sm::add_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/sm/addstrategy");

        auto json_data = nlohman::json::parse(req->body());
        auto strat = json_data.get<market_making_strategy>();
        this->sm.add_strategy(strat);

        return req->create_response(status).done();
    }

    restinio::request_handling_status_t
    sm::get_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/sm/getstrategy");

        auto json_data = nlohman::json::parse(req->body());
        auto cross = json_data.get<antara::pair>().to_cross();
        json strat = this->sm.get_strategy(cross);

        return req->create_response(status)
            .append_header(restinio::http_field::content_type, "application/json")
            .set_body(strat.dump())
            .done();
    }
}
