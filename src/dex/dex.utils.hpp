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
    mm2::buy_request to_buy (orders::order_level ol, antara::pair pair);
    mm2::sell_request to_sell (orders::order_level ol, antara::pair pair);

    const orders::order to_order (mm2::trade_result res);
    const orders::order to_order (mm2::buy_result res);
    const orders::order to_order (mm2::sell_result res);

    mm2::cancel_order_request out(st_order_id o_id);
    bool in(mm2::cancel_order_answer ans);

    std::vector<orders::order> to_orders(mm2::my_orders_answer answer);
}
