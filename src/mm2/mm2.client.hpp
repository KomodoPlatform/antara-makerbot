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

#include <string>
#include <thread>
#include <reproc++/reproc.hpp>
#include <config/config.hpp>
#include <reproc++/sink.hpp>

namespace antara::mmbot
{
    class mm2_client
    {
    public:
        explicit mm2_client(const antara::mmbot::config &cfg) : mmbot_cfg_(cfg)
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            using namespace std::literals;
            std::array<std::string, 1> args = {(std::filesystem::current_path() / "assets/mm2").string()};
            auto path = (std::filesystem::current_path() / "assets/").string();
            auto ec = background_.start(args, std::addressof(path));
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
            ec = background_.wait(5s);
            if (ec != reproc::error::wait_timeout) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            } else {
                VLOG_SCOPE_F(loguru::Verbosity_INFO, "mm2 successfully launched");
            }
            sink_thread_ = std::thread(
                    [this]() { this->background_.drain(reproc::stream::out, reproc::sink::discard()); });
        }

        ~mm2_client() noexcept
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            auto ec = background_.stop(reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                       reproc::infinite);
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
            sink_thread_.join();
        }

    private:
        [[maybe_unused]] const antara::mmbot::config &mmbot_cfg_;
        reproc::process background_{reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                    reproc::infinite};
        std::thread sink_thread_;
    };
}