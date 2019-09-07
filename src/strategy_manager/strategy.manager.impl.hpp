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

#include "strategy.manager.hpp"

namespace antara::mmbot
{
    bool market_making_strategy::operator==(const market_making_strategy &other) const
    {
        return pair == other.pair
               && spread == other.spread
               && quantity == other.quantity;
    }

    bool market_making_strategy::operator!=(const market_making_strategy &other) const
    {
        return !(*this == other);
    }

    template <class PS>
    void strategy_manager<PS>::add_strategy(const market_making_strategy &strat)
    {
        antara::cross cross = strat.pair.to_cross();
        registry_strategies_.emplace(cross, strat);
    }

    template <class PS>
    const market_making_strategy &strategy_manager<PS>::get_strategy(antara::cross cross) const
    {
        return registry_strategies_.at(cross);
    }

    template <class PS>
    const typename strategy_manager<PS>::registry_strategies &strategy_manager<PS>::get_strategies() const
    {
        return registry_strategies_;
    }

    template <class PS>
    orders::order_level strategy_manager<PS>::make_bid(
        antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity, antara::pair pair)
    {
        antara::st_spread mod = antara::st_spread{1.0} - spread;
        antara::st_price price = mid * mod;
        orders::order_level ol{price, quantity, pair};
        return ol;
    }

    template <class PS>
    orders::order_level strategy_manager<PS>::make_ask(
        antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity, antara::pair pair)
    {
        antara::st_spread mod = 1.0 + spread;
        antara::st_price price = mid * mod;
        orders::order_level ol{price, quantity, pair, true};
        return ol;
    }

    template <class PS>
    orders::order_group strategy_manager<PS>::create_order_group(
            const market_making_strategy &strat, antara::st_price mid)
    {
        auto pair = strat.pair;
        auto spread = strat.spread;
        auto quantity = strat.quantity;

        std::vector<orders::order_level> levels;
        auto level = make_bid(mid, spread, quantity, pair);
        levels.push_back(level);

        if (strat.both) {
            orders::order_level other_level = make_ask(mid, spread, quantity, pair);
            levels.push_back(other_level);
        }

        auto os = orders::order_group{pair.to_cross(), levels};
        return os;
    }

    template <class PS>
    orders::order_group strategy_manager<PS>::create_order_group(const market_making_strategy &strat)
    {
        auto mid = ps_.get_price(strat.pair);
        return create_order_group(strat, mid);
    }

    template <class PS>
    void strategy_manager<PS>::refresh_orders(antara::cross pair)
    {
        auto strat = registry_strategies_.at(pair);
        auto orders = create_order_group(strat);

        om_.cancel_orders(pair);
        om_.place_order(orders);
    }

    template <class PS>
    void strategy_manager<PS>::refresh_all_orders()
    {
        for(const auto& [pair, strat] : registry_strategies_) {
            refresh_orders(pair);
        }
    }

    // Call refresh_all_orders on a loop
    template <class PS>
    void strategy_manager<PS>::start()
    {
        // TODO
        // For the time being this loops with a sleep
        // But could run in response to price changes in the future
        while(running_) {
            // if there is latency in this function call
            // then we should run this on a new thread for each pair
            refresh_all_orders();
            // std::this_thread::sleep_for(1s);
        }
    }

    template class strategy_manager<price_service_platform>;
    template class strategy_manager<price_service_platform_mock>;
}
