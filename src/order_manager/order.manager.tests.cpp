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
#include <dex/dex.mock.hpp>
#include <cex/cex.mock.hpp>

#include "order.manager.hpp"

namespace antara::mmbot::tests
{
    using trompeloeil::_;
    using trompeloeil::lt;

    TEST_CASE ("add order to pair map")
    {
        dex_mock dex;
        cex_mock cex;

        auto om = order_manager(dex, cex);

        auto pair = antara::pair::of("A", "B");

        REQUIRE_THROWS(om.get_orders(pair));

        auto o_id = st_order_id{"id"};
        auto b = orders::order_builder(o_id, pair);
        orders::order o = b.build();

        om.add_order_to_pair_map(o);

        auto& new_orders = om.get_orders(pair);
        CHECK_EQ(1, new_orders.size());

        om.remove_order_from_pair_map(o);

        auto& remove_orders = om.get_orders(pair);
        CHECK_EQ(0, remove_orders.size());
    }

    TEST_CASE("add orders")
    {
        dex_mock dex;
        cex_mock cex;

        auto om = order_manager(dex, cex);

        auto pair = antara::pair::of("A", "B");

        auto e1_id = st_execution_id{"e1_id"};
        auto e2_id = st_execution_id{"e2_id"};

        auto o1_id = st_order_id{"o1_id"};
        auto b1 = orders::order_builder(o1_id, pair);
        orders::order o1 = b1.build();
        o1.add_execution_id(e1_id);

        auto o2_id = st_order_id{"o2_id"};
        auto b2 = orders::order_builder(o2_id, pair);
        orders::order o2 = b2.build();
        o2.add_execution_id(e2_id);

        auto e1_quantity = st_quantity{5};
        orders::execution e1 = o1.create_execution(e1_id, e1_quantity, true);

        auto e2_quantity = st_quantity{5};
        orders::execution e2 = o2.create_execution(e2_id, e2_quantity, true);

        auto os = std::vector<orders::order>();
        os.push_back(o1);
        os.push_back(o2);

        // Act
        om.add_orders(os);

        auto &orders = om.get_all_orders();
        auto &o_ids = om.get_orders(pair);

        CHECK_EQ(2, orders.size());
        CHECK_EQ(2, o_ids.size());

        // Act
        om.remove_order(o1);

        CHECK_EQ(1, orders.size());
        CHECK_EQ(1, o_ids.size());

        // Act
        om.remove_order(o2);

        CHECK_EQ(0, orders.size());
        CHECK_EQ(0, o_ids.size());
    }

    TEST_CASE ("on start, the OM loads existing orders")
    {
        auto o_id = st_order_id{"id"};
        auto b = orders::order_builder(o_id, antara::pair::of("A", "B"));
        orders::order o = b.build();

        auto ex_id = st_execution_id{"ex_id"};
        auto ex_quantity = st_quantity{5};
        orders::execution e = o.create_execution(ex_id, ex_quantity, true);

        dex_mock dex;
        cex_mock cex;

        auto om = order_manager(dex, cex);

        auto o_list = std::vector<orders::order>();
        o_list.push_back(o);

        REQUIRE_CALL(dex, get_live_orders())
            .RETURN(o_list);

        auto o_ids = std::unordered_set<st_order_id>();
        o_ids.emplace(o_id);

        auto ex_list = std::vector<orders::execution>();
        ex_list.push_back(e);

        REQUIRE_CALL(dex, get_executions(o_ids))
            .RETURN(ex_list);

        om.start();

        auto orders = om.get_all_orders();
        CHECK_EQ(1, orders.size());

        auto executions = om.get_all_executions();
        CHECK_EQ(1, executions.size());
    }

    TEST_CASE ("the OM can poll the exchange for new orders and executions")
    {
        auto pair = antara::pair::of("A", "B");
        st_price price = st_price(10);
        st_quantity quantity = st_quantity(10);
        antara::side side = antara::side::buy;
        antara::maker maker = true;

        // This order is already on the book
        auto o1_id = st_order_id{"id_1"};
        auto b1 = orders::order_builder(o1_id, pair);
        orders::order o1 = b1.build();

        auto e1_id = st_execution_id{"e_id_1"};
        orders::execution e1 = { e1_id, pair, price, quantity, side, maker };
        o1.add_execution_id(e1_id);



        // This order we will learn about
        auto o2_id = st_order_id{"id_2"};
        auto b2 = orders::order_builder(o2_id, pair);
        orders::order o2 = b2.build();

        auto e2_id = st_execution_id{"e_id_2"};
        orders::execution e2 = { e2_id, pair, price, quantity, side, maker };
        o2.add_execution_id(e2_id);



        // This order is lost
        auto o3_id = st_order_id{"id_3"};
        auto b3 = orders::order_builder(o3_id, pair);
        orders::order o3 = b3.build();

        auto e3_id = st_execution_id{"e_id_3"};
        orders::execution e3 = { e3_id, pair, price, quantity, side, maker };
        o3.add_execution_id(e3_id);



        dex_mock dex;
        cex_mock cex;

        auto om = order_manager(dex, cex);



        // We have one order we already know about
        std::vector<orders::order> existing_orders;
        existing_orders.push_back(o1);
        om.add_orders(existing_orders);

        std::vector<orders::execution> existing_executions;
        existing_executions.push_back(e1);
        om.add_executions(existing_executions);

        REQUIRE_CALL(dex, get_order_status(o1_id))
            .RETURN(o1);

        // We have 2 present on the book
        std::vector<orders::order> live_orders;
        live_orders.push_back(o1);
        live_orders.push_back(o2);
        REQUIRE_CALL(dex, get_live_orders())
            .RETURN(live_orders);

        // Each order has one execution
        std::unordered_set<st_order_id> live_order_ids;
        live_order_ids.emplace(o1_id);
        live_order_ids.emplace(o2_id);

        std::vector<orders::execution> live_executions;
        live_executions.push_back(e1);
        live_executions.push_back(e2);

        REQUIRE_CALL(dex, get_executions(live_order_ids))
            .RETURN(live_executions);

        std::vector<orders::execution> recent;
        recent.push_back(e1);
        recent.push_back(e2);
        recent.push_back(e3);
        // And there is an execution for an order we did'nt know
        REQUIRE_CALL(dex, get_recent_executions())
            .RETURN(recent);

        // And there is an execution for an order we did'nt know
        REQUIRE_CALL(cex, mirror(e2));

        REQUIRE_CALL(cex, mirror(e3));

        om.poll();


    }

    TEST_CASE ("orders can be cancelled by pair")
    {
        auto pair = antara::pair::of("A", "B");

        orders::order_level ol = {st_price(10), st_quantity(10), antara::side::sell};

        auto o_id = st_order_id{"id"};
        auto b = orders::order_builder(o_id, pair);
        orders::order o = b.build();

        dex_mock dex;
        cex_mock cex;

        auto om = order_manager(dex, cex);

        ALLOW_CALL(dex, place(ol, pair))
            .LR_RETURN(std::ref(o));

        REQUIRE_CALL(dex, cancel(o_id))
            .RETURN(true);

        om.place_order(ol, pair);
        CHECK_EQ(1, om.get_all_orders().size());

        auto ids = om.cancel_orders(pair);

        CHECK_EQ(1, ids.size());
        CHECK_EQ(1, ids.count(o_id));

        CHECK_EQ(0, om.get_all_orders().size());
    }
}
