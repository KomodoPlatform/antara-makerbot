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
            server_thread_ = std::thread(&http_server::run, server_);
        }

        ~http_server_tests_fixture()
        {
            server_thread_.join();
        }

    private:
        antara::mmbot::config mmbot_config_{
                mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json")};
        antara::mmbot::http_server server_{mmbot_config_};
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
        auto resp = RestClient::get("localhost:8080/");
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
    }
}