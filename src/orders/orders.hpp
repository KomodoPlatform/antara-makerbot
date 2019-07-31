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

#pragma once

#include <vector>

#include <utils/mmbot_strong_types.hpp>

namespace antara::orders
{

class order_level
{
 public:
  antara::st_price price;
  antara::st_quantity quantity;
  antara::side side;

  order_level(antara::st_price price, antara::st_quantity quantity, antara::side side);

  bool operator==(const order_level &rhs) const;
};

class order_set
{
 public:
  antara::pair pair;
  std::vector<order_level> levels;

  order_set();

  order_set(antara::pair pair, std::vector<order_level> levels);
};

}
