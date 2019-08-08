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

#include "dex.hpp"

#include <utils/exceptions.hpp>

namespace antara
{
    st_order_id dex::place(const orders::order_level &o)
    {
        throw mmbot::errors::not_implemented();
    }

    std::vector<orders::order> dex::get_live_orders()
    {
        throw mmbot::errors::not_implemented();
        // auto v = std::vector<orders::order>();
        // return v;
    }

    orders::order dex::get_order_status(const st_order_id &id)
    {
        throw mmbot::errors::not_implemented();
        // antara::pair pair = pair.of("B", "C");
        // auto b = orders::order_builder(st_order_id{"A"}, pair);
        // orders::order o = b.build();
        // return o;
    }

    std::vector<orders::execution> dex::get_executions()
    {
        throw mmbot::errors::not_implemented();
        // auto exs = std::vector<orders::execution>();
        // return exs;
    }

    std::vector<orders::execution> dex::get_executions(const st_order_id &id)
    {
        throw mmbot::errors::not_implemented();
        // auto exs = std::vector<orders::execution>();
        // return exs;
    }

    std::vector<orders::execution> dex::get_executions(const std::vector<st_order_id> &ids)
    {
        throw mmbot::errors::not_implemented();
        // auto exs = std::vector<orders::execution>();
        // return exs;
    }

    std::vector<orders::execution> dex::get_recent_executions()
    {
        throw mmbot::errors::not_implemented();
        // auto exs = std::vector<orders::execution>();
        // return exs;
    }
}