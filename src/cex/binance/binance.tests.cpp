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

    TEST_CASE ("must flip")
    {
        auto pair = antara::pair::of("BTC", "ETH");
        CHECK(cex::binance::must_flip(pair));

        auto pair2 = antara::pair::of("BTC", "USDT");
        CHECK(!(cex::binance::must_flip(pair2)));
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
}
