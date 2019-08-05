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

#include <utils/mmbot_strong_types.hpp>
#include <orders/orders.hpp>
#include <dex/dex.hpp>
#include <cex/cex.hpp>

namespace antara
{
    class order_manager
    {
    public:
        order_manager() = default;

        using order_sets_by_pair = std::unordered_map<antara::pair, orders::order_set>;

        [[nodiscard]] const orders::order_set &get_orders(const antara::pair &pair) const;
        [[nodiscard]] const order_sets_by_pair &get_all_order_sets() const;

        using orders_by_id = std::unordered_map<st_order_id, orders::order>;

        [[nodiscard]] const orders::order &get_order(const st_order_id &id) const;
        [[nodiscard]] const orders_by_id &get_all_orders() const;

        bool place_order(const orders::order_level &ol);
        bool place_order(const orders::order &o);
        bool place_orders(const orders::order_set &os);

        void change_order_status(const orders::order_status_change &osc);

    private:
        order_sets_by_pair order_sets_by_pair_;
        orders_by_id orders_by_id_;

        dex dex_;
        cex cex_;
    };
}
