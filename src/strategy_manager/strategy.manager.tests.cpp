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

#include <doctest/doctest.h>

#include <utils/mmbot_strong_types.hpp>
#include "strategy.manager.hpp"

namespace antara::mmbot::tests
{

    TEST_CASE ("asset equality")
    {
        antara::asset a1 = {antara::st_symbol{"A"}};
        antara::asset a2 = {antara::st_symbol{"A"}};
        antara::asset a3 = {antara::st_symbol{"B"}};

        CHECK_EQ(a1, a2);
        CHECK_NE(a1, a3);
    }

    TEST_CASE ("strats can be added and retreived")
    {
        auto sm = strategy_manager();

        antara::pair pair = {{st_symbol{"A"}},
                             {st_symbol{"B"}}};
        st_spread spread = st_spread{0.1};
        st_quantity quantity = st_quantity{5};
        antara::side side = antara::side::sell;

        auto strat = market_making_strategy{pair, spread, quantity, side};
        CHECK_EQ(0, sm.get_strategies().size());

        sm.add_strategy(strat);
        CHECK_EQ(1, sm.get_strategies().size());

        const auto& other = sm.get_strategy(pair);
        CHECK_EQ(strat, other);

    }

    TEST_CASE ("bids can be made")
    {
        auto mid = antara::st_price{10};
        auto spread = antara::st_spread{0.1};
        auto quantity = antara::st_quantity{10.0};
        auto bid_price = antara::st_price{9};

        auto sm = strategy_manager();

        auto expected = orders::order_level{bid_price, quantity, antara::side::buy};
        auto actual = strategy_manager::make_bid(mid, spread, quantity);

        //CHECK_EQ(expected, actual);
    }

    TEST_CASE ("asks can be made")
    {
        antara::st_price mid = antara::st_price{10};
        antara::st_spread spread = antara::st_spread{0.1};
        antara::st_quantity quantity = antara::st_quantity{10.0};

        antara::st_price ask_price = antara::st_price{11};

        strategy_manager sm = strategy_manager();

        auto expected = orders::order_level{ask_price, quantity, antara::side::sell};
        auto actual = strategy_manager::make_ask(mid, spread, quantity);

        //CHECK_EQ(expected, actual);
    }

}
