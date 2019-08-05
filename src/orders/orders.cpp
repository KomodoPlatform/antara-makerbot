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

#include <utils/mmbot_strong_types.hpp>

#include "orders.hpp"

namespace antara::orders
{
    order::order(st_order_id &id, antara::pair &pair, st_price &price,
                st_quantity &quantity, st_quantity &filled, antara::side &side,
                order_status &status)
    {
        this->id = id;
        this->pair = pair;
        this->price = price;
        this->quantity = quantity;
        this->filled = filled;
        this->side = side;
        this->status = status;
    }

    bool order_level::operator==(const order_level &other) const
    {
        return price == other.price
            && quantity == other.quantity
            && side == other.side;
    }

    void order::change_status(const order_status_change &osc)
    {
        status = osc.status;
    }

    bool execution::operator==(const execution &other) const
    {
        return pair == other.pair
            && price == other.price
            && quantity == other.quantity
            && side == other.side
            && maker == other.maker;
    }

    bool execution::operator!=(const execution &other) const
    {
        return !(*this == other);
    }

    const execution order::create_execution(const st_quantity &q, const st_maker &maker) const
    {
        return execution{pair, price, q, side, maker};
    }

    void order::execute(const execution &ex)
    {
        this->filled = this->filled + ex.quantity;
    }

    // Order Builder

    order_builder::order_builder(const st_order_id &id, const antara::pair &pair)
    {
        this->id_ = id;
        this->pair_ = pair;

        this->price_ = st_price{0};
        this->quantity_ = st_quantity{0};
        this->filled_ = st_quantity{0};
        this->side_ = antara::side::buy;
        this->status_ = orders::order_status::live;
    }

    order_builder order_builder::price(const st_price &price)
    {
        this->price_ = price;
    }

    order_builder order_builder::quantity(const st_quantity &quantity)
    {
        this->quantity_ = quantity;
    }

    order_builder order_builder::filled(const st_quantity &filled)
    {
        this->filled_ = filled;
    }

    order_builder order_builder::side(const antara::side &side)
    {
        this->side_ = side;
    }

    order_builder order_builder::status(const orders::order_status &status)
    {
        this->status_ = status;
    }

    order order_builder::build()
    {
        return order(id_, pair_, price_, quantity_, filled_, side_, status_);
    }
}
