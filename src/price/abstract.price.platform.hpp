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

#include <utils/mmbot_strong_types.hpp>
#include <config/config.hpp>

namespace antara::mmbot
{
    class abstract_price_platform
    {
    public:
        explicit abstract_price_platform(const config &cfg) noexcept : mmbot_config_(cfg)
        {

        }

        virtual double get_price(antara::pair currencyPair) = 0;
        virtual ~abstract_price_platform() = default;

    protected:
        const config &mmbot_config_;
    };
}