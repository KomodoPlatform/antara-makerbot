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

#include <functional>
#include <map>
#include <unordered_map>
#include <vector>

#include <orders/orders.hpp>
#include "cex.hpp"

namespace antara::mmbot
{
    class fake_cex : public abstract_cex
    {
        using order_books_by_pair = std::unordered_map<antara::pair, orders::order_book>;

    public:
        fake_cex() = default;

        const orders::order_book &add_book(const orders::order_book &book);
        const orders::order_book &get_book(const antara::pair &pair) const;

        void place_order(const orders::order_level &ol) override;
        void place_order(const orders::order &o);

        void mirror(const orders::execution &ex) override;

    private:
        order_books_by_pair order_books_;
    };
}
