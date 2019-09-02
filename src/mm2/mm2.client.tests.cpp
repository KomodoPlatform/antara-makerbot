
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
#include <restclient-cpp/restclient.h>
#include "mm2.client.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("test launch mm2 in background")
    {
        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        antara::mmbot::mm2_client mm2;
        SUBCASE("mm2 rpc orderbooks")
        {
            //! Good pair
            mm2::orderbook_request request({antara::pair::of("RICK", "MORTY")});
            auto answer = mm2.rpc_orderbook(std::move(request));
            CHECK_EQ(200, answer.rpc_result_code);

            //! wrong rel
            mm2::orderbook_request bad_request({antara::pair::of("KMDD", "MORTY")});
            answer = mm2.rpc_orderbook(std::move(bad_request));
            CHECK_EQ(500, answer.rpc_result_code);

            //! wrong base
            mm2::orderbook_request another_bad_request({antara::pair::of("MORTY", "KMDD")});
            answer = mm2.rpc_orderbook(std::move(another_bad_request));
            CHECK_EQ(500, answer.rpc_result_code);
        }

        SUBCASE("mm2 rpc my_balance")
        {
            mm2::balance_request request({antara::asset{st_symbol{"RICK"}}});
            auto answer = mm2.rpc_balance(std::move(request));
            CHECK_EQ(200, answer.rpc_result_code);

            if (auto force_passphrase = std::getenv("FORCE_MM2_PASSPHRASE"); force_passphrase != nullptr) {
                CHECK_NE("0", answer.balance);
            }
            //! Non enable coin
            mm2::balance_request bad_request({antara::asset{st_symbol{"BTC"}}});
            answer = mm2.rpc_balance(std::move(bad_request));
            CHECK_EQ(500, answer.rpc_result_code);
        }

        SUBCASE("mm2 rpc version")
        {
            auto answer = mm2.rpc_version();
            CHECK_EQ(200, answer.rpc_result_code);
        }

        SUBCASE ("mm2 rpc setprice") {
            if (auto force_passphrase = std::getenv("FORCE_MM2_PASSPHRASE"); force_passphrase != nullptr) {
                mm2::setprice_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
                auto answer = mm2.rpc_setprice(std::move(request));
                CHECK_EQ(200, answer.rpc_result_code);
                mm2::cancel_order_request cancel_request{answer.result_setprice.uuid};
                auto cancel_answer = mm2.rpc_cancel_order(std::move(cancel_request));
                CHECK_EQ(200, cancel_answer.rpc_result_code);
            }
        }

        SUBCASE ("mm2 rpc buy") {
            if (auto force_passphrase = std::getenv("FORCE_MM2_PASSPHRASE"); force_passphrase != nullptr) {
                mm2::buy_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
                auto answer = mm2.rpc_buy(std::move(request));
                REQUIRE_EQ(200, answer.rpc_result_code);

                mm2::cancel_order_request cancel_request{answer.result_trade.value().uuid};
                auto cancel_answer = mm2.rpc_cancel_order(std::move(cancel_request));
                CHECK_EQ(200, cancel_answer.rpc_result_code);
            }
        }


        SUBCASE ("mm2 cancel_all_request by ALL") {
            if (auto force_passphrase = std::getenv("FORCE_MM2_PASSPHRASE"); force_passphrase != nullptr) {
                mm2::buy_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
                auto answer = mm2.rpc_buy(std::move(request));
                REQUIRE_EQ(200, answer.rpc_result_code);

                mm2::cancel_all_orders_request cancel_request{"All"};
                auto cancel_answer = mm2.rpc_cancel_all_orders(std::move(cancel_request));
                CHECK_EQ(200, cancel_answer.rpc_result_code);
            }
        }

        SUBCASE ("mm2 cancel_all_request by PAIR") {
            if (auto force_passphrase = std::getenv("FORCE_MM2_PASSPHRASE"); force_passphrase != nullptr) {
                mm2::buy_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
                auto answer = mm2.rpc_buy(std::move(request));
                REQUIRE_EQ(200, answer.rpc_result_code);

                auto pair_asset = antara::pair::of("MORTY", "RICK");
                mm2::cancel_all_orders_request cancel_request{"Pair", mm2::cancel_all_orders_data{pair_asset.base, pair_asset.quote}};
                auto cancel_answer = mm2.rpc_cancel_all_orders(std::move(cancel_request));
                CHECK_EQ(200, cancel_answer.rpc_result_code);
            }
        }
    }

    TEST_CASE ("mm2 rpc electrum")
    {
        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        antara::mmbot::mm2_client mm2(false);
        mm2::electrum_request request{"MORTY", {{"electrum2.cipig.net:10018"}, {"electrum1.cipig.net:10018"},
                                                {"electrum3.cipig.net:10018"}}};
        auto answer = mm2.rpc_electrum(std::move(request));
        CHECK_EQ(200, answer.rpc_result_code);


        mm2::electrum_request bad_request{"KMDD", {{"electrum2.cipig.net:10018"}, {"electrum1.cipig.net:10018"},
                                                   {"electrum3.cipig.net:10018"}}};
        answer = mm2.rpc_electrum(std::move(bad_request));
        CHECK_EQ(500, answer.rpc_result_code);
    }
}
