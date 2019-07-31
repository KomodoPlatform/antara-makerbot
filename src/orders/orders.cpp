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

#include <utils/mmbot_strong_types.hpp>

namespace antara
{

namespace orders
{

class OrderLevel
{
 public:
  antara::st_price price;
  antara::st_quantity quantity;
  antara::side side;

  OrderLevel(antara::st_price price, antara::st_quantity quantity, antara::side side)
  {
    this->price = price;
    this->quantity = quantity;
    this->side = side;
  }
};

class OrderSet
{
 public:
  antara::pair pair;
  std::vector<OrderLevel> levels;

  OrderSet();

  OrderSet(antara::pair pair, std::vector<OrderLevel> levels)
  {
    this->pair = pair;
    this->levels = levels;
  }
};

}

}
