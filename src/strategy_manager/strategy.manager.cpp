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

#include "orders/orders.cpp"
#include "strategy.manager.hpp"

namespace antara
{

bool asset::operator==(const asset &other) const
{
  return symbol == other.symbol;
};

std::pair<asset, asset> pair::to_std_pair()
{
  return std::pair<asset, asset>(quote, base);
};

bool pair::operator==(const pair &rhs) const
{
  return base == rhs.base && quote == rhs.quote;
};

class market_making_strategy
{
 public:
  antara::pair pair;
  antara::st_spread spread;
  antara::st_quantity quantity;
  antara::side side;
};

class StrategyManager
{

 public:
  StrategyManager() {}

  void add_strategy(antara::pair pair, market_making_strategy strat)
  {
    strategies.emplace(pair, strat);
  }

  // const market_making_strategy& get_strategy(const antara::pair& pair) const {
  //   return strategies.at(pair);
  // }

  market_making_strategy& get_strategy(const antara::pair& pair)
  {
    return strategies.at(pair);
  }

 private:
  std::unordered_map<antara::pair, market_making_strategy> strategies;

  orders::OrderLevel make_bid(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity)
  {
    antara::st_spread mod = antara::st_spread{1.0} - spread;
    antara::st_price price = antara::st_price{mid.value() * mod.value()};
    antara::side side = antara::side::buy;
    orders::OrderLevel ol(antara::st_price{price}, quantity, side);
    return ol;
  }

  orders::OrderLevel make_ask(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity)
  {
    antara::st_spread mod = antara::st_spread{1.0} + spread;
    antara::st_price price = antara::st_price{mid.value() * mod.value()};
    antara::side side = antara::side::sell;
    orders::OrderLevel ol(price, quantity, side);
    return ol;
  }

  orders::OrderSet create_order_level(antara::pair pair, market_making_strategy strat, antara::st_price mid)
  {
    antara::side side = strat.side;
    antara::st_spread spread = strat.spread;
    antara::st_quantity quantity = strat.quantity;

    orders::OrderSet os;

    switch (side) {

      case antara::side::buy: {
        orders::OrderLevel level = make_bid(mid, spread, quantity);
        std::vector<orders::OrderLevel> levels; levels.push_back(level);
        os = orders::OrderSet(pair, levels);
        break;
      }

      case antara::side::sell: {
        orders::OrderLevel level = make_ask(mid, spread, quantity);
        std::vector<orders::OrderLevel> levels; levels.push_back(level);
        os = orders::OrderSet(pair, levels);
        break;
      }

      case antara::side::both: {
        orders::OrderLevel bid = make_bid(mid, spread, quantity);
        orders::OrderLevel ask = make_ask(mid, spread, quantity);
        std::vector<orders::OrderLevel> levels;
        levels.push_back(bid);
        levels.push_back(ask);
        os = orders::OrderSet(pair, levels);
        break;
      }

    }

    return os;
  }
};

}
