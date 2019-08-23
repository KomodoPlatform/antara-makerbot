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

#include "mmbot_strong_types.hpp"
#include <absl/numeric/int128.h>

namespace antara
{
    st_price operator*(const st_price &price, const st_spread &spread)
    {
        // This means that spreads are accurate to 0.001 or 0.1%
        // Necessary for the conversion from double to uint128
        const int dp = 1000;
        return st_price{(price.value() * absl::uint128(spread.value() * dp)) / dp};
    }

    bool operator==(const st_price &price, const st_price &other)
    {
        return price.value() == other.value();
    }

    bool operator!=(const st_price &price, const st_price &other)
    {
        return !(price == other);
    }

    bool operator<(const st_price &price, const st_price &other)
    {
        return price.value() < other.value();
    }
}
