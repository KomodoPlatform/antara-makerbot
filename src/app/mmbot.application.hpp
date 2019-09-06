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

#include <http/http.server.hpp>
#include <order_manager/order.manager.hpp>
#include "cex/cex.hpp"
#include "dex/dex.hpp"

namespace antara::mmbot
{
    class application
    {
    public:
        application() noexcept;
        ~application() noexcept;
        int run();
    private:
        price_service_platform price_service_;
        mm2_client mm2_client_;
        mmbot::dex dex_{mm2_client_};
        mmbot::cex cex_{};
        mmbot::order_manager om_{dex_, cex_};
        antara::mmbot::http_server server_{price_service_, mm2_client_};
    };
}