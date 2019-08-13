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
    //! BDD
    TEST_SUITE ("http")
    {
        SCENARIO("price service functionnality") {
            GIVEN("a price service with a good configuration") {
                auto cfg = load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
                price_service_platform price_service{cfg};
                WHEN("give a valid asset pair") {
                    antara::pair currency_pair{{st_symbol{"EUR"}},
                                              {st_symbol{"KMD"}}};
                    THEN("i ask for the price of this valid pair") {
                        auto price = price_service.get_price(currency_pair).value();
                        AND_THEN("i'm exepecting to get a price greater than zero") {
                            CHECK_GT(price, 0);
                        }
                    }
                }
                AND_WHEN("give a wrong asset pair") {
                    antara::pair currency_pair{{st_symbol{"EUR"}},
                                              {st_symbol{"NONEXISTENT"}}};
                    THEN("i ask for the price of this wrong pair") {
                        AND_THEN("i'm exepecting to get an exception pair not available") {
                            CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
                        }
                    }
                }
                AND_WHEN("i want to get price with a registry of symbols") {
                    THEN("i create the registry of symbols that i to get price") {
                        registry_quotes_for_specific_base registry_symbols{{"KMD", {st_symbol{"DOGE"}, st_symbol{"ETH"}, st_symbol{"BTC"}}}, {"ZIL", {st_symbol{"KMD"}}}};
                        AND_THEN("i ask for the price using the registry of symbols") {
                            auto res = price_service.get_price(registry_symbols);
                            AND_THEN("i should atleast get 4 different price that's all greater than 0")
                            {
                                CHECK(!res.empty());
                                CHECK_EQ_F(4u, res.size(), "size should be 4");
                                for (auto&& current_result: res) {
                                    CHECK_GT(current_result.second.value(), 0);
                                }
                                CHECK(!price_service.get_all_price().empty());
                            }
                        }
                    }
                }
            }
            GIVEN("a price service with a wrong configuration (bad endpoint)") {
                config cfg{};
                cfg.price_registry["coinpaprika"] = price_config{st_endpoint{"wrong"}};
                price_service_platform price_service{cfg};
                WHEN("give a valid asset pair") {
                    antara::pair currency_pair{{st_symbol{"EUR"}},
                                              {st_symbol{"KMD"}}};
                    THEN("i ask for the price of this valid pair") {
                        AND_THEN("i'm exepecting to get an exception pair not available") {
                            CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
                        }
                    }
                }
            }
            GIVEN("a price service with a wrong configuration (empty") {
                config cfg{};
                price_service_platform price_service{cfg};
                WHEN("give a valid asset pair") {
                    antara::pair currency_pair{{st_symbol{"EUR"}},
                                              {st_symbol{"KMD"}}};
                    THEN("i ask for the price of this valid pair") {
                        AND_THEN("i'm exepecting to get an exception pair not available") {
                            CHECK_THROWS_AS(price_service.get_price(currency_pair), errors::pair_not_available);
                        }
                    }
                }
            }
        }
    }
}
