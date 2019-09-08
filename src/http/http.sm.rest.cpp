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

#include "http.sm.rest.hpp"

namespace antara::mmbot::http::rest
{
    sm::sm(strategy_manager<price_service_platform> &sm, order_manager &om) noexcept : sm_(sm), om_(om)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    sm::~sm() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    restinio::request_handling_status_t
    sm::add_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/sm/addstrategy");

        auto json_data = nlohmann::json::parse(req->body());
        market_making_strategy strat;
        from_json(json_data, strat);
        this->sm_.add_strategy(strat);

        auto status = restinio::status_ok();
        return req->create_response(status).done();
    }

    restinio::request_handling_status_t
    sm::get_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/sm/getstrategy");

        auto json_data = nlohmann::json::parse(req->body());
        antara::pair pr;
        from_json(json_data.at("pair"), pr);
        auto cross = pr.to_cross();
        auto strat = this->sm_.get_strategy(cross);
        nlohmann::json json_strat;
        to_json(json_strat, strat);

        auto status = restinio::status_ok();
        return req->create_response(status)
            .append_header(restinio::http_field::content_type, "application/json")
            .set_body(json_strat.dump())
            .done();
    }

    restinio::request_handling_status_t
    sm::cancel_orders(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/sm/getstrategy");

        auto json_data = nlohmann::json::parse(req->body());
        antara::pair pr;
        from_json(json_data, pr);
        auto cross = pr.to_cross();
        auto ids = this->om_.cancel_orders(cross);
        nlohmann::json json_ids;
        nlohmann::to_json(json_ids, ids);

        auto status = restinio::status_ok();
        return req->create_response(status)
            .append_header(restinio::http_field::content_type, "application/json")
            .set_body(json_ids.dump())
            .done();
    }
}
