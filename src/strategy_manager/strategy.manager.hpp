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
#include <order_manager/order.manager.hpp>
#include <price/service.price.platform.hpp>

namespace antara::mmbot
{
    struct market_making_strategy
    {
        antara::cross pair;
        antara::st_spread spread;
        antara::st_quantity quantity;
        bool both;
        // antara::side side;
        bool operator==(const market_making_strategy &other) const;
        bool operator!=(const market_making_strategy &other) const;
    };

    template <class PS>
    class abstract_sm
    {
    public:
        using registry_strategies = std::unordered_map<antara::cross, market_making_strategy>;

        virtual ~abstract_sm() = default;

        virtual void add_strategy(const market_making_strategy& strat) = 0;

        virtual const market_making_strategy &get_strategy(antara::cross pair) const = 0;
        virtual const registry_strategies &get_strategies() const = 0;

        virtual orders::order_level make_bid(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) = 0;

        virtual orders::order_level make_ask(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) = 0;

        virtual orders::order_group create_order_group(
            const market_making_strategy &strat, antara::st_price mid) = 0;

        virtual orders::order_group create_order_group(const market_making_strategy &strat) = 0;
    };

    template <class PS>
    class strategy_manager : public abstract_sm<PS>
    {
    public:
        using registry_strategies = std::unordered_map<antara::cross, market_making_strategy>;

        strategy_manager(PS& ps, abstract_om& om): om_(om), ps_(ps)
        {
            running_ = true;
        }

        void add_strategy(const market_making_strategy& strat) override;

        [[nodiscard]] const market_making_strategy &get_strategy(antara::cross pair) const override;
        [[nodiscard]] const registry_strategies &get_strategies() const override;

        orders::order_level make_bid(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) override;

        orders::order_level make_ask(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) override;

        orders::order_group create_order_group(
            const market_making_strategy &strat, antara::st_price mid) override;

        orders::order_group create_order_group(const market_making_strategy &strat) override;

        void refresh_orders(antara::cross pair);
        void refresh_all_orders();

        void start();

    private:
        registry_strategies registry_strategies_;
        abstract_om &om_;
        PS &ps_;
        bool running_;
    };
}

#include "strategy.manager.impl.hpp"
