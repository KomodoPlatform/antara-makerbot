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

namespace antara {
  struct asset {
    std::string symbol;

    bool operator==(const asset &other) const;
  };

  // struct pair {
  //   antara::asset base;
  //   antara::asset quote;

  //   std::pair<antara::asset, antara::asset> to_std_pair();

  //   bool operator==(const pair &rhs) const;
  // };
}
