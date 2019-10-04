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
    mm2::trade_request to_request (const orders::order_level &ol)
    {
        const auto& cfg = get_mmbot_config();

        auto base = ol.pair.base;
        auto quote = ol.pair.quote;
        auto price = get_price_as_string_decimal(cfg, base.symbol, base.symbol, ol.price);
        auto quantity = std::to_string(ol.quantity.value());

        return mm2::trade_request{base, quote, price, quantity};
    }

    const orders::order to_order (const mm2::trade_result &res)
    {
        const auto &config = get_mmbot_config();

        auto id = st_order_id{res.uuid};
        auto pair = antara::pair{res.base, res.rel};

        auto base_amount = std::stod(res.base_amount);
        auto rel_amount = std::stod(res.rel_amount);

        auto price_str = std::to_string( rel_amount / base_amount  );

        auto symbol = st_symbol{res.rel.symbol.value()};
        auto price = generate_st_price_from_api_price(config, symbol, price_str);

        auto quantity = st_quantity{std::stod(res.base_amount)};

        auto b = orders::order_builder(id, pair);
        b.price(price);
        b.quantity(quantity);
        return b.build();
    }

    const orders::order to_order(const mm2::order &order)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        try {
            auto pair = antara::pair::of(order.base, order.rel);
            auto b = orders::order_builder(order.uuid, pair);
            b.quantity(st_quantity{std::stod(order.base_amount)});
            b.price(st_price{std::stoull(order.price)});
            return b.build();
        }
        catch (const std::exception& error) {
            VLOG_F(loguru::Verbosity_ERROR, "exception catched: %s", error.what());
            return {};
        }
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
        std::vector<orders::order> orders;

        for (const auto& [id, o] : answer.m_orders) {
            orders.push_back(to_order(o));
        }

        for (const auto& [id, o] : answer.t_orders) {
            orders.push_back(to_order(o));
        }

        return orders;
    }

    orders::execution to_execution(const mm2::swap &swap)
    {
        auto events = swap.events;
        if (!events.empty()) {

            auto event = events[0].event;
            if (event.type != "Started") {
                // throw?
            }

            auto data = event.data;

            auto id = data.value().uuid;
            auto pair = antara::pair::of(data.value().maker_coin.value(), data.value().taker_coin.value());

            auto maker_amount = std::stod(data.value().maker_amount.value());
            auto taker_amount = std::stod(data.value().taker_amount.value());
            auto price = st_price{ absl::uint128( taker_amount / maker_amount ) };

            auto quantity = st_quantity{std::stod(data.value().maker_amount.value())};

            // TODO
            antara::maker maker{swap.type == "Maker"};

            return orders::execution{ id.value(), pair, price, quantity, maker };
        }
        return {};
    }

    std::vector<orders::execution> to_executions(const mm2::my_recent_swaps_answer &answer)
    {
        std::vector<orders::execution> executions;
        for (const auto &swap : answer.swaps) {
            auto ex = to_execution(swap);
            executions.push_back(ex);
        }
        return executions;
    }
}
