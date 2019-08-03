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

    struct order_set
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
        antara::pair pair;
        order_level order;
        order_status status;
    };

    struct execution
    {
        antara::pair pair;
        st_price price;
        st_quantity quantity;
        antara::side side;
        st_maker maker;

        bool operator==(const execution &other) const;
        bool operator!=(const execution &other) const;
    };

    class order
    {
    public:
        antara::pair pair;
        st_price price;
        st_quantity quantity;
        st_quantity filled;
        antara::side side;
        order_status status;

        order(antara::pair pair, st_price price, st_quantity quantity,
              st_quantity filled, antara::side side, order_status status);

        bool operator==(const order_level &other) const;

        void change_status(const order_status_change &osc);

        const execution create_execution(const st_quantity &quantity, const st_maker &maker) const;

        void execute(const execution &ex);
    };
}
