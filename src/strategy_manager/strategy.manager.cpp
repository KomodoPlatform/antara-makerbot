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

namespace antara {
  bool asset::operator==(const asset &other) const {
    return symbol == other.symbol;
  };

  std::pair<st_quote, st_base> pair::to_std_pair() {
    return std::pair<st_quote, st_base>(quote, base);
  };

  bool pair::operator==(const pair &rhs) const {
    return base == rhs.base && quote == rhs.quote;
  };
}

namespace std {

  template <>
  struct hash<antara::pair> {
    std::size_t operator()(const antara::pair& p) const {
      using std::size_t;
      using std::hash;

      std::size_t h1 = std::hash<std::string>{}(p.base.value());
      std::size_t h2 = std::hash<std::string>{}(p.quote.value());

      return h1 ^ (h2 << 1);
    }
  };
}

enum class Side { BUY, SELL, BOTH };

class market_making_strategy {
public:
  antara::pair pair;
  antara::st_spread spread;
  antara::st_quantity quantity;
  Side side;
};

class OrderLevel {
public:
  antara::st_price price;
  antara::st_quantity quantity;
  Side side;

  OrderLevel(antara::st_price price, antara::st_quantity quantity, Side side) {
    this->price = price;
    this->quantity = quantity;
    this->side = side;
  }
};

class OrderSet {
public:
  antara::pair pair;
  std::vector<OrderLevel> levels;

  OrderSet();

  OrderSet(antara::pair pair, std::vector<OrderLevel> levels) {
    this->pair = pair;
    this->levels = levels;
  }
};

class StrategyManager {

public:
  StrategyManager() {}

  void add_strategy(antara::pair pair, market_making_strategy strat) {
    strategies.insert(std::pair<antara::pair, market_making_strategy>(pair, strat));
  }

  market_making_strategy get_strategy(antara::pair pair) {
    return strategies.at(pair);
  }

private:
  std::unordered_map<antara::pair, market_making_strategy> strategies;

  OrderLevel make_bid(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) {
    antara::st_spread mod = antara::st_spread{1.0} - spread;
    antara::st_price price = antara::st_price{mid.value() * mod.value()};
    Side side = Side::BUY;
    OrderLevel ol(antara::st_price{price}, quantity, side);
    return ol;
  }

  OrderLevel make_ask(antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity) {
    antara::st_spread mod = antara::st_spread{1.0} + spread;
    antara::st_price price = antara::st_price{mid.value() * mod.value()};
    Side side = Side::SELL;
    OrderLevel ol(price, quantity, side);
    return ol;
  }

  OrderSet create_order_level(antara::pair pair, market_making_strategy strat, antara::st_price mid) {
    Side side = strat.side;
    antara::st_spread spread = strat.spread;
    antara::st_quantity quantity = strat.quantity;

    OrderSet os;

    switch (side) {

    case Side::BUY: {
      OrderLevel level = make_bid(mid, spread, quantity);
      std::vector<OrderLevel> levels; levels.push_back(level);
      os = OrderSet(pair, levels);
      break;
    }

    case Side::SELL: {
      OrderLevel level = make_ask(mid, spread, quantity);
      std::vector<OrderLevel> levels; levels.push_back(level);
      os = OrderSet(pair, levels);
      break;
    }

    case Side::BOTH: {
      OrderLevel bid = make_bid(mid, spread, quantity);
      OrderLevel ask = make_ask(mid, spread, quantity);
      std::vector<OrderLevel> levels;
      levels.push_back(bid);
      levels.push_back(ask);
      os = OrderSet(pair, levels);
      break;
    }

    }

    return os;
  }
};
