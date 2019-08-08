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

#include <utils/mmbot_strong_types.hpp>

namespace antara::orders
{
    struct order_level
    {
        antara::st_price price;
        antara::st_quantity quantity;
        antara::side side;

        bool operator==(const order_level &other) const;
    };

    struct order_group
    {
        antara::pair pair;
        std::vector<order_level> levels;
    };

    enum class order_status
    {
        live, cancelled, filled
    };

    struct order_status_change
    {
        st_order_id id;
        order_status status;
    };

    struct execution
    {
        st_execution_id id;

        antara::pair pair;
        st_price price;
        st_quantity quantity;
        antara::side side;
        antara::maker maker;

        bool operator==(const execution &other) const;

        bool operator!=(const execution &other) const;
    };

    class order
    {
    public:
        st_order_id id;

        antara::pair pair;
        st_price price;
        st_quantity quantity;
        st_quantity filled;
        antara::side side;
        order_status status;

        std::vector<st_execution_id> execution_ids;

        order(const st_order_id &id, const antara::pair &pair, const st_price &price,
              const st_quantity &quantity, const st_quantity &filled,
              const antara::side &side, const order_status &status) :
                id(id), pair(pair), price(price), quantity(quantity),
                filled(filled), side(side), status(status)
        {};

        bool operator==(const order_level &other) const;

        [[nodiscard]] bool finished() const;

        void change_status(const order_status_change &osc);

        [[nodiscard]] execution
        create_execution(const st_execution_id &id, const st_quantity &quantity, const maker &maker) const;

        void execute(const execution &ex);
    };

    class order_builder
    {
    public:
        order_builder(const st_order_id &id, const antara::pair &pair) : id_(id), pair_(pair)
        {

        }

        order build();

        order_builder &price(const st_price &price);

        order_builder &quantity(const st_quantity &quantity);

        order_builder &filled(const st_quantity &filled);

        order_builder &side(const antara::side &side);

        order_builder &status(const order_status &status);

    private:
        st_order_id id_;
        antara::pair pair_;
        st_price price_{0};
        st_quantity quantity_{0};
        st_quantity filled_{0};
        antara::side side_{antara::side::buy};
        orders::order_status status_{orders::order_status::live};
    };

    using orders_by_id = std::unordered_map<std::string, orders::order>;
    using executions_by_id = std::unordered_map<std::string, orders::execution>;
}
