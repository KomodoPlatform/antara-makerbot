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
}
