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
    TEST_CASE ("mmbot cfg from json")
    {
        auto json_mmbot_cfg = R"({
  "cex_infos": {
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
  "price_infos": {
    "coinpaprika": {
      "endpoint": "https://api.coinpaprika.com/v1"
    }
  }
})"_json;

        antara::mmbot::config cfg{};
        CHECK_NOTHROW(from_json(json_mmbot_cfg, cfg));
        CHECK_EQ(2, cfg.cex_registry.size());
        CHECK_EQ("https://api.binance.com", cfg.cex_registry["binance"].cex_endpoint.value());
        CHECK_EQ("https://api.coinpaprika.com/v1", cfg.prices_registry["coinpaprika"].api_endpoint.value());
        CHECK_THROWS(cfg.prices_registry.at("nonexistent").api_endpoint.value());
        CHECK_THROWS(cfg.cex_registry.at("nonexistent").cex_endpoint.value());
    }
}