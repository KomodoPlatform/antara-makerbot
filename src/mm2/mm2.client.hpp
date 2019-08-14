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

#include <future>
#include <string>
#include <thread>
#include <reproc++/reproc.hpp>
#include <config/config.hpp>

namespace antara::mmbot
{

    class thread_safe_string_sink
    {
    public:
        thread_safe_string_sink(std::string &out, std::mutex &mutex)
                : out_(out), mutex_(mutex)
        {}

        bool operator()(const uint8_t *buffer, unsigned int size)
        {
            std::lock_guard<std::mutex> lock(mutex_);
            out_.append(reinterpret_cast<const char *>(buffer), size);
            return true;
        }

    private:
        std::string &out_;
        std::mutex &mutex_;
    };

    class mm2_client
    {
    public:
        explicit mm2_client(const antara::mmbot::config &cfg) : mmbot_cfg_(cfg)
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            using namespace std::literals;
            std::array<std::string, 1> args = {std::filesystem::current_path() / "assets/mm2"};
            auto path = (std::filesystem::current_path() / "assets/").string();
            auto ec = background_.start(args, std::addressof(path));
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
        }

        ~mm2_client()
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            auto ec = background_.stop(reproc::cleanup::kill, reproc::infinite, reproc::cleanup::terminate, reproc::milliseconds(0));
            if (ec) {
                VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
            }
        }

    private:
        [[maybe_unused]] const antara::mmbot::config &mmbot_cfg_;
        reproc::process background_{reproc::cleanup::kill, reproc::infinite, reproc::cleanup::terminate, reproc::milliseconds(0)};
    };
}