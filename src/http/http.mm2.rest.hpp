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

#pragma once

#include <restinio/all.hpp>
#include <restinio/common_types.hpp>
#include "config/config.hpp"
#include "mm2/mm2.client.hpp"

namespace antara::mmbot::http::rest
{
    class mm2
    {
    public:
        mm2(mm2_client &mm2_client) noexcept;

        ~mm2() noexcept;

        restinio::request_handling_status_t
        get_orderbook(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        my_orders(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        my_balance(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        version(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        get_enabled_coins(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        my_recent_swaps(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        enable_all_electrums_coins(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        set_price(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        buy(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        withdraw(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        sell(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        cancel_all_orders(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        restinio::request_handling_status_t
        cancel_order(const restinio::request_handle_t &req, const restinio::router::route_params_t &params);

        template<typename TRequest, typename Functor>
        restinio::request_handling_status_t process_post_function(
            const restinio::request_handle_t &req, const restinio::router::route_params_t &, Functor&& rpc_functor)
        {
            nlohmann::json json_answer;
            auto status = restinio::status_ok();
            try {
                auto json_data = nlohmann::json::parse(req->body());
                TRequest request;
                mmbot::mm2::from_json(json_data, request);
                auto answer = rpc_functor(std::move(request));
                status = restinio::http_status_line_t(
                        static_cast<restinio::http_status_code_t>(answer.rpc_result_code), "");
                json_answer = nlohmann::json::parse(answer.result);
            }
            catch (const nlohmann::json::exception &error) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "json error: %s", error.what());
                return req->create_response(restinio::status_bad_request()).set_body(error.what()).done();
            }
            return req->create_response(status)
                .append_header(restinio::http_field::content_type, "application/json")
                .set_body(json_answer.dump())
                .done();
        }

    private:
        mm2_client &mm2_client_;
    };
}
