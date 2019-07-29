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

#include <string>
#include <memory>
#include "abstract.price.platform.hpp"
#include "coinpaprika.price.platform.hpp"

namespace antara::mmbot
{
    using price_platform_name = std::string;
    using price_platform_ptr = std::unique_ptr<abstract_price_platform>;

    class factory_price_platform
    {
    public:
        template <typename ... Args>
        static price_platform_ptr create(const price_platform_name& price_platform_name, Args&&... args) noexcept
        {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            if (price_platform_name == "coinpaprika") {
                return std::make_unique<coinpaprika_price_platform>(std::forward<Args>(args)...);
            }
            return nullptr;
        }
    };
}