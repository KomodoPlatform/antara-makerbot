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

#include <sstream>
#include <fmt/format.h>
#include "antara.utils.hpp"

namespace antara
{
    std::string get_price_as_string_decimal(const mmbot::config &cfg, const st_symbol &symbol, st_price price) noexcept
    {
        std::stringstream ss;
        ss << price.value();
        std::string price_str;
        ss >> price_str;
        return unformat_str_to_representation_price(cfg, symbol, price_str);
    }

    std::string
    unformat_str_to_representation_price(const mmbot::config &cfg, const st_symbol &symbol, std::string price_str)
    {
        auto nb_decimal = static_cast<int>(cfg.precision_registry.at(symbol.value()));

        while (static_cast<int>(price_str.length()) <= nb_decimal) {
            price_str.insert(0, 1, '0');
        }

        if (nb_decimal < static_cast<int>(price_str.length())) {
            price_str.insert(price_str.size() - nb_decimal, 1, '.');
        } else if (nb_decimal == static_cast<int>(price_str.length())) {
            price_str.insert(price_str.size() - nb_decimal, 1, '.');
            price_str.insert(0, 1, '0');
        }
        return price_str;
    }

    std::string format_str_api_price(const mmbot::config &cfg, const st_symbol &symbol, std::string price_str)
    {
        auto nb_decimal = static_cast<int>(cfg.precision_registry.at(symbol.value()));
        auto after_decimal_str = price_str.substr(price_str.find('.') + 1, price_str.size());
        if (static_cast<int>(after_decimal_str.size()) > nb_decimal || price_str.find('e') != std::string::npos) {
            auto as_double = std::stod(price_str);
            price_str = fmt::format("{:." + std::to_string(nb_decimal) + "f}", as_double);
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
        if (price_str.length() <= 20) {
            return st_price{std::stoull(price_str)};
        }

        absl::uint128 value = std::stoull(price_str.substr(0, 20));
        auto low_str = price_str.substr(20);
        for (std::size_t i = 0; i < low_str.size(); ++i) {
            value *= 10;
        }
        value += std::stoull(low_str);
        return st_price{value};
    }
}