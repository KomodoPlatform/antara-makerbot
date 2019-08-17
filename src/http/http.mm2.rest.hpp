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
        mm2(const config &cfg, mm2_client &mm2_client) noexcept;

        ~mm2() noexcept;

        restinio::request_handling_status_t
        get_orderbook(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

        restinio::request_handling_status_t
        my_balance(const restinio::request_handle_t &req, const restinio::router::route_params_t &);

    private:
        [[maybe_unused]] const config &mmbot_config_;
        mm2_client &mm2_client_;
    };
}