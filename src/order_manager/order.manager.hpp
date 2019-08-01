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

namespace antara
{
    class order_manager
    {
    public:
        using registry_order_sets = std::unordered_map<antara::pair, antara::order_set>;

        bool place_order(orders:order_level);
        bool place_orders(orders::order_set);

        [[nodiscard]] const orders::order_set &get_orders(antara::pair &pair) const;
        [[nodiscard]] const registry_order_sets &get_all_orders() const;

    private:
        registry_order_sets registry_order_sets_;
    }
}
