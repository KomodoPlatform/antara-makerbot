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
#include "strategy_manager/strategy.manager.hpp"
#include "order_manager/order.manager.hpp"

namespace antara::mmbot::http::rest
{
    class sm
    {
    public:
        sm(strategy_manager<price_service_platform> &sm, order_manager &om) noexcept;

        ~sm() noexcept;

        restinio::request_handling_status_t
        add_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        get_strategy(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        cancel_orders(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

    private:
        strategy_manager<price_service_platform> &sm_;
        order_manager &om_;
    };
}
