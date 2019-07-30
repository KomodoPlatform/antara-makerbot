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

#include <doctest/doctest.h>
#include "factory.price.platform.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("factory price platform good parameters")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        CHECK_NOTNULL_F(factory_price_platform::create("coinpaprika", cfg), "should not be nullptr");
    }

    TEST_CASE ("factory price platform wrong parameters")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        CHECK_EQ_F(factory_price_platform::create("nonexistent", cfg), nullptr, "should be nullptr");
    }
}