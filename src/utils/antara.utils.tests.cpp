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
#include <config/config.hpp>
#include "antara.utils.hpp"

namespace antara::tests
{
    TEST_CASE("antara price as string decimal with normal coin")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto dummy_price = generate_st_price_from_api_price("1.15000000");
        CHECK_EQ(115000000ull, dummy_price.value());
        auto dummy_price_str = get_price_as_string_decimal(cfg, st_symbol{"BTC"}, dummy_price);
        CHECK_EQ("1.15000000", dummy_price_str);
    }

    TEST_CASE("antara price as string decimal with erc coin")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto dummy_price = generate_st_price_from_api_price("0.010089534999000000");
        CHECK_EQ(10089534999000000ull, dummy_price.value());
        auto dummy_price_str = get_price_as_string_decimal(cfg, st_symbol{"ZIL"}, dummy_price);
        CHECK_EQ("0.010089534999000000", dummy_price_str);
    }
}