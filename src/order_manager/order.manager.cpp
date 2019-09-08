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
        auto cross = o.pair.to_cross();

        this->orders_by_pair_mutex_.lock();
        if (orders_by_pair_.find(cross) == orders_by_pair_.end()) {
            orders_by_pair_.emplace(cross, std::unordered_set<st_order_id>());
        }

        auto &ids = orders_by_pair_.at(cross);
        this->orders_by_pair_mutex_.unlock();
        ids.emplace(id);
    }

    void order_manager::remove_order_from_pair_map(const orders::order &o)
    {
        auto id = o.id;
        auto cross = o.pair.to_cross();

        std::scoped_lock locker(this->orders_by_pair_mutex_);
        auto &orders = orders_by_pair_.at(cross);
        orders.erase(id);
    }

    const orders::order &order_manager::get_order(const st_order_id &id) const
    {
        return orders_.at(id);
    }

    const std::unordered_set<st_order_id> &order_manager::get_orders(antara::cross cross) const
    {
        return orders_by_pair_.at(cross);
    }

    void order_manager::add_order(const orders::order &o)
    {
        this->orders_mutex_.lock();
        orders_.emplace(o.id, o);
        this->orders_mutex_.unlock();
        add_order_to_pair_map(o);
        // Should we be adding the executions too?
        // I think not, so that we can track new executions outside of this function
    }

    void order_manager::add_orders(const std::vector<orders::order> &orders)
    {
        //! Locked by subcall
        for (const auto &o : orders) {
            add_order(o);
        }
    }

    void order_manager::add_execution(const orders::execution &e)
    {
        std::scoped_lock locker(this->executions_mutex_);
        executions_.emplace(e.id, e);
    }

    void order_manager::add_executions(const std::vector<orders::execution> &executions)
    {
        //! Locked by subcall.
        for (const auto &e : executions) {
            add_execution(e);
        }
    }

    void order_manager::remove_order(const orders::order &order)
    {
        auto ex_ids = order.execution_ids;
        {
            std::scoped_lock execution_locker(this->executions_mutex_);
            for (auto &&current_id : ex_ids) {
                executions_.erase(current_id);
            }
        }
        remove_order_from_pair_map(order);
        std::scoped_lock locker(this->orders_mutex_);
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

        for (const auto&[id, ex] : all_executions) {
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
        for (const auto &order : live) {
            add_order(order);
        }
    }

    std::optional<st_order_id> order_manager::place_order(const orders::order_level &ol)
    {
        auto opt_order = dex_.place(ol);
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
        auto order_ids = std::unordered_set<st_order_id>();
        for (const auto &ol : og.levels) {
            auto id = place_order(ol);
            if (id) {
                order_ids.emplace(id.value());
            }
        }
        return order_ids;
    }

    std::unordered_set<st_order_id> order_manager::cancel_orders(antara::cross pair)
    {
        this->orders_by_pair_mutex_.lock();
        auto ids = orders_by_pair_.at(pair);
        this->orders_by_pair_mutex_.unlock();

        std::unordered_set<st_order_id> cancelled_orders;
        for (const auto &id : ids) {
            auto result = dex_.cancel(id);
            if (result) {
                cancelled_orders.emplace(id);
                this->orders_mutex_.lock();
                auto &order = orders_.at(id);
                this->orders_mutex_.unlock();
                remove_order(order);
            }
        }

        return cancelled_orders;
    }

    void order_manager::enable_om_service_thread()
    {
        using namespace std::literals;
        this->om_thread_ = std::thread([this]() {
            loguru::set_thread_name("om thread");
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            while (this->keep_thread_alive_) {
                DVLOG_F(loguru::Verbosity_INFO, "%s", "polling started");
                this->poll();
                DVLOG_F(loguru::Verbosity_INFO, "%s", "polling finished, waiting 10s before next polling");
                std::this_thread::sleep_for(10s);
            }
        });
    }

    order_manager::~order_manager()
    {
        this->keep_thread_alive_ = false;
        if (om_thread_.joinable()) {
            om_thread_.join();
        }
    }
}
