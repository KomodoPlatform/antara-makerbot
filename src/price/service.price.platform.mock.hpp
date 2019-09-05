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
#include "service.price.platform.hpp"

namespace antara::mmbot
{
    class price_service_platform_mock
    {
    public:
        MAKE_CONST_MOCK1(get_price, st_price(antara::cross));
        MAKE_MOCK0(enable_price_service_thread, void());
        MAKE_MOCK1(get_all_price_pairs_of_given_coin, nlohmann::json(const antara::asset &asset));
        MAKE_MOCK0(fetch_all_price, nlohmann::json());
        MAKE_MOCK0(get_price_registry, nlohmann::json(), noexcept);
    };
}
