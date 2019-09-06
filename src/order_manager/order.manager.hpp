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

#include <thread>
#include <atomic>
#include <vector>
#include <unordered_map>
#include <loguru.hpp>
#include <optional>

#include <utils/pretty_function.hpp>
#include "utils/mmbot_strong_types.hpp"
#include "orders/orders.hpp"
#include "mm2/mm2.client.hpp"
#include "dex/dex.hpp"
#include "cex/cex.hpp"

namespace antara::mmbot
{
    class abstract_om
    {
    public:
        virtual ~abstract_om() = default;

        virtual const orders::order &get_order(const st_order_id &id) const = 0;
        virtual const orders::orders_by_id &get_all_orders() const = 0;
        virtual const std::unordered_set<st_order_id> &get_orders(antara::pair pair) const = 0;

        virtual const orders::executions_by_id &get_all_executions() const = 0;

        virtual void add_order(const orders::order &o) = 0;
        virtual void add_orders(const std::vector<orders::order> &o) = 0;

        virtual void add_execution(const orders::execution &e) = 0;
        virtual void add_executions(const std::vector<orders::execution> &e) = 0;

        virtual void remove_order(const orders::order &o) = 0;

        virtual void start() = 0;
        virtual void poll() = 0;

        virtual void update_from_live() = 0;

        virtual std::optional<st_order_id> place_order(const orders::order_level &ol, antara::pair pair) = 0;
        virtual std::unordered_set<st_order_id> place_order(const orders::order_group &os) = 0;

        virtual std::unordered_set<st_order_id> cancel_orders(antara::pair pair) = 0;
    };

    class order_manager : public abstract_om
    {
    public:
        order_manager(abstract_dex& dex, abstract_cex& cex) : dex_(dex), cex_(cex)
        {}
        ~order_manager();

        void add_order_to_pair_map(const orders::order &o);
        void remove_order_from_pair_map(const orders::order &o);

        [[nodiscard]] const orders::order &get_order(const st_order_id &id) const override;
        [[nodiscard]] const orders::orders_by_id &get_all_orders() const override
        {
            return orders_;
        }
        const std::unordered_set<st_order_id> &get_orders(antara::pair pair) const override;

        [[nodiscard]] const orders::executions_by_id &get_all_executions() const override
        {
            return executions_;
        }

        void add_order(const orders::order &o) override;
        void add_orders(const std::vector<orders::order> &o) override;

        void add_execution(const orders::execution &e) override;
        void add_executions(const std::vector<orders::execution> &e) override;

        void remove_order(const orders::order &o) override;

        void start() override;
        void poll() override;

        void update_from_live() override;

        std::optional<st_order_id> place_order(const orders::order_level &ol, antara::pair pair) override;
        std::unordered_set<st_order_id> place_order(const orders::order_group &os) override;

        std::unordered_set<st_order_id> cancel_orders(antara::pair pair) override;


        void enable_om_service_thread();

    private:
        abstract_dex& dex_;
        abstract_cex& cex_;

        orders::orders_by_id orders_;
        orders::executions_by_id executions_;

        //! Thread stuffs
        std::thread om_thread_;
        std::atomic_bool keep_thread_alive_{true};
        std::mutex orders_mutex_;
        std::mutex executions_mutex_;
        std::mutex orders_by_pair_mutex_;

        std::unordered_map<antara::pair, std::unordered_set<st_order_id>> orders_by_pair_;
    };
}
