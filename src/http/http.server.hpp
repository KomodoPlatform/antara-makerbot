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

#include <memory>
#include <restinio/all.hpp>
#include <config/config.hpp>
#include "price/service.price.platform.hpp"
#include "http.price.rest.hpp"
#include "mm2/mm2.client.hpp"
#include "http.mm2.rest.hpp"
#include "http.sm.rest.hpp"

namespace antara::mmbot
{
    struct http_server_traits : public restinio::default_single_thread_traits_t
    {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    class http_server
    {
    public:
        using router = std::unique_ptr<restinio::router::express_router_t<>>;

        explicit http_server(
            price_service_platform &price_service,
            mmbot::mm2_client &mm2_client,
            mmbot::strategy_manager<price_service_platform> &sm,
            mmbot::order_manager &om);

        void run();

    private:
        router create_routes();

    private:
        http::rest::price price_rest_callbook_;
        http::rest::mm2 mm2_rest_callbook_;
        http::rest::sm sm_rest_callbook_;
    };
}
