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

#include "cex.hpp"

#include <utils/exceptions.hpp>

namespace antara::mmbot
{
    std::optional<orders::order> cex_::place_order([[maybe_unused]] const orders::order_level &ol)
    {
        throw mmbot::errors::not_implemented();
    }

    std::optional<orders::order> cex_::mirror([[maybe_unused]] const orders::execution &ex)
    {
        throw mmbot::errors::not_implemented();
    }
}
