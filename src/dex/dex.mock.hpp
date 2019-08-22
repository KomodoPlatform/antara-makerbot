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

#include <doctest/doctest.h>
#include <doctest/trompeloeil.hpp>
#include <trompeloeil.hpp>

#include <utils/mmbot_strong_types.hpp>
#include "dex.hpp"

namespace antara::mmbot
{
    class dex_mock : public dex
    {
    public:
        MAKE_MOCK1(place, orders::order&(const orders::order_level&), override);

        MAKE_MOCK0(get_live_orders, std::vector<orders::order>(), override);
        MAKE_MOCK1(get_order_status, orders::order(const st_order_id&), override);

        MAKE_MOCK0(get_executions, std::vector<orders::execution>(), override);
        MAKE_MOCK1(get_executions, std::vector<orders::execution>(const st_order_id&), override);
        MAKE_MOCK1(get_executions, std::vector<orders::execution>(const std::unordered_set<st_order_id>&), override);
        MAKE_MOCK0(get_recent_executions, std::vector<orders::execution>(), override);
    };
}
