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

#include <vector>
#include <unordered_map>

#include <orders/orders.hpp>

namespace antara
{
    class abstract_cex
    {
    public:
        virtual ~abstract_cex() = default;

        virtual void place_order(const orders::order_level &ol) = 0;
        virtual void mirror(const orders::execution &ex) = 0;
    };

    class cex : public abstract_cex
    {
    public:
        void place_order(const orders::order_level &ol);
        void mirror(const orders::execution &ex);
    };
}
