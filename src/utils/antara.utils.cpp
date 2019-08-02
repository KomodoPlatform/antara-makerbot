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
    std::string get_price_as_string_decimal(const mmbot::config &cfg, const st_symbol &symbol, st_price price) noexcept
    {
        std::string price_str;
        price_str = std::to_string(price.value());
        auto process_functor = [](std::string &str, std::size_t factor) {
            while (str.size() < factor) {
                str.insert(0, "0");
            }
            str.insert(str.size() - factor, ".");
            if (str[0] == '.') {
                str.insert(0, "0");
            }
        };
        process_functor(price_str, cfg.base_ercs_registry.at(symbol.value()) ? 18u : 8u);
        return price_str;
    }

    st_price
    generate_st_price_from_api_price(const mmbot::config &cfg, const st_symbol &symbol, double price_api_value) noexcept
    {
        std::string price_str;
        price_str = fmt::format(cfg.base_ercs_registry.at(symbol.value()) ? "{:.18f}" : "{:.8f}", price_api_value);
        price_str.erase(std::find(price_str.begin(), price_str.end(), '.'));
        ltrim(price_str, "0");
        return st_price{std::stoull(price_str)};
    }
}