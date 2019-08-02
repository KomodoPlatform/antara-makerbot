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
    TEST_CASE ("test run http_server")
    {
        using namespace std::chrono_literals;
        mmbot::config cfg;
        cfg.http_port = 8080;
        http_server server{cfg};
        std::thread thr(&http_server::run, server);
        std::this_thread::sleep_for(1s);
        std::raise(SIGINT);
        thr.join();
    }

    TEST_CASE ("test welcome http_server")
    {
        using namespace std::chrono_literals;
        mmbot::config cfg;
        cfg.http_port = 8080;
        http_server server{cfg};
        std::thread thr(&http_server::run, server);
        std::this_thread::sleep_for(1s);
        auto resp = RestClient::get("localhost:8080/");
        CHECK_EQ(resp.code, 200);
        std::raise(SIGINT);
        thr.join();
    }
}