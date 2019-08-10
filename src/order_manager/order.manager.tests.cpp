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
#include "order.manager.hpp"

namespace antara::mmbot::tests
{
    class dex_mock
    {
    public:
        MAKE_MOCK0(get_live_orders, std::vector<orders::order>());
        MAKE_MOCK1(get_executions, std::vector<orders::execution>(const std::vector<st_order_id>&));
    };

    TEST_CASE ("foo")
    {
        auto o_id = st_order_id{"id"};
        auto b = orders::order_builder(o_id, antara::pair::of("A", "B"));
        orders::order o = b.build();

        auto ex_id = st_execution_id{"ex_id"};
        auto ex_quantity = st_quantity{5};
        orders::execution e = o.create_execution(ex_id, ex_quantity, true);

        dex_mock dex;
        antara::cex cex = antara::cex();

        auto om = order_manager<dex_mock>(dex, cex);

        auto o_list = std::vector<orders::order>();
        o_list.push_back(o);

        REQUIRE_CALL(dex, get_live_orders())
            .RETURN(o_list);

        auto o_ids = std::vector<st_order_id>();
        o_ids.push_back(o_id);

        auto ex_list = std::vector<orders::execution>();
        ex_list.push_back(e);

        REQUIRE_CALL(dex, get_executions(o_ids))
            .RETURN(ex_list);

        om.start();


        auto orders = om.get_all_orders();

        CHECK_EQ(1, orders.size());
    }


    // TEST_CASE ("individual orders can be placed on the cex")
    // {
    //     CHECK(true);
    // }

    // TEST_CASE ("whole order sets can be placed")
    // {
    //     antara::order_manager om = antara::order_manager();

    //     CHECK_EQ(0, om.get_all_order_groups().size());

    //     orders::order_group os =
    //         {{st_symbol{"A"}, st_symbol{"B"}},
    //          std::vector<orders::order_level>()};
    //     om.place_orders(os);

    //     CHECK_EQ(1, om.get_all_order_groups().size());
    // }

    // TEST_CASE ("order status can be changed")
    // {
    //     st_order_id id = st_order_id{"ID"};
    //     antara::pair pair = pair.of("A", "B");

    //     orders::order_status status = orders::order_status::live;

    //     orders::order o = orders::order_builder(id, pair)
    //         .status(status)
    //         .build();

    //     orders::order_status new_status = orders::order_status::cancelled;
    //     orders::order_status_change osc = { id, new_status };

    //     o.change_status(osc);

    //     CHECK_EQ(new_status, o.status);
    // }

    // TEST_CASE ("can return the order set for a pair")
    // {
    //     CHECK(true);
    // }

    // TEST_CASE ("can return all order sets")
    // {
    //     CHECK(true);
    // }
}
