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

#include "order.manager.hpp"

namespace antara
{
    bool order_manager::place_order(const orders::order_level &ol)
    {
        // for manually placing orders
        // the strat manager will use place_orders
        // registry_order_sets.emplace
        return true;
    }

    bool order_manager::place_order(const orders::order &o)
    {
        st_order_id id = dex_.place(o);
        orders_by_id_.emplace(id, o);
        return true;
    }

    bool order_manager::place_orders(const orders::order_set &os)
    {
        order_sets_by_pair_.emplace(os.pair, os);

        // the return value will indicate success
        // could be a specific return type, or throw exception on failure, not sure yet
        return true;
    }

    // void order_manager::change_order_status(const orders::order_status_change &osc)
    // {
    //     // if the dex order has been filled, place the order on the cex
    //     // take account of partial fills

    //     // orders::order_set os = order_sets_by_pair_.at(osc.pair);

    //     // orders::order_level ol;

    //     // cex_.place_order(ol);
    // }

    const orders::order_set &order_manager::get_orders(const antara::pair &pair) const
    {
        return order_sets_by_pair_.at(pair);
    }

    const order_manager::order_sets_by_pair &order_manager::get_all_order_sets() const
    {
        return order_sets_by_pair_;
    }

    const orders::order &order_manager::get_order(const st_order_id &id) const
    {
        return orders_by_id_.at(id);
    }

    const order_manager::orders_by_id &order_manager::get_all_orders() const
    {
        return orders_by_id_;
    }
}
