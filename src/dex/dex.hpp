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

#include <vector>
#include <unordered_map>

#include <orders/orders.hpp>
#include <utils/mmbot_strong_types.hpp>

namespace antara
{
    class dex
    {
    public:
        st_order_id place(const orders::order_level &ol);

        std::vector<orders::order> get_live_orders();
        orders::order get_order_status(const st_order_id &id);

        std::vector<orders::execution> get_executions();
        std::vector<orders::execution> get_executions(const st_order_id &id);
        std::vector<orders::execution> get_executions(const std::vector<st_order_id> &ids);
        std::vector<orders::execution> get_recent_executions();
    };
}
