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

#include <vector>
#include <unordered_map>

#include <utils/mmbot_strong_types.hpp>
#include <orders/orders.hpp>

namespace antara {

class market_making_strategy
{
 public:
  antara::pair pair;
  antara::st_spread spread;
  antara::st_quantity quantity;
  antara::side side;

  market_making_strategy(antara::pair pair, antara::st_spread spread, antara::st_quantity quantity, antara::side side);
};

class strategy_manager
{
 public:
  strategy_manager();

  void add_strategy(antara::pair pair, antara::market_making_strategy strat);
  void add_strategy(antara::market_making_strategy strat);

  antara::market_making_strategy& get_strategy(const antara::pair& pair);

 private:
  std::unordered_map<antara::pair, antara::market_making_strategy> strategies;
  orders::order_level make_bid(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity);
  orders::order_level make_ask(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity);
  orders::order_set create_order_level(antara::pair pair, market_making_strategy strat, antara::st_price mid);
};

}

