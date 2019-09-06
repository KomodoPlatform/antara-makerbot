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
#include <loguru.hpp>
#include <unordered_set>

#include "order.manager.hpp"

namespace antara::mmbot
{
    void order_manager::add_order_to_pair_map(const orders::order &o)
    {
        auto &id = o.id;
        auto &pair = o.pair;

        if (orders_by_pair_.find(pair) == orders_by_pair_.end()) {
            orders_by_pair_.emplace(pair, std::unordered_set<st_order_id>());
        }

        auto &ids = orders_by_pair_.at(pair);
        ids.emplace(id);
    }

    void order_manager::remove_order_from_pair_map(const orders::order &o)
    {
        auto id = o.id;
        auto pair = o.pair;

        auto& orders = orders_by_pair_.at(pair);
        orders.erase(id);
    }

    const orders::order &order_manager::get_order(const st_order_id &id) const
    {
        return orders_.at(id);
    }

    const std::unordered_set<st_order_id> &order_manager::get_orders(antara::pair pair) const
    {
        return orders_by_pair_.at(pair);
    }

    void order_manager::add_order(const orders::order &o)
    {
        orders_.emplace(o.id, o);
        add_order_to_pair_map(o);
        // Should we be adding the executions too?
        // I think not, so that we can track new executions outside of this function
    }

    void order_manager::add_orders(const std::vector<orders::order> &orders)
    {
        for (const auto &o : orders) {
            add_order(o);
        }
    }

    void order_manager::add_execution(const orders::execution &e)
    {
        executions_.emplace(e.id, e);
    }

    void order_manager::add_executions(const std::vector<orders::execution> &executions)
    {
        for (const auto &e : executions) {
            add_execution(e);
        }
    }

    void order_manager::remove_order(const orders::order &order)
    {
        auto ex_ids = order.execution_ids;
        for (auto &&current_id : ex_ids) {
            executions_.erase(current_id);
        }
        remove_order_from_pair_map(order);
        orders_.erase(order.id);
    }

    void order_manager::start()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);

        update_from_live();

        auto order_ids = std::unordered_set<st_order_id>();
        for (const auto&[id, o] : orders_) {
            order_ids.emplace(id);
        }

        auto exs = dex_.get_executions(order_ids);
        add_executions(exs);
    }

    void order_manager::poll()
    {
        // update the orders we know about
        for (const auto&[id, o] : orders_) {
            auto order = dex_.get_order_status(st_order_id{id});
            // orders_.emplace(id, order);
            add_order(order);
        }

        // add new orders
        update_from_live();

        // get all their executions
        auto order_ids = std::unordered_set<st_order_id>();
        for (const auto&[id, o] : orders_) {
            order_ids.emplace(id);
        }

        auto live_executions = dex_.get_executions(order_ids);
        auto recent_executions = dex_.get_recent_executions();
        auto all_executions = std::unordered_map<st_execution_id, orders::execution>();

        for (const auto &e : live_executions) {
            all_executions.emplace(e.id, e);
        }

        for (const auto &e : recent_executions) {
            all_executions.emplace(e.id, e);
        }

        for (const auto& [id, ex] : all_executions) {
            if (executions_.find(id) == executions_.end()) {
                // can't find the exection, it's new
                // for any that aren't in the ex object
                // make a call to cex
                cex_.mirror(ex);
                // and add to known executions
                add_execution(ex);
            }
        }

        // when an order is finished, remove it's executions
        for (auto&&[id, order] : orders_) {
            if (order.finished()) {
                remove_order(order);
            }
        }
    }

    void order_manager::update_from_live()
    {
        auto live = dex_.get_live_orders();
        for (const auto& order : live) {
            add_order(order);
        }
    }

    std::optional<st_order_id> order_manager::place_order(const orders::order_level &ol, antara::pair pair)
    {
        auto opt_order = dex_.place(ol, pair);
        if (opt_order) {
            auto &order = opt_order.value();
            add_order(order);
            return std::make_optional<st_order_id>(order.id);
        } else {
            return std::nullopt;
        }
    }

    std::unordered_set<st_order_id> order_manager::place_order(const orders::order_group &og)
    {
        auto pair = og.pair;
        auto order_ids = std::unordered_set<st_order_id>();
        for (const auto &ol : og.levels) {
            auto id = place_order(ol, pair);
            if (id) {
                order_ids.emplace(id.value());
            }
        }
        return order_ids;
    }

    std::unordered_set<st_order_id> order_manager::cancel_orders(antara::pair pair)
    {
        auto ids = orders_by_pair_.at(pair);

        std::unordered_set<st_order_id> cancelled_orders;
        for (const auto &id : ids) {
            auto result = dex_.cancel(id);
            if (result) {
                cancelled_orders.emplace(id);
                auto &order = orders_.at(id);
                remove_order(order);
            }
        }

        return cancelled_orders;
    }
}
