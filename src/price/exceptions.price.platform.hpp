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

#include <stdexcept>

namespace antara::mmbot::errors
{
    class pair_not_available : public std::runtime_error
    {
    public:
        pair_not_available() noexcept : std::runtime_error("pair not available")
        {

        }

        ~pair_not_available() noexcept final = default;
        [[nodiscard]] const char *what() const noexcept final
        {
            return runtime_error::what();
        }
    };
}