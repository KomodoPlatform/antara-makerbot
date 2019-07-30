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

namespace antara {
  struct asset {
    std::string symbol;

    bool operator==(const asset &other) const {
      return symbol == other.symbol;
    }
  };

  struct pair {
    antara::asset base;
    antara::asset quote;

    std::pair<antara::asset, antara::asset> to_std_pair() {
      return std::pair<antara::asset, antara::asset>(base, quote);
    }

    bool operator==(const pair &rhs) const {
      return base == rhs.base && quote == rhs.quote;
    }
  };
}

namespace std {

  template <>
  struct hash<antara::pair> {
    std::size_t operator()(const antara::pair& p) const {
      using std::size_t;
      using std::hash;
      using std::string;

      std::size_t h1 = std::hash<std::string>{}(p.base.symbol);
      std::size_t h2 = std::hash<std::string>{}(p.quote.symbol);

      return h1 ^ (h2 << 1);
    }
  };
}

enum class Side { BUY, SELL, BOTH };

class MMStrategy {
public:
  antara::pair pair;
  float spread;
  float quantity;
  Side side;
};

class OrderLevel {
 public:
  float price;
  float quantity;
  Side side;

  OrderLevel(float price, float quantity, Side side) {
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

  void add_strategy(antara::pair pair, MMStrategy strat) {
    strategies.insert(std::pair<antara::pair, MMStrategy>(pair, strat));
  }

  MMStrategy get_strategy(antara::pair pair) {
    return strategies.at(pair);
  }

private:
  std::unordered_map<antara::pair, MMStrategy> strategies;

  OrderLevel make_bid(float mid, float spread, float quantity) {
    float price = mid * (1.0 - spread);
    Side side = Side::BUY;
    OrderLevel ol(price, quantity, side);
    return ol;
  }

  OrderLevel make_ask(float mid, float spread, float quantity) {
    float price = mid * (1.0 + spread);
    Side side = Side::SELL;
    OrderLevel ol(price, quantity, side);
    return ol;
  }

  OrderSet create_order_level(antara::pair pair, MMStrategy strat, float mid) {
    Side side = strat.side;
    float spread = strat.spread;
    float quantity = strat.quantity;

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
