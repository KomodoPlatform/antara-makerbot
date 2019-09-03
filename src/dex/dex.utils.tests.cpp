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

        auto side = antara::side::sell;
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
            side, action,
            base, rel,
            base_amount, rel_amount,
            method,
            dest_pub_key, sender_pub_key,
            uuid
        };

        auto pair = antara::pair::of("A", "BTC");
        auto symbol = st_symbol{"BTC"};

        std::cout << "Okay so far" << std::endl;

        additional_coin_info coin_info;
        coin_info.nb_decimals = 5;

        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto &cfg = get_mmbot_config();

        auto price = generate_st_price_from_api_price(cfg, symbol, "2.0");

        auto b = orders::order_builder(id, pair);
        b.price(price);
        b.quantity(st_quantity{3});
        b.side(side);
        auto expected = b.build();

        auto actual = to_order(result);

        std::cout << expected.price.value() << std::endl;
        std::cout << actual.price.value() << std::endl;
        CHECK_EQ(expected, actual);
    }
}
