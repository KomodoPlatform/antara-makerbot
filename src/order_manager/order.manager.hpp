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

#include "utils/mmbot_strong_types.hpp"
#include "orders/orders.hpp"
#include "dex/dex.hpp"
#include "cex/cex.hpp"

namespace antara
{
    class order_manager
    {
    public:
        order_manager(const dex &dex, const cex &cex): dex_(dex), cex_(cex) {};

        [[nodiscard]] const orders::order &get_order(const st_order_id &id) const;
        [[nodiscard]] const orders::orders_by_id &get_all_orders() const;

        void start();
        void poll();

        st_order_id place_order(const orders::order_level &ol);
        std::vector<st_order_id> place_order(const orders::order_group &os);

    private:
        dex dex_;
        cex cex_;

        orders::orders_by_id orders_;
        orders::executions_by_id executions_;
    };
}
