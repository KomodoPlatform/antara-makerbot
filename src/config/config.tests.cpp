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
#include "config.hpp"

namespace antara::mmbot::tests
{
    TEST_CASE("load full config json")
    {
        auto cfg = load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        CHECK(!cfg.base_ercs_registry.empty());
        CHECK(!cfg.base_ercs_registry["ETH"]);
        CHECK(cfg.base_ercs_registry["ZIL"]);
    }
    TEST_CASE ("mmbot cfg from json")
    {
        auto json_mmbot_cfg = R"({
  "cex_infos_registry": {
    "binance": {
      "cex_endpoint": "https://api.binance.com",
      "cex_api_public_key": "<your public key here>",
      "cex_api_private_key": "<your private key here>"
    },
    "coinbase": {
      "cex_endpoint": "https://api.pro.coinbase.com",
      "cex_api_public_key": "<your public key here>",
      "cex_api_private_key": ""
    }
  },
  "price_infos_registry": {
    "coinpaprika": {
      "price_endpoint": "https://api.coinpaprika.com/v1"
    }
  },
  "http_port": 8080
})"_json;

        antara::mmbot::config cfg{};
        CHECK_NOTHROW(from_json(json_mmbot_cfg, cfg));
        CHECK_EQ(2, cfg.cex_registry.size());
        CHECK_EQ("https://api.binance.com", cfg.cex_registry["binance"].cex_endpoint.value());
        CHECK_EQ("https://api.coinpaprika.com/v1", cfg.price_registry["coinpaprika"].price_endpoint.value());
        CHECK_THROWS(cfg.price_registry.at("nonexistent").price_endpoint.value());
        CHECK_THROWS(cfg.cex_registry.at("nonexistent").cex_endpoint.value());
    }

    SCENARIO ("loading configuration")
    {
        GIVEN("a configuration doesn't exist in the given path") {
            AND_WHEN("we load the configuration from a root directory") {
                THEN("we got a default configuration") {
#if defined (__unix__) || (defined (__APPLE__) && defined (__MACH__))
                    REQUIRE_EQ(load_configuration<config>("/foo", "mmbot_config.json"), config{});
                    REQUIRE_FALSE(std::filesystem::exists("/foo"));

#else
                    auto res = load_configuration<config>(std::filesystem::path("G:\\foo"), "mmbot_config.json");
                    REQUIRE_EQ(res, config{});
                    auto path_exist = std::filesystem::exists("G:\\foo");
                    REQUIRE_FALSE(path_exist);
#endif
                }
            }
            AND_WHEN ("we load the configuration from a non root directory") {
                THEN("we create a default configuration in the given path and we got a default configuration") {
                    REQUIRE_EQ(load_configuration<config>(std::filesystem::current_path() / "my_assets/config", "mmbot_config.json"), config{});
                    REQUIRE(std::filesystem::exists(std::filesystem::current_path() / "my_assets/config/mmbot_config.json"));
                }
                AND_THEN("We clear the directory that we create for this test") {
                    std::filesystem::remove_all(std::filesystem::current_path() / "my_assets");
                    REQUIRE_FALSE(std::filesystem::exists(std::filesystem::current_path() / "my_assets"));
                }
            }
        }

        GIVEN ("a configuration exist in the given path") {
            auto path = std::filesystem::current_path() / "example_assets";
            THEN("we create the configuration and the directories") {
                auto json_mmbot_cfg = R"({
  "cex_infos_registry": {
    "binance": {
      "cex_endpoint": "https://api.binance.com",
      "cex_api_public_key": "<your public key here>",
      "cex_api_private_key": "<your private key here>"
    },
    "coinbase": {
      "cex_endpoint": "https://api.pro.coinbase.com",
      "cex_api_public_key": "<your public key here>",
      "cex_api_private_key": ""
    }
  },
  "price_infos_registry": {
    "coinpaprika": {
      "price_endpoint": "https://api.coinpaprika.com/v1"
    }
  },
  "http_port": 8080
})"_json;
                std::filesystem::create_directories(path);
                REQUIRE(std::filesystem::exists(path));
                std::ofstream ofs(path / "mmbot_example_config.json");
                REQUIRE(ofs.is_open());
                ofs << json_mmbot_cfg;
                REQUIRE(std::filesystem::exists(path / "mmbot_example_config.json"));
            }
            AND_WHEN("We load the configuration from this fresh directories") {
                THEN("We got this config") {
                    using namespace antara;
                    config mmbot_cfg{
                        config::cex_infos_registry{
                            {
                            "binance",      cex_config{st_endpoint{"https://api.binance.com"}, st_key{"<your public key here>"}, st_key{"<your private key here>"}}
                            },
                            {
                                "coinbase", cex_config{st_endpoint{"https://api.pro.coinbase.com"}, st_key{"<your public key here>"}, st_key{""}}
                            }
                    }, config::price_infos_registry{{"coinpaprika", price_config{st_endpoint{"https://api.coinpaprika.com/v1"}, std::nullopt}}}, 8080,{}};
                    REQUIRE_EQ(load_configuration<config>(std::move(path), "mmbot_example_config.json"), mmbot_cfg);
                }
                AND_THEN("We clear the directory that we create for this test") {
                    std::filesystem::remove_all(std::filesystem::current_path() / "example_assets");
                    REQUIRE_FALSE(std::filesystem::exists(std::filesystem::current_path() / "example_assets"));
                }
            }
        }
    }
}