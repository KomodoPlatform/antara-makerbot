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
#include <loguru.hpp>

#include <utils/pretty_function.hpp>
#include "utils/mmbot_strong_types.hpp"
#include "orders/orders.hpp"
#include "dex/dex.hpp"
#include "cex/cex.hpp"

namespace antara
{
    class order_manager
    {
    public:
        order_manager(abstract_dex& dex, abstract_cex& cex) : dex_(dex), cex_(cex)
        {}


        [[nodiscard]] const orders::order &get_order(const st_order_id &id) const;

        [[nodiscard]] const orders::orders_by_id &get_all_orders() const
        {
            return orders_;
        }

        void add_orders(const std::vector<orders::order> &o);

        void add_executions(const std::vector<orders::execution> &e);

        void start();

        void poll();

        void update_from_live();

        st_order_id place_order(const orders::order_level &ol);

        std::unordered_set<st_order_id> place_order(const orders::order_group &os);

    private:
        abstract_dex& dex_;
        abstract_cex& cex_;

        orders::orders_by_id orders_;
        orders::executions_by_id executions_;
    };
}
