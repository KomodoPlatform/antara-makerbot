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

#include "strategy.manager.hpp"

namespace antara
{

bool asset::operator==(const asset &other) const
{
  return symbol == other.symbol;
};

bool asset::operator!=(const asset &other) const
{
  return !(this->operator==(other));
}

std::pair<asset, asset> pair::to_std_pair()
{
  return std::pair<asset, asset>(quote, base);
};

bool pair::operator==(const pair &rhs) const
{
  return base == rhs.base && quote == rhs.quote;
};

market_making_strategy::market_making_strategy(
    antara::pair pair,
    antara::st_spread spread,
    antara::st_quantity quantity,
    antara::side side)
{
  this->pair = pair;
  this->spread = spread;
  this->quantity = quantity;
  this->side = side;
};

bool market_making_strategy::operator==(const market_making_strategy &other) const
{
  return pair == other.pair
      && spread == other.spread
      && quantity == other.quantity
      && side == side;
}

strategy_manager::strategy_manager() {}

void antara::strategy_manager::add_strategy(antara::pair pair, market_making_strategy strat)
{
  strategies.emplace(pair, strat);
}

void strategy_manager::add_strategy(market_making_strategy strat)
{
  antara::pair pair = strat.pair;
  this->add_strategy(pair, strat);
}

const market_making_strategy& strategy_manager::get_strategy(const antara::pair& pair) const
{
  return strategies.at(pair);
}

const std::unordered_map<antara::pair, antara::market_making_strategy>& strategy_manager::get_strategies()
{
  return strategies;
}

orders::order_level strategy_manager::make_bid(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity)
{
  antara::st_spread mod = antara::st_spread{1.0} - spread;
  antara::st_price price = antara::st_price{mid.value() * mod.value()};
  antara::side side = antara::side::buy;
  orders::order_level ol(antara::st_price{price}, quantity, side);
  return ol;
}

orders::order_level strategy_manager::make_ask(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity)
{
  antara::st_spread mod = antara::st_spread{1.0} + spread;
  antara::st_price price = antara::st_price{mid.value() * mod.value()};
  antara::side side = antara::side::sell;
  orders::order_level ol(price, quantity, side);
  return ol;
}

orders::order_set strategy_manager::create_order_level(
    antara::pair pair, market_making_strategy strat, antara::st_price mid)
{
  antara::side side = strat.side;
  antara::st_spread spread = strat.spread;
  antara::st_quantity quantity = strat.quantity;

  orders::order_set os;

  switch (side) {

    case antara::side::buy: {
      orders::order_level level = make_bid(mid, spread, quantity);
      std::vector<orders::order_level> levels; levels.push_back(level);
      os = orders::order_set(pair, levels);
      break;
    }

    case antara::side::sell: {
      orders::order_level level = make_ask(mid, spread, quantity);
      std::vector<orders::order_level> levels; levels.push_back(level);
      os = orders::order_set(pair, levels);
      break;
    }

    case antara::side::both: {
      orders::order_level bid = make_bid(mid, spread, quantity);
      orders::order_level ask = make_ask(mid, spread, quantity);
      std::vector<orders::order_level> levels;
      levels.push_back(bid);
      levels.push_back(ask);
      os = orders::order_set(pair, levels);
      break;
    }

  }

  return os;
}

}
