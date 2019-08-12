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

    class strategy_manager
    {
    public:
        using registry_strategies = std::unordered_map<antara::pair, market_making_strategy>;
        strategy_manager() = default;

        void add_strategy(const antara::pair& pair, const market_making_strategy& strat);

        void add_strategy(const market_making_strategy& strat);

        [[nodiscard]] const market_making_strategy &get_strategy(const antara::pair &pair) const;

        [[nodiscard]] const registry_strategies &get_strategies() const;

        static orders::order_level make_bid(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity);

        static orders::order_level make_ask(
            antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity);

        static orders::order_group create_order_group(
            antara::pair pair, const market_making_strategy &strat, antara::st_price mid);

    private:

        registry_strategies registry_strategies_;
    };

}

