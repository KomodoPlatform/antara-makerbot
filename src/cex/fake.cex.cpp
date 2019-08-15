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

#include <utils/exceptions.hpp>

#include "fake.cex.hpp"

namespace antara::mmbot
{
    void fake_cex::add_book(const orders::order_book &book)
    {
        order_books_.emplace(book.pair, book);
    }

    orders::order_book fake_cex::get_book(const antara::pair &pair)
    {
        return order_books_.at(pair);
    }

    void fake_cex::place_order([[maybe_unused]] const orders::order_level &ol)
    {
        throw mmbot::errors::not_implemented();
    }

    void fake_cex::place_order(const orders::order &o)
    {
        auto pair = o.pair;
        orders::order_book book(pair);

        if (order_books_.find(pair) == order_books_.end()) {
            book = orders::order_book(pair);
        } else {
            book = order_books_.at(pair);
        }

        book.add_order(o);
    }

    void fake_cex::mirror([[maybe_unused]] const orders::execution &ex)
    {
        throw mmbot::errors::not_implemented();
    }
}
