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
#include "service.price.platform.hpp"
#include "exceptions.price.platform.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("simple service working")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        price_service_platform price_service{cfg};
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"KMD"}};
        CHECK_GT(price_service.get_price(currency_pair).value(), 0.0);
    }

    TEST_CASE ("simple service not working (wrong cfg)")
    {
        config cfg{};
        cfg.prices_registry["coinpaprika"] = price_config{st_endpoint{"wrong"}};
        price_service_platform price_service{cfg};
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"KMD"}};
        CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
    }

    TEST_CASE ("simple service not working (empty cfg)")
    {
        config cfg{};
        //cfg.prices_registry["coinpaprika"] = price_config{st_endpoint{"wrong"}};
        price_service_platform price_service{cfg};
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"KMD"}};
                CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
    }

    TEST_CASE ("simple service not working (wrong pair)")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        price_service_platform price_service{cfg};
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"NONEXISTENT"}};
        CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
    }
}