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
#include <iostream>

#include "fake.cex.hpp"

namespace antara::mmbot
{
    const orders::order_book &fake_cex::add_book(const orders::order_book &book)
    {
        return order_books_.emplace(book.cross, book).first->second;
    }

    const orders::order_book &fake_cex::get_book(const antara::cross &cross) const
    {
        return order_books_.at(cross);
    }

    std::optional<orders::order> fake_cex::place_order([[maybe_unused]] const orders::order_level &ol)
    {
        throw mmbot::errors::not_implemented();
    }

    std::optional<orders::order> fake_cex::place_order(const orders::order &o)
    {
        auto cross = o.pair.to_cross();

        if (order_books_.find(cross) == order_books_.end()) {
            auto new_book = orders::order_book(cross);
            order_books_.emplace(cross, new_book);
        }

        auto &book = order_books_.at(cross);
        book.add_order(o);

        return std::nullopt;
    }

    bool can_match(const orders::order_book &book)
    {
        return false;
    }

    std::optional<orders::execution> match_single(const orders::order_book &book)
    {
        if (can_match(book)) {
            return std::nullopt;
        } else {
            return std::nullopt;
        }
    }

    std::vector<orders::execution> match_orders(const orders::order_book &book)
    {
        std::vector<orders::execution> exs;

        while (can_match(book)) {
            auto ex = match_single(book);
            if (ex) {
                exs.push_back(ex.value());
            }
        }

        return exs;
    }

    std::optional<orders::order> fake_cex::mirror([[maybe_unused]] const orders::execution &ex)
    {
        throw mmbot::errors::not_implemented();
    }
}
