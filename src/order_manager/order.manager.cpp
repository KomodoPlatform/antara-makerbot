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

#include <algorithm>
#include <iterator>

#include "order.manager.hpp"

namespace antara
{
    // template<class DexImpl>
    // order_manager(DexImpl dex, cex cex) // -> order_manager<DexImpl>
    // {
    //     this->dex_ = dex;
    //     this->cex_ = cex;
    // }

    template<class DexImpl>
    const orders::order &order_manager<DexImpl>::get_order(const st_order_id &id) const
    {
        return orders_.at(id.value());
    }

    template<class DexImpl>
    const orders::orders_by_id &order_manager<DexImpl>::get_all_orders() const
    {
        return orders_;
    }

    template<class DexImpl>
    void order_manager<DexImpl>::start()
    {
        update_from_live();

        auto order_ids = std::vector<st_order_id>();
        std::transform(orders_.begin(), orders_.end(),
                      std::back_inserter(order_ids),
                      [] (const auto& pair) {
                          return st_order_id{pair.first};
                      }
            );

        auto exs = dex_.get_executions(order_ids);
        for (const auto& ex : exs) {
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

    template<class DexImpl>
    void order_manager<DexImpl>::poll()
    {
        // update the orders we know about
        for (const auto& [id, o] : orders_) {
            auto order = dex_.get_order_status(st_order_id{id});
            orders_.emplace(id, order);
        }

        // add new orders
        update_from_live();

        // get all their executions
        auto order_ids = std::vector<st_order_id>();
        std::transform(orders_.begin(), orders_.end(),
                      std::back_inserter(order_ids),
                      [] (const auto& pair) {
                          return st_order_id{pair.first};
                      }
            );

        auto all_executions = dex_.get_executions(order_ids);
        auto recent_executions = dex_.get_recent_executions();
        std::copy(recent_executions.begin(), recent_executions.end(), std::back_inserter(all_executions));

        for (const auto& ex : all_executions) {
            if (executions_.find(ex.id.value()) != executions_.end()) {
                // can't find the exection, it's new
                // for any that aren't in the ex object
                // make a call to cex
                cex_.mirror(ex);
            }
        }

        // when an order is finished, remove it's executions
        for (const auto& [id, order] : orders_) {
            if (order.finished()) {
                std::vector<st_execution_id> ex_ids = order.execution_ids;
                for (const auto& id : ex_ids) {
                    executions_.erase(id.value());
                }
                orders_.erase(order.id.value());
            }
        }
    }

    template<class DexImpl>
    st_order_id order_manager<DexImpl>::place_order(const orders::order_level &ol)
    {
        return dex_.place(ol);
    }

    template<class DexImpl>
    std::vector<st_order_id> order_manager<DexImpl>::place_order(const orders::order_group &os)
    {
        auto order_ids = std::vector<st_order_id>();
        for (const auto& ol : os.levels) {
            auto id = dex_.place(ol);
            order_ids.push_back(id);
        }

        return order_ids;
    }

    template<class DexImpl>
    void order_manager<DexImpl>::update_from_live()
    {
        auto live = dex_.get_live_orders();
        std::transform(live.begin(), live.end(), std::inserter(orders_, orders_.end()),
                      [] (const auto &o) {
                          return std::make_pair(o.id.value(), o);
                      }
            );
    }
}
