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

#include <algorithm>
#include <cctype>
#include <locale>
#include <string>
#include "config/config.hpp"
#include "utils/mmbot_strong_types.hpp"

namespace antara
{
    [[nodiscard]] std::string get_price_as_string_decimal(const mmbot::config &cfg, const st_symbol& symbol,
                                                          st_price price) noexcept;

    [[nodiscard]] st_price generate_st_price_from_api_price(const mmbot::config &cfg, const st_symbol &symbol,
                                                            double price_api_value) noexcept;

    //[[nodiscard]]double generate_api_price_from_st_price(st_price price) noexcept;

    static inline void ltrim(std::string &s, const std::string &delimiters = " \f\n\r\t\v")
    {
        s.erase(0, s.find_first_not_of(delimiters));
    }
}