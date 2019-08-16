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

#include <algorithm>
#include <string>
#include <thread>
#include <restclient-cpp/restclient.h>
#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>
#include "http/http.endpoints.hpp"
#include "config/config.hpp"

namespace antara::mmbot
{
    namespace mm2
    {
        struct electrum_request
        {
            std::string coin_name;
            std::vector<electrum_server> servers;
            bool with_tx_history{true};
        };

        struct electrum_answer
        {
            std::string address;
            std::string balance;
            std::string result;
            int rpc_result_code;
        };

        void to_json(nlohmann::json &j, const electrum_request &cfg);

        void from_json(const nlohmann::json &j, electrum_answer &answer);
    }


    class mm2_client
    {
    public:
        explicit mm2_client(const antara::mmbot::config &cfg, bool should_enable_coins = true);

        ~mm2_client() noexcept;

        mm2::electrum_answer rpc_electrum(mm2::electrum_request &&request);

    private:
        nlohmann::json template_request(std::string method_name) noexcept;

        bool enable_tests_coins();

    private:
        [[maybe_unused]] const antara::mmbot::config &mmbot_cfg_;
        reproc::process background_{reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                    reproc::infinite};
        std::thread sink_thread_;
    };
}