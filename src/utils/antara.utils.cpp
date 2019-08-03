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
        process_functor(price_str, cfg.base_ercs_registry.at(symbol.value()));
        return price_str;
    }

    std::string unformat_str_to_representation_price(const mmbot::config &cfg, const st_symbol &symbol, std::string price_str)
    {
        //! 115000000
        auto nb_decimal = static_cast<int>(cfg.base_ercs_registry.at(symbol.value()));
        if (nb_decimal <= static_cast<int>(price_str.size())) {
            price_str.insert(price_str.size() - nb_decimal, 1, '.');
        }
        return price_str;
    }

    std::string format_str_api_price(const mmbot::config &cfg, const st_symbol &symbol, std::string price_str)
    {
        auto nb_decimal = static_cast<int>(cfg.base_ercs_registry.at(symbol.value()));
        auto after_decimal_str = price_str.substr(price_str.find('.') + 1, price_str.size());
        if (static_cast<int>(after_decimal_str.size()) > nb_decimal) {
            price_str = price_str.substr(0, price_str.find('.') + nb_decimal + 1);
        } else {
            int missing_zero = nb_decimal - static_cast<int>(after_decimal_str.size());
            for (; missing_zero > 0; --missing_zero) {
                price_str += '0';
            }
        }
        if (auto pos = price_str.find('.'); pos != std::string::npos) {
            //! use std::rotate lol ?
            price_str += '*';
            std::iter_swap(price_str.begin() + pos, price_str.begin() + (price_str.size() - 1));
            price_str.erase(pos, 1);
            price_str.pop_back();
        }
        ltrim(price_str, "0");
        return price_str;
    }

    st_price
    generate_st_price_from_api_price(const mmbot::config &cfg, const st_symbol &symbol, std::string price_str) noexcept
    {
        price_str = format_str_api_price(cfg, symbol, price_str);
        return st_price{std::stoull(price_str)};
    }
}