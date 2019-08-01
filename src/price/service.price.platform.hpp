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
#include <memory>
#include <vector>
#include <unordered_map>
#include "factory.price.platform.hpp"
#include "abstract.price.platform.hpp"

namespace antara::mmbot
{
    using registry_quotes_for_specific_base = std::unordered_map<st_symbol::value_type, std::vector<st_symbol>>;
    using registry_price_result = std::unordered_map<antara::pair, st_price>;
    class price_service_platform
    {
    public:
        explicit price_service_platform(const config &cfg) noexcept;

        st_price get_price(antara::pair currency_pair) const;
        registry_price_result get_price(const registry_quotes_for_specific_base& quotes_for_specific_base);
        [[nodiscard]] const std::string& get_all_price() const;;
    private:
        using registry_platform_price = std::unordered_map<price_platform_name, price_platform_ptr>;
        [[maybe_unused]] const config &mmbot_config_;
        registry_platform_price registry_platform_price_{};
        std::string all_price_;
    };
}