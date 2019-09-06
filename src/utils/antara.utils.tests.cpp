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
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();


        auto formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "1.15000000");
        CHECK_EQ("115000000", formatted_price);
        CHECK_EQ("1.15000000", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, st_symbol{"BTC"}, formatted_price));

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "1.15");
        CHECK_EQ("115000000", formatted_price);

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "100000.15");
        CHECK_EQ("10000015000000", formatted_price);
        CHECK_EQ("100000.15000000", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, st_symbol{"BTC"}, formatted_price));


        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "2");
        CHECK_EQ("200000000", formatted_price);

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "2.0");
        CHECK_EQ("200000000", formatted_price);
    }

    TEST_CASE("antara price format bitcoin erc coin normal price")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto formatted_price = format_str_api_price(cfg, st_symbol{"ZIL"}, "0.010089534999000000");
        CHECK_EQ("10089534999000000", formatted_price);
        CHECK_EQ("0.010089534999000000", unformat_str_to_representation_price(cfg, st_symbol{"ZIL"}, st_symbol{"ZIL"}, formatted_price));
    }

    TEST_CASE("antara price as string decimal with normal coin lot of decimals")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "0.0000972439793401814");
        CHECK_EQ("9724", formatted_price);

        formatted_price = format_str_api_price(cfg, st_symbol{"BTC"}, "0.00009724");
        CHECK_EQ("9724", formatted_price);
        CHECK_EQ("0.00009724", unformat_str_to_representation_price(cfg, st_symbol{"BTC"}, st_symbol{"BTC"}, formatted_price));
    }

    TEST_CASE("antara price as string decimal with fiat")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto formatted_price = format_str_api_price(cfg, st_symbol{"EUR"}, "0.92");
        CHECK_EQ("92", formatted_price);
        formatted_price = format_str_api_price(cfg, st_symbol{"EUR"}, "0.922222");
        CHECK_EQ("92", formatted_price);
        CHECK_EQ("0.92", unformat_str_to_representation_price(cfg, st_symbol{"EUR"}, st_symbol{"EUR"}, formatted_price));
    }

    TEST_CASE("antara price with more than 20 digits")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto price = generate_st_price_from_api_price(cfg, st_symbol{"ZIL"}, "12345678.010089534999123456");
        CHECK_EQ("12345678.010089534999123456", get_price_as_string_decimal(cfg, st_symbol{"ZIL"}, st_symbol{"ZIL"}, price));
        price = generate_st_price_from_api_price(cfg, st_symbol{"ZIL"}, "12345678.010089534999000000");
        CHECK_EQ("12345678.010089534999000000", get_price_as_string_decimal(cfg, st_symbol{"ZIL"}, st_symbol{"ZIL"}, price));

        price = generate_st_price_from_api_price(cfg, st_symbol{"ETH"}, "54.27638512030834");
        CHECK_EQ("54.276385120308340000", get_price_as_string_decimal(cfg, st_symbol{"ETH"}, st_symbol{"ETH"}, price));
        CHECK_EQ("54.27638512", get_price_as_string_decimal(cfg, st_symbol{"BTC"}, st_symbol{"ETH"}, price));
    }

    TEST_CASE("antara price with to much decimal than expected")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto price = generate_st_price_from_api_price(cfg, st_symbol{"BTC"}, "17999.204999999998");
        CHECK_EQ("17999.20500000", get_price_as_string_decimal(cfg, st_symbol{"BTC"}, st_symbol{"BTC"}, price));
    }

    TEST_CASE("antara price with scientific notation")
    {
        mmbot::load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = antara::mmbot::get_mmbot_config();
        auto price = generate_st_price_from_api_price(cfg, st_symbol{"DOGE"}, "2.5319564650362795e-7");
        CHECK_EQ("0.00000025", get_price_as_string_decimal(cfg, st_symbol{"DOGE"}, st_symbol{"DOGE"}, price));
    }
}