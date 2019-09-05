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
        abstract_price_platform() noexcept = default;
        [[nodiscard]] virtual st_price get_price(antara::cross currency_pair, [[maybe_unused]] std::size_t nb_try_in_a_row) const = 0;
        virtual ~abstract_price_platform() = default;
    };
}
