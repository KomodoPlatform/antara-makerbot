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

namespace antara::mmbot
{
    struct market_making_strategy
    {
        antara::pair pair;
        antara::st_spread spread;
        antara::st_quantity quantity;
        antara::side side;
        bool operator==(const market_making_strategy &other) const;
    };

    class abstract_sm
    {
    public:
        using registry_strategies = std::unordered_map<antara::pair, market_making_strategy>;

        virtual ~abstract_sm() = default;

        virtual void add_strategy(const antara::pair& pair, const market_making_strategy& strat) = 0;
        virtual void add_strategy(const market_making_strategy& strat) = 0;

        virtual [[nodiscard]] const market_making_strategy &get_strategy(const antara::pair &pair) const = 0;
        virtual [[nodiscard]] const registry_strategies &get_strategies() const = 0;

        virtual orders::order_level make_bid(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) = 0;

        virtual orders::order_level make_ask(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) = 0;

        virtual orders::order_group create_order_group(
            antara::pair pair, const market_making_strategy &strat, antara::st_price mid) = 0;
    };

    class strategy_manager : public abstract_sm
    {
    public:
        strategy_manager(order_manager& om, price_service_platform& ps): om_(om), ps_(ps)
        {
            running_ = true;
        }

        void add_strategy(const antara::pair& pair, const market_making_strategy& strat) override;
        void add_strategy(const market_making_strategy& strat) override;

        [[nodiscard]] const market_making_strategy &get_strategy(const antara::pair &pair) const override;
        [[nodiscard]] const registry_strategies &get_strategies() const override;

        orders::order_level make_bid(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) override;

        orders::order_level make_ask(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) override;

        orders::order_group create_order_group(
            antara::pair pair, const market_making_strategy &strat, antara::st_price mid) override;

        orders::order_group create_order_group(antara::pair pair, const market_making_strategy &strat) override;

    private:
        registry_strategies registry_strategies_;
        order_manager &om_;
        price_service_platform &ps_;
        bool running_;
    };
}
