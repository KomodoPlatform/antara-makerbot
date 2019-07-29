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

#include <memory>
#include <doctest/doctest.h>
#include "coinpaprika.price.platform.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE ("simple get price coinpaprika working")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        std::unique_ptr<abstract_price_platform> price_platform = std::make_unique<coinpaprika_price_platform>(cfg);
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"KMD"}};
        CHECK_GT(price_platform->get_price(currency_pair).value(), 0.0);
    }

    TEST_CASE ("simple get price coinpaprika wrong base")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        std::unique_ptr<abstract_price_platform> price_platform = std::make_unique<coinpaprika_price_platform>(cfg);
        antara::pair currency_pair{st_quote{"EUR"}, st_base{"NONEXISTENTBASE"}};
        CHECK_EQ(price_platform->get_price(currency_pair).value(), 0.0);
    }

    TEST_CASE ("simple get price coinpaprika wrong quote")
    {
        auto cfg = load_configuration<config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
        std::unique_ptr<abstract_price_platform> price_platform = std::make_unique<coinpaprika_price_platform>(cfg);
        antara::pair currency_pair{st_quote{"NONEXISTENTQUOTE"}, st_base{"KMD"}};
        CHECK_EQ(price_platform->get_price(currency_pair).value(), 0.0);
    }
}