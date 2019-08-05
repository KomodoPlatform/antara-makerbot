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

#include <cstdlib>
#include "app/mmbot.application.hpp"

int main()
{
    loguru::add_file("mmbot.everything.log", loguru::Append, loguru::Verbosity_MAX);
    loguru::add_file("mmbot.latest.readable.log", loguru::Truncate, loguru::Verbosity_INFO);
    loguru::set_thread_name("main thread");
    loguru::set_fatal_handler([](const loguru::Message& message){
        VLOG_F(loguru::Verbosity_FATAL, "err occured: %s", message.message);
        std::exit(1);
    });
    antara::mmbot::application app;
    return app.run();
}