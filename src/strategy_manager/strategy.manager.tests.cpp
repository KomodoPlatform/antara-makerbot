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
#include <doctest/trompeloeil.hpp>
#include <trompeloeil.hpp>

#include <utils/mmbot_strong_types.hpp>
#include <order_manager/order.manager.mock.hpp>
#include <price/service.price.platform.mock.hpp>
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

    TEST_CASE ("market_making_strategy equality")
    {
        auto pair = antara::pair::of("A", "B");
        auto spread = st_spread{0.1};

        auto q_10 = st_quantity{10};
        auto q_11 = st_quantity{11};

        auto mms_1 = market_making_strategy{pair, spread, q_10, true};
        auto mms_2 = market_making_strategy{pair, spread, q_11, true};

        CHECK_EQ(mms_1, mms_1);
        CHECK_NE(mms_1, mms_2);
    }

    TEST_CASE ("strats can be added and retreived")
    {
        auto om = order_manager_mock();
        auto ps = price_service_platform_mock();
        auto sm = strategy_manager<price_service_platform_mock>(ps, om);

        auto pair = antara::pair::of("A", "B");
        st_spread spread = st_spread{0.1};
        st_quantity quantity = st_quantity{5};

        auto strat = market_making_strategy{pair, spread, quantity, true};
        CHECK_EQ(0, sm.get_strategies().size());

        sm.add_strategy(strat);
        CHECK_EQ(1, sm.get_strategies().size());

        const auto& other = sm.get_strategy(pair.to_cross());
        CHECK_EQ(strat, other);

        auto new_q = st_quantity{10};
        auto new_strat = market_making_strategy{pair, spread, new_q, true};

        // update
        sm.add_strategy(new_strat);
        CHECK_EQ(new_strat, sm.get_strategy(pair.to_cross()));
    }

    TEST_CASE ("bids can be made")
    {
        auto mid = antara::st_price{10};
        auto spread = antara::st_spread{0.1};
        auto quantity = antara::st_quantity{10.0};

        auto bid_price = antara::st_price{9};

        auto pair = antara::pair::of("A", "B");

        auto om = order_manager_mock();
        auto ps = price_service_platform_mock();
        auto sm = strategy_manager<price_service_platform_mock>(ps, om);

        auto expected = orders::order_level{bid_price, quantity, pair};
        auto actual = sm.make_bid(mid, spread, quantity, pair);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE ("asks can be made")
    {
        antara::st_price mid = antara::st_price{10};
        antara::st_spread spread = antara::st_spread{0.1};
        antara::st_quantity quantity = antara::st_quantity{10.0};

        antara::st_price ask_price = antara::st_price{11};

        auto pair = antara::pair::of("A", "B");

        auto om = order_manager_mock();
        auto ps = price_service_platform_mock();
        auto sm = strategy_manager<price_service_platform_mock>(ps, om);

        auto expected = orders::order_level{ask_price, quantity, pair, true};
        auto actual = sm.make_ask(mid, spread, quantity, pair);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE("orders are refreshed by cancelling them and placing new")
    {
        auto pair = antara::pair::of("A", "B");
        market_making_strategy strat
            = {pair, st_spread{0.1}, st_quantity{10}, true};

        auto om = order_manager_mock();
        auto ps = price_service_platform_mock();

        auto sm = strategy_manager<price_service_platform_mock>(ps, om);

        sm.add_strategy(strat);

        REQUIRE_CALL(om, cancel_orders(pair.to_cross()))
            .RETURN(std::unordered_set<st_order_id>());

        auto mid = st_price{1};
        auto og = sm.create_order_group(strat, mid);

        auto o_id = st_order_id{"o_id"};
        auto new_orders = std::unordered_set<st_order_id>();
        new_orders.emplace(o_id);
        REQUIRE_CALL(om, place_order(og))
            .RETURN(new_orders);

        REQUIRE_CALL(ps, get_price(pair))
            .RETURN(mid);

        sm.refresh_orders(pair.to_cross());
    }
}
