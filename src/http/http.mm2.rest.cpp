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
    mm2::get_orderbook(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/getorderbook");
        const auto query_params = restinio::parse_query(req->header().query());
        if (query_params.size() != 2) {
            DVLOG_F(loguru::Verbosity_ERROR,
                    "Not enough parameters, require base_currency and quote_currency parameters");
            return req->create_response(restinio::status_bad_request()).done();
        }
        if (!query_params.has("base_currency") || !query_params.has("quote_currency")) {
            DVLOG_F(loguru::Verbosity_ERROR, "Wrong parameters, require base_asset and quote_asset parameters");
            return req->create_response(restinio::status_unprocessable_entity()).done();
        }
        antara::mmbot::mm2::orderbook_request orderbook_request{
                antara::cross::of(std::string(query_params["base_currency"]), std::string(query_params["quote_currency"]))};
        auto orderbook_answer = mm2_client_.rpc_orderbook(std::move(orderbook_request));
        auto answer_json = nlohmann::json::parse(orderbook_answer.result);
        auto final_status = restinio::http_status_line_t(
                static_cast<restinio::http_status_code_t>(orderbook_answer.rpc_result_code), "");
        return req->create_response(final_status).set_body(answer_json.dump()).done();
    }

    restinio::request_handling_status_t
    mm2::my_balance(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/my_balance");
        const auto query_params = restinio::parse_query(req->header().query());
        if (query_params.size() != 1) {
            DVLOG_F(loguru::Verbosity_ERROR,
                    "Not enough parameters, require currency parameters");
            return req->create_response(restinio::status_bad_request()).done();
        }
        if (!query_params.has("currency")) {
            DVLOG_F(loguru::Verbosity_ERROR, "Wrong parameters, require currency parameters");
            return req->create_response(restinio::status_unprocessable_entity()).done();
        }
        antara::mmbot::mm2::balance_request balance_request{
                antara::asset{st_symbol{std::string(query_params["currency"])}}};
        auto balance_answer = mm2_client_.rpc_balance(std::move(balance_request));
        auto answer_json = nlohmann::json::parse(balance_answer.result);
        auto final_status = restinio::http_status_line_t(
                static_cast<restinio::http_status_code_t>(balance_answer.rpc_result_code), "");
        return req->create_response(final_status).set_body(answer_json.dump()).done();
    }

    restinio::request_handling_status_t
    mm2::version(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/version");
        auto version_answer = mm2_client_.rpc_version();
        auto answer_json = nlohmann::json::parse(version_answer.result);
        auto final_status = restinio::http_status_line_t(
                static_cast<restinio::http_status_code_t>(version_answer.rpc_result_code), "");
        return req->create_response(final_status).set_body(answer_json.dump()).done();
    }

    restinio::request_handling_status_t
    mm2::set_price(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/setprice");
        return process_post_function<antara::mmbot::mm2::setprice_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_setprice(
                    std::forward<decltype(request)>(request));
            });
    }

    restinio::request_handling_status_t
    mm2::cancel_order(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/cancel_order");
        return process_post_function<antara::mmbot::mm2::cancel_order_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_cancel_order(
                    std::forward<decltype(request)>(
                        request));
            });
    }

    restinio::request_handling_status_t
    mm2::buy(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/buy");
        return process_post_function<antara::mmbot::mm2::trade_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_buy(
                    std::forward<decltype(request)>(request));
            });
    }

    restinio::request_handling_status_t
    mm2::cancel_all_orders(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/cancel_all_orders");
        return process_post_function<antara::mmbot::mm2::cancel_all_orders_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_cancel_all_orders(
                    std::forward<decltype(request)>(
                        request));
            });
    }

    restinio::request_handling_status_t
    mm2::get_enabled_coins(const restinio::request_handle_t &req, const restinio::router::route_params_t &)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/get_enabled_coins");
        auto get_enabled_coins_answer = mm2_client_.rpc_get_enabled_coins();
        auto answer_json = nlohmann::json::parse(get_enabled_coins_answer.result);
        auto final_status = restinio::http_status_line_t(
            static_cast<restinio::http_status_code_t>(get_enabled_coins_answer.rpc_result_code), "");
        return req->create_response(final_status).set_body(answer_json.dump()).done();
    }

    restinio::request_handling_status_t
    mm2::enable_all_electrums_coins(
        const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/mm2/enable_all_electrums_coins");
        mm2_client_.enable_all_coins();
        return get_enabled_coins(req, params);
    }

    restinio::request_handling_status_t
    mm2::sell(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/sell");
        return process_post_function<antara::mmbot::mm2::trade_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_sell(
                    std::forward<decltype(request)>(request));
            });
    }

    restinio::request_handling_status_t
    mm2::my_recent_swaps(const restinio::request_handle_t &req, const restinio::router::route_params_t &params)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "http call: %s", "/api/v1/legacy/mm2/my_recent_swaps");
        return process_post_function<antara::mmbot::mm2::my_recent_swaps_request>(
            req, params,
            [this](auto &&request) {
                return this->mm2_client_.rpc_my_recent_swaps(
                    std::forward<decltype(request)>(request));
            });
    }
}
