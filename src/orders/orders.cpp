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

#include <stdexcept>
#include "utils/mmbot_strong_types.hpp"

#include "orders.hpp"

namespace antara
{
    pair pair::of(std::string a, std::string b)
    {
        return {{st_symbol{std::move(a)}}, {st_symbol{std::move(b)}}};
    }

    cross cross::of(std::string a, std::string b)
    {
        return {{st_symbol{std::move(a)}}, {st_symbol{std::move(b)}}};
    }
}

namespace antara::mmbot::orders
{
    // Order level

    bool order_level::operator==(const order_level &other) const
    {
        return price.value() == other.price.value()
            && quantity == other.quantity
            && pair == other.pair
            && sell == other.sell;
    }

    bool order_level::operator!=(const order_level &other) const
    {
        return !(*this == other);
    }

    // Order group

    bool order_group::operator==(const order_group &other) const
    {
        auto equal = true;
        if (cross != other.cross) { equal = false; }
        if (levels.size() != other.levels.size()) { equal = false; }

        for (decltype(levels)::size_type i = 0; i < levels.size(); i++) {
            if (levels[i] != other.levels[i]) { equal = false; }
        }

        return equal;
    }

    bool order_group::operator!=(const order_group &other) const
    {
        return !(*this == other);
    }

    // Execution

    bool execution::operator==(const execution &other) const
    {
        return pair == other.pair
            && price.value() == other.price.value()
            && quantity == other.quantity
            && maker == other.maker;
    }

    bool execution::operator!=(const execution &other) const
    {
        return !(*this == other);
    }

    // Order

    bool order::operator==(const order &other) const
    {
        return id == other.id
            && pair == other.pair
            && price == other.price
            && quantity == other.quantity
            && filled == other.filled
            && status == other.status;
    }

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
        return execution{execution_id, pair, price, q, maker };
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

    order_builder& order_builder::status(const orders::order_status &status)
    {
        this->status_ = status;
        return *this;
    }

    order order_builder::build()
    {
        return order(id_, pair_, price_, quantity_, filled_, status_);
    }

    const orders_by_price &order_book::get_bids() const
    {
        return bids_;
    }

    const orders_by_price &order_book::get_asks() const
    {
        return asks_;
    }

    void order_book::add_order(const order &o)
    {
        auto& side = bids_;
        if (cross.base == o.pair.base) {
            side = bids_;
        } else if (cross.base == o.pair.quote) {
            side = asks_;
        } else {
            throw std::logic_error("Wrong pair for this orderbook");
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
