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
        using registry_order_sets = std::unordered_map<antara::pair, orders::order_set>;
        order_manager() = default;

        bool place_order(const orders::order_level &ol);
        bool place_orders(const orders::order_set &os);

        [[nodiscard]] const orders::order_set &get_orders(antara::pair &pair) const;
        [[nodiscard]] const registry_order_sets &get_all_orders() const;

        void change_order_status(const orders::order_status_change &osc);

    private:
        registry_order_sets registry_order_sets_;
        dex dex_;
        cex cex_;
    };
}
