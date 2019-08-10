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
    template<class DexImpl>
    class order_manager
    {
    public:
        order_manager(DexImpl &dex, cex &cex) : dex_(dex), cex_(cex)
        {};
        // order_manager(const DexImpl &dex, const cex &cex);

        [[nodiscard]] const orders::order &get_order(const st_order_id &id) const;

        [[nodiscard]] const orders::orders_by_id &get_all_orders() const
        {
            return orders_;
        }

        void start()
        {
            update_from_live();

            auto order_ids = std::vector<st_order_id>();
            std::transform(orders_.begin(), orders_.end(),
                           std::back_inserter(order_ids),
                           [](const auto &pair) {
                               return st_order_id{pair.first};
                           }
            );

            auto exs = dex_.get_executions(order_ids);
            for (const auto &ex : exs) {
                executions_.emplace(ex.id.value(), ex);
            }

            // for (const auto& [id, o] : orders_) {
            //     st_order_id order_id = st_order_id{id};
            //     auto exs = dex_.get_executions(order_id);
            //     for (const auto& ex : exs) {
            //         executions_.emplace(ex.id.value(), ex);
            //     }
            // }
        }

        void poll();

        void update_from_live()
        {
            auto live = dex_.get_live_orders();
            std::transform(live.begin(), live.end(), std::inserter(orders_, orders_.end()),
                           [] (const auto &o) {
                               return std::make_pair(o.id.value(), o);
                           }
            );
        }

        st_order_id place_order(const orders::order_level &ol);

        std::vector<st_order_id> place_order(const orders::order_group &os);

    private:
        DexImpl &dex_;
        cex &cex_;

        orders::orders_by_id orders_;
        orders::executions_by_id executions_;
    };
}
