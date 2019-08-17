
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
#include "http/http.endpoints.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("test launch mm2 in background")
    {
        auto cfg = load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        antara::mmbot::mm2_client mm2(cfg);

        nlohmann::json json_data = {{"method",   "version"},
                                    {"userpass", cfg.mm2_rpc_password}};
        auto resp = RestClient::post(antara::mmbot::mm2_endpoint, "application/json", json_data.dump());
        CHECK_EQ(200, resp.code);
        DVLOG_F(loguru::Verbosity_INFO, "body: %s", resp.body.c_str());
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

            //! Non enable coin
            mm2::balance_request bad_request({antara::asset{st_symbol{"BTC"}}});
            answer = mm2.rpc_balance(std::move(bad_request));
            CHECK_EQ(500, answer.rpc_result_code);
        }
    }

    TEST_CASE ("mm2 rpc electrum")
    {
        auto cfg = load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        antara::mmbot::mm2_client mm2(cfg, false);
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