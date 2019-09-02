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

#include <vector>
#include <unordered_set>
#include <unordered_map>
#include <string>

#include <absl/numeric/int128.h>
#include <orders/orders.hpp>
#include <utils/mmbot_strong_types.hpp>
#include <utils/antara.utils.hpp>
#include <mm2/mm2.client.hpp>

#include "dex.utils.hpp"

namespace antara::mmbot
{
    mm2::buy_request to_buy (const orders::order_level &ol, antara::pair pair)
    {
        const auto& cfg = get_mmbot_config();

        auto base = pair.base;
        auto quote = pair.quote;
        auto price = get_price_as_string_decimal(cfg, base.symbol, base.symbol, ol.price);
        auto quantity = std::to_string(ol.quantity.value());

        return mm2::buy_request{base, quote, price, quantity};
    }

    mm2::sell_request to_sell (const orders::order_level &ol, antara::pair pair)
    {
        const auto& cfg = get_mmbot_config();

        auto base = pair.base;
        auto quote = pair.quote;
        auto price = get_price_as_string_decimal(cfg, base.symbol, base.symbol, ol.price);
        auto quantity = std::to_string(ol.quantity.value());

        return mm2::sell_request{base, quote, price, quantity};
    }

    const orders::order to_order (const mm2::trade_result &res)
    {
        auto side = res.side;

        auto id = st_order_id{res.uuid};
        auto pair = antara::pair{res.base, res.rel};

        auto base_amount = std::stod(res.base_amount);
        auto rel_amount = std::stod(res.rel_amount);
        auto price = st_price{ absl::uint128( base_amount / rel_amount ) };

        auto quantity = st_quantity{std::stod(res.base_amount)};

        auto b = orders::order_builder(id, pair);
        b.price(price);
        b.quantity(quantity);
        b.side(side);
        return b.build();
    }

    const orders::order to_order (const mm2::order &res)
    {
        auto pair = antara::pair::of(res.base, res.rel);
        auto b = orders::order_builder(st_order_id{res.id}, pair);
        return b.build();
    }

    mm2::cancel_order_request out(st_order_id o_id)
    {
        return mm2::cancel_order_request{o_id};
    }

    bool in(const mm2::cancel_order_answer &ans)
    {
        return ans.result == "success";
    }

    std::vector<orders::order> to_orders(const mm2::my_orders_answer &answer)
    {
        return std::vector<orders::order>();
    }

    std::vector<orders::execution> to_executions(const mm2::my_recent_swaps_answer &answer)
    {
        return std::vector<orders::execution>();
    }
}
