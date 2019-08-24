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

#include <csignal>
#include <thread>
#include <doctest/doctest.h>
#include <restclient-cpp/restclient.h>
#include "config/config.hpp"
#include "http.server.hpp"

namespace antara::mmbot::tests
{
    class http_server_tests_fixture
    {
    public:
        http_server_tests_fixture()
        {
            server_thread_ = std::thread([this](){this->server_.run();});
        }

        ~http_server_tests_fixture()
        {
            server_thread_.join();
        }

    private:
        struct tmp_magic
                {
                    tmp_magic() {
                        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
                    }
                };

        tmp_magic magic;
        price_service_platform price_service_;
        mm2_client mm2_client_;
        antara::mmbot::http_server server_{price_service_, mm2_client_};
        std::thread server_thread_;
    };

    using namespace std::chrono_literals;
    TEST_CASE_FIXTURE (http_server_tests_fixture, "test run http_server")
    {
        std::this_thread::sleep_for(1s);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test welcome http_server")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/");
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test getprice")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/api/v1/getprice");
        CHECK_EQ(resp.code, 400); //! Bad request
        resp = RestClient::get("localhost:7777/api/v1/getprice?wrong_option=0&wrong_option2=1");
        CHECK_EQ(resp.code, 422); //! Unprocessable entity
        resp = RestClient::get("localhost:7777/api/v1/getprice?base_currency=KMD&quote_currency=BTC"); //Well formed
        CHECK_EQ(resp.code, 200);
        resp = RestClient::get("localhost:7777/api/v1/getprice?base_currency=DOGE&quote_currency=BTC"); //Well formed
        CHECK_EQ(resp.code, 200);
        resp = RestClient::get("localhost:7777/api/v1/getprice?base_currency=KMDD&quote_currency=BTC"); //Wrong base_currency throw pair not available (internal error)
        CHECK_EQ(resp.code, 500);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test mm2 get orderbook")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/getorderbook");
        CHECK_EQ(resp.code, 400); //! Bad request
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/getorderbook?wrong_option=0&wrong_option2=1");
        CHECK_EQ(resp.code, 422); //! Unprocessable entity
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/getorderbook?base_currency=RICK&quote_currency=MORTY"); //Well formed
        CHECK_EQ(resp.code, 200);
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/getorderbook?base_currency=BTC&quote_currency=MORTY"); //Well formed but BTC not enabled.
        CHECK_EQ(resp.code, 500);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test mm2 my balance")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/my_balance");
        CHECK_EQ(resp.code, 400); //! Bad request
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/my_balance?wrong_option=0");
        CHECK_EQ(resp.code, 422); //! Unprocessable entity
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/my_balance?currency=RICK"); //Well formed
        CHECK_EQ(resp.code, 200);
        resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/my_balance?currency=BTC"); //Well formed but BTC not enabled.
        CHECK_EQ(resp.code, 500);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test mm2 version")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/api/v1/legacy/mm2/version"); //Well formed
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test get all prices")
    {
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:7777/api/v1/getallprice"); //Well formed
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }

    TEST_CASE_FIXTURE(http_server_tests_fixture, "test mm2 setprice")
    {
        std::this_thread::sleep_for(1s);
        mm2::setprice_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
        nlohmann::json json_request;
        mm2::to_json(json_request, request);
        auto resp = RestClient::post("localhost:7777/api/v1/legacy/mm2/setprice", "application/json", json_request.dump());
        CHECK_EQ(resp.code, 200);
        nlohmann::json resp_answer = nlohmann::json::parse(resp.body);
        mm2::setprice_answer answer;
        mm2::from_json(resp_answer, answer);
        mm2::cancel_order_request cancel_request{answer.result_setprice.uuid};
        json_request.clear();
        mm2::to_json(json_request, cancel_request);
        resp = RestClient::post("localhost:7777/api/v1/legacy/mm2/cancel_order", "application/json", json_request.dump());
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }


    TEST_CASE_FIXTURE(http_server_tests_fixture, "test mm2 buy")
    {
        std::this_thread::sleep_for(1s);
        mm2::buy_request request{{antara::asset{st_symbol{"RICK"}}}, {antara::asset{st_symbol{"MORTY"}}}, "1", "1"};
        nlohmann::json json_request;
        mm2::to_json(json_request, request);
        auto resp = RestClient::post("localhost:7777/api/v1/legacy/mm2/buy", "application/json", json_request.dump());
        CHECK_EQ(resp.code, 200);
        nlohmann::json resp_answer = nlohmann::json::parse(resp.body);
        mm2::buy_answer answer;
        mm2::from_json(resp_answer, answer);
        mm2::cancel_order_request cancel_request{answer.result_buy.value().uuid};
        json_request.clear();
        mm2::to_json(json_request, cancel_request);
        resp = RestClient::post("localhost:7777/api/v1/legacy/mm2/cancel_order", "application/json", json_request.dump());
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }
}