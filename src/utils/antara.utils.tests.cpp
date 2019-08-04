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
    TEST_CASE("antara price format bitcoin normal price")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");

        auto formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "1.15000000");
        CHECK_EQ("115000000", formatted_price);
        CHECK_EQ("1.15000000", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, formatted_price));

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "1.15");
        CHECK_EQ("115000000", formatted_price);

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "100000.15");
        CHECK_EQ("10000015000000", formatted_price);
        CHECK_EQ("100000.15000000", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, formatted_price));
    }

    TEST_CASE("antara price format bitcoin erc coin normal price")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto formatted_price = format_str_api_price(cfg, st_symbol{"ZIL"}, "0.010089534999000000");
        CHECK_EQ("10089534999000000", formatted_price);
        CHECK_EQ("0.010089534999000000", unformat_str_to_representation_price(cfg, st_symbol{"ZIL"}, formatted_price));
    }

    TEST_CASE("antara price as string decimal with normal coin lot of decimals")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");

        auto formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "0.0000972439793401814");
        CHECK_EQ("9724", formatted_price);

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "0.00009724");
        CHECK_EQ("9724", formatted_price);
        CHECK_EQ("0.00009724", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, formatted_price));

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "9.72439793401814e-05");
        CHECK_EQ("9724", formatted_price);
    }

    TEST_CASE("antara price as string decimal with fiat")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto formatted_price = format_str_api_price(cfg, st_symbol{"EUR"}, "0.92");
        CHECK_EQ("92", formatted_price);
        formatted_price = format_str_api_price(cfg, st_symbol{"EUR"}, "0.922222");
        CHECK_EQ("92", formatted_price);
        CHECK_EQ("0.92", unformat_str_to_representation_price(cfg, st_symbol{"EUR"}, formatted_price));
    }

    TEST_CASE("antara price with more than 20 digits")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto price = generate_st_price_from_api_price(cfg, st_symbol{"ZIL"}, "12345678.010089534999123456");
        CHECK_EQ("12345678.010089534999123456", get_price_as_string_decimal(cfg, st_symbol{"ZIL"}, price));
        price = generate_st_price_from_api_price(cfg, st_symbol{"ZIL"}, "12345678.010089534999000000");
        CHECK_EQ("12345678.010089534999000000", get_price_as_string_decimal(cfg, st_symbol{"ZIL"}, price));
    }

    TEST_CASE("antara price with to much decimal than expected")
    {
        auto cfg = mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        auto price = generate_st_price_from_api_price(cfg, st_symbol{"BTC"}, "17999.204999999998");
        CHECK_EQ("17999.20500000", get_price_as_string_decimal(cfg, st_symbol{"BTC"}, price));
    }
}