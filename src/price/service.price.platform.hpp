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
#include <unordered_map>
#include "factory.price.platform.hpp"
#include "abstract.price.platform.hpp"

namespace antara::mmbot
{
    class price_service_platform
    {
    public:
        explicit price_service_platform(const config &cfg) noexcept;

        st_price get_price(antara::pair currency_pair);

    private:
        using registry_platform_price = std::unordered_map<price_platform_name, price_platform_ptr>;
        const config &mmbot_config_;
        registry_platform_price registry_platform_price_{};
    };
}