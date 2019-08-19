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
    // TEST_CASE ("an execution cb can be registered for executions")
    // {
    //     auto id = st_order_id{"ID"};
    //     orders::order_builder b = orders::order_builder(id, antara::pair::of("A", "B"));
    //     orders::order o = b.build();

    //     auto cex = fake_cex();
    //     cex.place_order(o);
    // }

    // TEST_CASE ("We can add an order to the cex and it is put in the book")
    // {
    //     // Order
    //     antara::pair pair = antara::pair::of("A", "B");
    //     auto side = antara::side::buy;
    //     auto price = st_price{10};

    //     auto o_id = st_order_id{"o_id"};
    //     auto b = orders::order_builder(o_id, pair);
    //     b.side(side);
    //     b.price(price);
    //     orders::order o = b.build();

    //     // Order book
    //     auto book = orders::order_book(pair);



    //     // auto books = std::unordered_map<antara::pair, orders::order_book>();
    //     // books.emplace(pair, book);
    //     // auto book5 = books.at(pair);
    //     // book.add_order(o);
    //     // auto book6 = books.at(pair);

    //     // CHECK_EQ(0, book.get_bids().size());
    //     // CHECK_EQ(0, book5.get_bids().size());
    //     // CHECK_EQ(0, book6.get_bids().size());



    //     // CEX
    //     auto cex = fake_cex();

    //     cex.add_book(book);
    //     cex.place_order(o);

    //     CHECK_EQ(antara::side::buy, antara::buy);

    //     auto asks = book.get_asks();
    //     CHECK_EQ(0, asks.size());

    //     auto bids = book.get_bids();
    //     CHECK_EQ(1, bids.size());

    //     if (bids.find(price) != bids.end()) {
    //         auto orders = bids.at(price);
    //         CHECK_EQ(1, orders.size());
    //     } else {
    //         CHECK(false);
    //     }

    //     auto book2 = cex.get_book(pair);
    //     auto asks2 = book2.get_asks();
    //     auto bids2 = book2.get_bids();

    //     CHECK_EQ(0, asks2.size());
    //     CHECK_EQ(1, bids2.size());

    // }

    TEST_CASE ("foo")
    {
        auto pair = antara::pair::of("A", "B");
        auto price = st_price{10};

        auto o_id = st_order_id{"o_id"};
        auto b = orders::order_builder(o_id, pair)
            .side(antara::buy)
            .price(price);
        auto o = b.build();

        auto o2_id = st_order_id{"o2_id"};
        auto b2 = orders::order_builder(o2_id, pair)
            .side(antara::buy)
            .price(price);
        auto o2 = b2.build();

        auto book = orders::order_book(pair);
        book.add_order(o);

        auto cex = fake_cex();
        cex.add_book(book);

        cex.place_order(o2);

        auto &book2 = cex.get_book(pair);
        auto bids = book2.get_bids();
        // There is one price level
        CHECK_EQ(1, bids.size());

        auto orders = bids.at(price);
        // And there are two orders at this price
        CHECK_EQ(2, orders.size());
    }
}
