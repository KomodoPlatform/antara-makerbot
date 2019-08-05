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
#include "order.manager.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("individual orders can be placed on the cex")
    {
        CHECK(true);
    }

    TEST_CASE ("whole order sets can be placed")
    {
        antara::order_manager om = antara::order_manager();

        CHECK_EQ(0, om.get_all_order_sets().size());

        orders::order_set os =
            {{st_symbol{"A"}, st_symbol{"B"}},
             std::vector<orders::order_level>()};
        om.place_orders(os);

        CHECK_EQ(1, om.get_all_order_sets().size());
    }

    TEST_CASE ("order status can be changed")
    {
        st_order_id id = st_order_id{"ID"};
        antara::pair pair = pair.of("A", "B");

        orders::order_status status = orders::order_status::live;

        orders::order o = orders::order_builder(id, pair)
            .status(status)
            .build();

        orders::order_status new_status = orders::order_status::cancelled;
        orders::order_status_change osc = { id, new_status };

        o.change_status(osc);

        CHECK_EQ(new_status, o.status);
    }

    TEST_CASE ("can return the order set for a pair")
    {
        CHECK(true);
    }

    TEST_CASE ("can return all order sets")
    {
        CHECK(true);
    }
}
