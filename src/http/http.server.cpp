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

#include "http/http.server.hpp"

namespace antara::mmbot
{
    http_server::http_server(const mmbot::config &mmbot_cfg) : mmbot_cfg_(mmbot_cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    http_server::router http_server::create_routes()
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

    void http_server::run()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        DVLOG_F(loguru::Verbosity_INFO, "launch http server on port: %d", mmbot_cfg_.http_port);
        restinio::run(
                restinio::on_this_thread<http_server_traits>().port(mmbot_cfg_.http_port).address(
                        "localhost").request_handler(create_routes()));
    }
}