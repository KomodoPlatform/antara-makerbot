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

#include "version/version.hpp"
#include "mmbot.application.hpp"

namespace antara::mmbot
{
    int application::run()
    {
        this->price_service_.enable_price_service_thread();
        this->om_.enable_om_service_thread();
        this->sm_.enable_sm_thread();
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        VLOG_SCOPE_F(loguru::Verbosity_INFO, "launching antara-mmbot version: %s", version());
        try {
            server_.run();
        }
        catch (const std::exception &e) {
            VLOG_F(loguru::Verbosity_FATAL, "exception catch: %s", e.what());
            return 1;
        }
        return 0;
    }

    application::application() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }

    application::~application() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
    }
}