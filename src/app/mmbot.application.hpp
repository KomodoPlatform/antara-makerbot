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

#include <http/http.server.hpp>

namespace antara::mmbot
{
    class application
    {
    public:
        int run();
    private:
        antara::mmbot::config mmbot_config_{
                mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json")};
        antara::mmbot::http_server server{mmbot_config_};
    };
}