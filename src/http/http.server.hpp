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

#pragma once

#include <memory>
#include <restinio/all.hpp>
#include <config/config.hpp>
#include <restclient-cpp/restclient.h>

namespace antara::mmbot
{
    struct http_server_traits : public restinio::default_single_thread_traits_t
    {
        using request_handler_t = restinio::router::express_router_t<>;
    };

    class http_server
    {
    public:
        using router = std::unique_ptr<restinio::router::express_router_t<>>;

        http_server(const mmbot::config &mmbot_cfg) : mmbot_cfg_(mmbot_cfg)
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        }

        router create_routes()
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            using namespace restinio;
            auto http_router = std::make_unique<restinio::router::express_router_t<>>();
            http_router->http_get("/", [](auto req, auto) {
                return req->create_response(status_ok()).set_body("Welcome.").done();
            });

            http_router->non_matched_request_handler(
                    [](auto req) {
                        return req->create_response(status_not_found()).set_body("Not Found").done();
                    });
            return http_router;
        }

        void run()
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            DVLOG_F(loguru::Verbosity_INFO, "launch http server on port: %d", mmbot_cfg_.http_port);
            restinio::run(
                    restinio::on_this_thread<http_server_traits>().port(mmbot_cfg_.http_port).address(
                            "localhost").request_handler(create_routes()));
        }

    private:
        const mmbot::config &mmbot_cfg_;

#ifdef DOCTEST_LIBRARY_INCLUDED

#include <thread>
#include <csignal>
        TEST_CASE_CLASS ("test run http_server")
        {
            using namespace std::chrono_literals;
            mmbot::config cfg;
            cfg.http_port = 8080;
            http_server server{cfg};
            std::thread thr(&http_server::run, server);
            std::this_thread::sleep_for(2s);
            std::raise(SIGINT);
            thr.join();
        }


        TEST_CASE_CLASS ("test welcome http_server")
        {
            using namespace std::chrono_literals;
            mmbot::config cfg;
            cfg.http_port = 8080;
            http_server server{cfg};
            std::thread thr(&http_server::run, server);
            std::this_thread::sleep_for(2s);
            auto resp = RestClient::get("localhost:8080/");
            CHECK_EQ(resp.code, 200);
            std::raise(SIGINT);
            thr.join();
        }

#endif
    };
}