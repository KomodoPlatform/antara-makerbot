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
#include "orders.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("execution equality")
    {
        antara::pair pair = {{st_symbol{"A"}}, {st_symbol{"B"}}};
        antara::st_price price = st_price{5};
        antara::st_quantity quantity = st_quantity{10};
        maker maker = true;

        orders::execution e1 = { st_execution_id{""}, pair, price, quantity, maker };
        orders::execution e2 = { st_execution_id{""}, pair, price, quantity, maker };

        CHECK_EQ(e1, e2);

        antara::st_price new_price = st_price{10};
        orders::execution e3 = { st_execution_id{""}, pair, new_price, quantity, maker };

        CHECK_NE(e1, e3);
    }

    TEST_CASE ("executions can be created from orders")
    {
        st_order_id id = st_order_id{"ID"};
        antara::pair pair = {{st_symbol{"A"}}, {st_symbol{"B"}}};
        st_price price = st_price{5};
        st_quantity quantity = st_quantity{10};
        st_quantity filled = st_quantity{1};
        orders::order_status status = orders::order_status::live;

        orders::order order = orders::order(
            id, pair, price, quantity, filled, status);

        st_execution_id ex_id = st_execution_id{""};
        st_quantity ex_quantity = st_quantity{10};
        maker maker = true;

        orders::execution actual = order.create_execution(ex_id, ex_quantity, maker);
        orders::execution expected = { ex_id, pair, price, ex_quantity, maker };

        CHECK_EQ(actual, expected);
    }

    TEST_CASE ("execute increases the filled quantity")
    {
        st_order_id id = st_order_id{"ID"};
        antara::pair pair = { {st_symbol{"A"}}, {st_symbol{"B"}} };
        st_price price = st_price{5};
        st_quantity quantity = st_quantity{10};
        st_quantity filled = st_quantity{0};
        orders::order_status status = orders::order_status::live;

        orders::order order = orders::order(id, pair, price, quantity, filled, status);

        st_execution_id ex_id = st_execution_id{""};
        st_quantity ex_quantity = st_quantity{3};
        orders::execution ex = {
            ex_id, pair, price, ex_quantity, true
        };

        CHECK_EQ(st_quantity{0}, order.filled);

        order.execute(ex);

        CHECK_EQ(st_quantity{3}, order.filled);
    }

    TEST_CASE ("order_book can have orders added")
    {
        auto pair = antara::pair::of("A", "B");
        auto cross = pair.to_cross();

        auto price = st_price{10};

        auto ob = orders::order_book(cross);

        auto o_id = st_order_id{"o_id"};
        auto o = orders::order_builder(o_id, pair)
            .price(price)
            .build();

        ob.add_order(o);

        auto& bids = ob.get_bids();
        auto& asks = ob.get_asks();

        CHECK_EQ(1, bids.size());
        CHECK_EQ(0, asks.size());
    }
}
