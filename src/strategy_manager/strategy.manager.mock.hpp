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
#include "strategy.manager.hpp"

namespace antara::mmbot
{
    class strategy_manager_mock : public strategy_manager
    {
    public:
        strategy_manager_mock() = default;

        MAKE_MOCK1(add_strategy, void(const market_making_strategy&), override);

        MAKE_CONST_MOCK1(get_strategy, market_making_strategy&(const antara::cross&), override);
        MAKE_CONST_MOCK0(get_strategies, registry_strategies&(), override);

        MAKE_MOCK3(make_bid, orders::order_level(st_price, st_spread, st_quantity), override);
        MAKE_MOCK3(make_ask, orders::order_level(st_price, st_spread, st_quantity), override);
        MAKE_MOCK3(create_order_group, orders::order_group(const market_making_strategy&, st_price), override);
    };
}
