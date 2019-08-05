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
        antara::side side = antara::side::buy;
        antara::st_maker maker = st_maker{true};

        antara::orders::execution e1 = {pair, price, quantity, side, maker};
        antara::orders::execution e2 = {pair, price, quantity, side, maker};

        CHECK_EQ(e1, e2);

        antara::st_price new_price = st_price{10};
        antara::orders::execution e3 = {pair, new_price, quantity, side, maker};

        CHECK_NE(e1, e3);
    }

    TEST_CASE ("executions can be created from orders")
    {
        st_order_id id = st_order_id{"ID"};
        antara::pair pair = {{st_symbol{"A"}}, {st_symbol{"B"}}};
        st_price price = st_price{5};
        st_quantity quantity = st_quantity{10};
        st_quantity filled = st_quantity{1};
        antara::side side = antara::side::buy;
        antara::orders::order_status status = antara::orders::order_status::live;

        antara::orders::order order = antara::orders::order(
            id, pair, price, quantity, filled, side, status);

        st_quantity ex_quantity = st_quantity{10};
        st_maker maker = st_maker{true};

        antara::orders::execution actual = order.create_execution(ex_quantity, maker);
        antara::orders::execution expected = { pair, price, ex_quantity, side, maker };

        CHECK_EQ(actual, expected);
    }

    TEST_CASE ("execute increases the filled quantity")
    {
        st_order_id id = st_order_id{"ID"};
        antara::pair pair = {{st_symbol{"A"}}, {st_symbol{"B"}}};
        st_price price = st_price{5};
        st_quantity quantity = st_quantity{10};
        st_quantity filled = st_quantity{0};
        antara::side side = antara::side::buy;
        orders::order_status status = orders::order_status::live;

        orders::order order = orders::order(id, pair, price, quantity, filled, side, status);

        st_quantity ex_quantity = st_quantity{3};
        orders::execution ex = {
            pair, price, ex_quantity, side, st_maker{true}
        };

        CHECK_EQ(st_quantity{0}, order.filled);

        order.execute(ex);

        CHECK_EQ(st_quantity{3}, order.filled);
    }
}
