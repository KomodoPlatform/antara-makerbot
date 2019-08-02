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
    std::string get_price_as_string_decimal(const mmbot::config &cfg, const antara::pair &pair, st_price price) noexcept
    {
        std::string price_str;
        if (cfg.base_ercs_registry.at(pair.base.symbol.value())) {
            price_str = fmt::format("{:.18f}", generate_api_price_from_st_price(price));
        } else {
            price_str = fmt::format("{:.8f}", generate_api_price_from_st_price(price));
        }
        return price_str;
    }

    double generate_api_price_from_st_price(st_price price) noexcept
    {
        return (static_cast<double>(price.value()) / g_factor) + g_rounding;
    }

    st_price generate_st_price_from_api_price(double price_api_value) noexcept
    {
        return st_price{static_cast<std::uint64_t>((price_api_value + g_rounding) * g_factor)};
    }
}