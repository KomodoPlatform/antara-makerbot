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
#include "fake.cex.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("an execution cb can be registered for executions")
    {
        auto id = st_order_id{"ID"};
        orders::order_builder b = orders::order_builder(id, antara::pair::of("A", "B"));
        orders::order o = b.build();

        auto cex = fake_cex();
        cex.place_order(o);
    }

    TEST_CASE ("We can add an order to the cex and it is put in the book")
    {
        // Order
        antara::pair pair = antara::pair::of("A", "B");

        auto o_id = st_order_id{"o_id"};
        auto b = orders::order_builder(o_id, pair);
        orders::order o = b.build();

        // Order book
        auto book = orders::order_book(pair);

        // CEX
        auto cex = fake_cex();

        cex.add_book(book);
        cex.place_order(o);


    }
}
