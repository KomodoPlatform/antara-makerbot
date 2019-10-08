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
#include "dex.utils.hpp"
#include <iostream>

namespace antara::mmbot::tests
{
    TEST_CASE ("trade result to order")
    {
        auto id = "o_id";

        auto action = "";
        auto base = asset{st_symbol{"A"}};
        auto rel = asset{st_symbol{"BTC"}};
        auto base_amount = "3";
        auto rel_amount = "6";
        auto method = "";
        auto dest_pub_key = "";
        auto sender_pub_key = "";
        auto uuid = id;

        auto result = mm2::trade_result{
            action,
            base, rel,
            base_amount, rel_amount,
            method,
            dest_pub_key, sender_pub_key,
            uuid
        };

        auto pair = antara::pair::of("A", "BTC");
        auto symbol = st_symbol{"BTC"};

        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto &cfg = get_mmbot_config();

        auto price = generate_st_price_from_api_price(cfg, symbol, "2");

        auto b = orders::order_builder(id, pair);
        b.price(price);
        b.quantity(st_quantity{3});
        auto expected = b.build();

        auto actual = to_order(result);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE ("mm::taker_order can be converted to orders::order")
    {
        auto id = "o_id";

        auto base = asset{st_symbol{"A"}};
        auto rel = asset{st_symbol{"BTC"}};
        auto base_amount = "3";
        auto rel_amount = "6";
        auto uuid = id;

        auto result = mm2::taker_order{
            base, rel,
            base_amount, rel_amount,
            uuid
        };

        auto pair = antara::pair::of("A", "BTC");
        auto symbol = st_symbol{"BTC"};

        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto &cfg = get_mmbot_config();

        auto price = generate_st_price_from_api_price(cfg, symbol, "2");

        auto b = orders::order_builder(id, pair);
        b.price(price);
        b.quantity(st_quantity{3});
        auto expected = b.build();

        auto actual = to_order(result);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE ("mm::order can be converted to orders::order")
    {
        auto uuid = "o_id";
        auto base = "A";
        auto rel = "BTC";
        auto base_amount = "10";
        auto price_str = "5";

        auto mm_order = mm2::order{base, rel, uuid, base_amount, price_str};

        auto b = orders::order_builder(st_order_id{uuid}, antara::pair::of(base, rel));
        b.quantity(st_quantity{10});

        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto &cfg = get_mmbot_config();
        auto price = generate_st_price_from_api_price(cfg, st_symbol{rel}, price_str);
        b.price(price);

        auto expected = b.build();

        auto actual = to_order(mm_order);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE ("can create cancel order requests")
    {
        auto expected = mm2::cancel_order_request{"o_id"};

        auto o_id = st_order_id{"o_id"};
        auto actual = out(o_id);

        CHECK_EQ(expected.uuid, actual.uuid);
    }

    TEST_CASE ("can parse successful cancel order answers")
    {
        auto expected = true;

        auto answer = mm2::cancel_order_answer{"success"};
        auto actual = in(answer);

        CHECK_EQ(expected, actual);
    }

    TEST_CASE ("can parse failed cancel order answers")
    {
        auto expected = false;

        auto answer = mm2::cancel_order_answer{"failed"};
        auto actual = in(answer);

        CHECK_EQ(expected, actual);
    }

    // TEST_CASE ("can convert a mm2::swap into an execution")
    // {
    //     auto uuid = "e_id";
    //     auto id = st_execution_id{uuid};

    //     auto maker_coin = "ETH";
    //     auto taker_coin = "BTC";

    //     auto pair = antara::pair::of(maker_coin, taker_coin);

    //     auto maker_amount = "10";
    //     auto taker_amount = "20";

    //     auto quantity = st_quantity{10};
    //     auto price = st_price{2};

    //     auto maker = true;

    //     auto expected = orders::execution{ id, pair, price, quantity, maker};

    //     auto e_data = mm2::event_data{ uuid, maker_coin, taker_coin, maker_amount, taker_amount };
    //     auto e_type = "Started";

    //     auto event = mm2::event{e_data, e_type};

    //     auto event_ts = mm2::event_ts{event, 100000};

    //     auto events = std::vector<mm2::event_ts>();
    //     events.push_back(event_ts);

    //     auto err_events = std::vector<std::string>();
    //     auto type = mm2::swap_type::maker;
    //     auto swap = mm2::swap{err_events, events, type};

    //     auto actual = to_execution(swap);

    //     CHECK_EQ(expected, actual);
    // }
}
