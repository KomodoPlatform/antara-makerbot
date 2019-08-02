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

#include <fmt/format.h>
#include "antara.utils.hpp"

namespace antara
{
    std::string get_price_as_string_decimal(const mmbot::config &cfg, const antara::pair &pair, st_price price)
    {
        std::string price_str;
        if (cfg.base_ercs_registry.at(pair.base.symbol.value())) {
            price_str = fmt::format("{:.18f}", (static_cast<double>(price.value()) / g_factor) + g_rounding);
        } else {
            price_str = fmt::format("{:.8f}", (static_cast<double>(price.value()) / g_factor) + g_rounding);
        }
        return price_str;
    }
}