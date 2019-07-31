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

#include <utils/mmbot_strong_types.hpp>
#include "strategy.manager.hpp"

namespace antara::mmbot::tests
{

TEST_CASE ("")
{
  antara::asset a1 = {antara::st_symbol{"A"}};
  antara::asset a2 = {antara::st_symbol{"A"}};
  antara::asset a3 = {antara::st_symbol{"B"}};

  CHECK_EQ(a1, a2);
  CHECK_NE(a1, a3);
}

TEST_CASE ("")
{
  antara::strategy_manager sm = antara::strategy_manager();

  antara::pair pair = {{st_symbol{"A"}}, {st_symbol{"B"}}};
  st_spread spread = st_spread{0.1};
  st_quantity quantity = st_quantity{5};
  antara::side side = antara::side::sell;

  antara::market_making_strategy strat = antara::market_making_strategy(
      pair,
      spread,
      quantity,
      side);

}

}
