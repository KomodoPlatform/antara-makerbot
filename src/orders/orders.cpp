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

#include "utils/mmbot_strong_types.hpp"

#include "orders.hpp"

namespace antara
{
    pair pair::of (std::string a, std::string b)
    {
        antara::pair pair = {{st_symbol{std::move(a)}}, {st_symbol{std::move(b)}}};
        return pair;
    }
}

namespace antara::mmbot::orders
{
    // Order level

    bool order_level::operator==(const order_level &other) const
    {
        return price.value() == other.price.value()
            && quantity == other.quantity
            && side == other.side;
    }

    // Execution

    bool execution::operator==(const execution &other) const
    {
        return pair == other.pair
            && price.value() == other.price.value()
            && quantity == other.quantity
            && side == other.side
            && maker == other.maker;
    }

    bool execution::operator!=(const execution &other) const
    {
        return !(*this == other);
    }

    // Order

    bool order::finished() const
    {
        return (status == orders::order_status::cancelled);
    }

    void order::change_status(const order_status_change &osc)
    {
        status = osc.status;
    }

    execution order::create_execution(const st_execution_id &execution_id, const st_quantity &q, const maker &maker) const
    {
        return execution{execution_id, pair, price, q, side, maker };
    }

    void order::execute(const execution &ex)
    {
        this->filled = this->filled + ex.quantity;
    }

    void order::add_execution_id(const st_execution_id &e_id)
    {
        this->execution_ids.emplace(e_id);
    }

    // Order Builder

    order_builder& order_builder::price(const st_price &price)
    {
        this->price_ = price;
        return *this;
    }

    order_builder& order_builder::quantity(const st_quantity &quantity)
    {
        this->quantity_ = quantity;
        return *this;
    }

    order_builder& order_builder::filled(const st_quantity &filled)
    {
        this->filled_ = filled;
        return *this;
    }

    order_builder& order_builder::side(const antara::side &side)
    {
        this->side_ = side;
        return *this;
    }

    order_builder& order_builder::status(const orders::order_status &status)
    {
        this->status_ = status;
        return *this;
    }

    order order_builder::build()
    {
        return order(id_, pair_, price_, quantity_, filled_, side_, status_);
    }

    const orders_by_price &order_book::get_bids() const
    {
        return bids_;
    }

    const orders_by_price &order_book::get_asks() const
    {
        return asks_;
    }

    void &order_book::add_order(const order &o)
    {
        // auto side = o.side == antara::side::buy ? bids_ : asks_ ;
        auto &side = bids_;
        if (o.side == antara::side::buy) {
            side = bids_;
        } else {
            side = asks_;
        }

        auto price = o.price;

        if (side.find(price) == side.end()) {
            side.emplace(price, std::vector<orders::order>());
        }

        auto &orders = side.at(price);
        orders.push_back(o);

        return;
    }
}
