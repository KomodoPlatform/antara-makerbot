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
    std::optional<orders::order> dex::buy(const orders::order_level &o)
    {
        auto answer = mm_.rpc_buy(to_buy(o));

        auto result = answer.result_trade;
        if (result) {
            return std::make_optional<orders::order>(to_order(result.value()));
        } else {
            return std::nullopt;
        }
    }

    std::optional<orders::order> dex::sell(const orders::order_level &o, antara::cross pair)
    {
        // auto answer = mm_.rpc_sell(to_sell(o, pair));

        // auto result = answer.result_trade;
        // if (result) {
        //     return std::make_optional<orders::order>(to_order(result.value()));
        // } else {
            return std::nullopt;
        // }
    }

    std::optional<orders::order> dex::place([[maybe_unused]] const orders::order_level &o)
    {
        // if (o.side == antara::side::buy) {
            return buy(o);
        // } else {
        //     return sell(o, pair);
        // }
    }

    bool dex::cancel([[maybe_unused]] st_order_id id)
    {
        auto answer = mm_.rpc_cancel_order(out(id));
        return in(answer);
    }

    std::vector<orders::order> dex::get_live_orders()
    {
        auto answer = mm_.rpc_my_orders();
        return to_orders(answer);
    }

    orders::order dex::get_order_status([[maybe_unused]] const st_order_id &id)
    {
        auto answer = mm_.rpc_order_status(id);
        return to_order(answer.o);
    }

    std::vector<orders::execution> dex::get_executions([[maybe_unused]] const st_order_id &id)
    {
        std::vector<orders::execution> executions;

        auto order = mm_.rpc_order_status(id);
        for (const auto &swap_id : order.swaps) {
            auto answer = mm_.rpc_my_swap_status(swap_id);
            executions.push_back(to_execution(answer.result.s));
        }

        return executions;
    }

    std::vector<orders::execution> dex::get_executions([[maybe_unused]] const std::unordered_set<st_order_id> &ids)
    {
        std::vector<orders::execution> executions;

        for (auto id : ids) {
            auto exs = get_executions(id);
            for (auto ex : exs) {
                executions.push_back(ex);
            }
        }

        return executions;
    }

    std::vector<orders::execution> dex::get_recent_executions()
    {
        //auto answer = mm_.rpc_my_recent_swaps();
        //return to_executions(answer);
        return {};
    }
}
