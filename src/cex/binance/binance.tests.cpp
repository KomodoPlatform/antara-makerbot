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

#include <orders/orders.hpp>

#include "binance.hpp"

namespace antara::mmbot::tests
{
    namespace cex = antara::mmbot::cex;

    TEST_CASE ("hmac")
    {
        auto api_key = "";
        auto sec_key = "ajflljklaf5jk43lh5k43r3jl";

        auto bin = cex::binance(api_key, sec_key);

        auto data = "foobar";

        auto sig = bin.hmac(sec_key, data);

        std::string expected = "10CE8A7D15487A9A16FB96D0001AF029CB5B59914210754A05B5F197D0060926";

        CHECK_EQ(sig, expected);
    }

    TEST_CASE ("must flip")
    {
        auto pair = antara::pair::of("BTC", "ETH");
        CHECK(cex::binance::must_flip(pair));

        auto pair2 = antara::pair::of("BTC", "USDT");
        CHECK(!(cex::binance::must_flip(pair2)));
    }

    TEST_CASE ("to symbol")
    {
        auto pair = antara::pair::of("A", "B");
        auto symbol = cex::binance::to_symbol(pair);

        CHECK_EQ(symbol, "AB");
    }

    TEST_CASE ("to request")
    {
        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = get_mmbot_config();

        auto id = "id-alkdflsadkfajdlsa";
        auto pair = antara::pair::of("ETH", "BTC");
        auto p = "0.0001";
        auto price = generate_st_price_from_api_price(cfg, pair.quote.symbol, p);
        auto quantity = st_quantity{0.001};
        auto maker = true;

        auto ex = orders::execution{
            id, pair, price, quantity, maker
        };

        auto actual = cex::binance::to_request(ex, false);
        auto expected = cex::binance::order_request{
            "ETHBTC",
            "BUY",
            "LIMIT",
            "0.001000",
            "0.00010000",
            2080249850
        };

        DVLOG_F(loguru::Verbosity_INFO, "expected: %s", expected.to_string().c_str());
        DVLOG_F(loguru::Verbosity_INFO, "actual: %s", actual.to_string().c_str());

        CHECK_EQ(actual, expected);

        auto actual2 = cex::binance::to_request(ex, true);
        auto expected2 = cex::binance::order_request{
            "ETHBTC",
            "SELL",
            "LIMIT",
            "0.001000",
            "0.00010000",
            2080249850
        };

        CHECK_EQ(actual2, expected2);
    }

    TEST_CASE ("can parse")
    {
        auto raw = R"foo(
{
  "symbol": "BTCUSDT",
  "orderId": 28,
  "orderListId": -1,
  "clientOrderId": "6gCrw2kRUAF9CvJDGP16IP",
  "transactTime": 1507725176595,
  "price": "1.00000000",
  "origQty": "10.00000000",
  "executedQty": "10.00000000",
  "cummulativeQuoteQty": "10.00000000",
  "status": "FILLED",
  "timeInForce": "GTC",
  "type": "MARKET",
  "side": "SELL"
}
)foo";

        auto json = nlohmann::json::parse(raw);
        cex::binance::order_result res;
        cex::binance::from_json(json, res);
    }

    TEST_CASE ("mirror")
    {
        load_mmbot_config(std::filesystem::current_path() / "assets", "mmbot_config.json");
        const auto& cfg = get_mmbot_config();

        auto cex_cfg = cfg.cex_registry.at("binance");
        std::string api_key = cex_cfg.cex_public_key.value();
        std::string sec_key = cex_cfg.cex_private_key.value();
        auto bin = cex::binance(api_key, sec_key);

        auto id = "id-alkdflsadkfajdlsa";
        auto pair = antara::pair::of("ETH", "BTC");
        auto p = "0.01";
        auto price = generate_st_price_from_api_price(cfg, pair.quote.symbol, p);
        auto quantity = st_quantity{0.01};
        auto maker = true;

        auto ex = orders::execution{
            id, pair, price, quantity, maker
        };

        // bin.mirror(ex);
    }
}
