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

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

#include <absl/numeric/int128.h>
#include <orders/orders.hpp>
#include <utils/mmbot_strong_types.hpp>
#include <utils/antara.utils.hpp>
#include <mm2/mm2.client.hpp>

namespace antara::mmbot
{
    mm2::buy_request to_buy (const orders::order_level &ol);
    // mm2::sell_request to_sell (const orders::order_level &ol, antara::cross pair);

    const orders::order to_order (const mm2::trade_result &res);
    const orders::order to_order (const mm2::order &res);

    mm2::cancel_order_request out(st_order_id o_id);
    bool in(const mm2::cancel_order_answer &ans);

    std::vector<orders::order> to_orders(const mm2::my_orders_answer &answer);

    orders::execution to_execution(const mm2::swap &swap);
    std::vector<orders::execution> to_executions(const mm2::my_recent_swaps_answer &answer);
}
