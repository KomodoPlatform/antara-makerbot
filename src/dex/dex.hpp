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
#include <unordered_set>
#include <unordered_map>

#include <orders/orders.hpp>
#include <utils/mmbot_strong_types.hpp>

namespace antara
{
    class abstract_dex
    {
    public:
        virtual ~abstract_dex() = default;

        virtual st_order_id place(const orders::order_level &ol) = 0;

        virtual std::vector<orders::order> get_live_orders() = 0;
        virtual orders::order get_order_status(const st_order_id &id) = 0;

        virtual std::vector<orders::execution> get_executions() = 0;
        virtual std::vector<orders::execution> get_executions(const st_order_id &id) = 0;
        virtual std::vector<orders::execution> get_executions(const std::unordered_set<st_order_id> &ids) = 0;
        virtual std::vector<orders::execution> get_recent_executions() = 0;
    };

    class dex : public abstract_dex
    {
    public:
        st_order_id place(const orders::order_level &ol) override;

        std::vector<orders::order> get_live_orders() override;
        orders::order get_order_status(const st_order_id &id) override;

        std::vector<orders::execution> get_executions() override;
        std::vector<orders::execution> get_executions(const st_order_id &id) override;
        std::vector<orders::execution> get_executions(const std::unordered_set<st_order_id> &ids) override;
        std::vector<orders::execution> get_recent_executions() override;
    };
}
