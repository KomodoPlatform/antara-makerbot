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

#include "dex.hpp"

#include <utils/exceptions.hpp>
#include <utils/pretty_function.hpp>

namespace antara::mmbot
{
    orders::order dex::buy(const orders::order_level &o, antara::pair pair)
    {
        auto answer = mm_.rpc_buy(to_buy(o, pair));

        auto result = answer.result_buy;
        if (result) {
            return to_order(result.value());
        }
    }

    orders::order dex::place([[maybe_unused]] const orders::order_level &o, antara::pair pair)
    {
        if (o.side == antara::side::buy) {
            return buy(o, pair);
        // } else {
        //     return sell(o);
        }
    }

    bool dex::cancel([[maybe_unused]] st_order_id id)
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }

    std::vector<orders::order> dex::get_live_orders()
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }

    orders::order dex::get_order_status([[maybe_unused]] const st_order_id &id)
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }

    std::vector<orders::execution> dex::get_executions([[maybe_unused]] const st_order_id &id)
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }

    std::vector<orders::execution> dex::get_executions([[maybe_unused]] const std::unordered_set<st_order_id> &ids)
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }

    std::vector<orders::execution> dex::get_recent_executions()
    {
        throw mmbot::errors::not_implemented(pretty_function);
    }
}
