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
#include "order.manager.hpp"

namespace antara::mmbot
{
    class order_manager_mock : public abstract_om
    {
    public:
        order_manager_mock() = default;

        MAKE_CONST_MOCK1(get_order, orders::order&(const st_order_id&), override);
        MAKE_CONST_MOCK0(get_all_orders, orders::orders_by_id&(), override);
        MAKE_CONST_MOCK1(get_orders, std::unordered_set<st_order_id>&(antara::cross), override);

        MAKE_CONST_MOCK0(get_all_executions, orders::executions_by_id&(), override);

        MAKE_MOCK1(add_order, void(const orders::order&), override);
        MAKE_MOCK1(add_orders, void(const std::vector<orders::order>&), override);

        MAKE_MOCK1(add_execution, void(const orders::execution&), override);
        MAKE_MOCK1(add_executions, void(const std::vector<orders::execution>&), override);

        MAKE_MOCK1(remove_order, void(const orders::order&), override);

        MAKE_MOCK0(start, void(), override);
        MAKE_MOCK0(poll, void(), override);

        MAKE_MOCK0(update_from_live, void(), override);

        MAKE_MOCK1(place_order, std::optional<st_order_id>(const orders::order_level&), override);
        MAKE_MOCK1(place_order, std::unordered_set<st_order_id>(const orders::order_group&), override);

        MAKE_MOCK1(cancel_orders, std::unordered_set<st_order_id>(antara::cross pair), override);
    };
}
