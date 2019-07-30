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

#include <taskflow/taskflow.hpp>

namespace antara
{
    template<class InputIt, class UnaryFunction>
    void par_for_each(InputIt first, InputIt last, UnaryFunction f)
    {
        tf::Executor executor;
        tf::Taskflow taskflow;
        for (; first != last; ++first) {
            taskflow.emplace([&f, first]() { f(*first); });
        }
        executor.run(taskflow);
        executor.wait_for_all();
    }
}
