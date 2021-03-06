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
#include <unordered_map>

namespace antara::mmbot
{
    template <class PS>
    void strategy_manager<PS>::add_strategy(const market_making_strategy &strat)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        antara::cross cross = strat.pair.to_cross();
        registry_strategies_[cross] = strat;
    }

    template <class PS>
    const market_making_strategy &strategy_manager<PS>::get_strategy(antara::cross cross) const
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        return registry_strategies_.at(cross);
    }

    template <class PS>
    const typename strategy_manager<PS>::registry_strategies &strategy_manager<PS>::get_strategies() const
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        return registry_strategies_;
    }

    template <class PS>
    orders::order_level strategy_manager<PS>::make_bid(
        antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity, antara::pair pair)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        antara::st_spread mod = antara::st_spread{1.0} - spread;
        antara::st_price price = mid * mod;
        orders::order_level ol{price, quantity, pair};
        return ol;
    }

    template <class PS>
    orders::order_level strategy_manager<PS>::make_ask(
        antara::st_price mid, antara::st_spread spread, antara::st_quantity quantity, antara::pair pair)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        antara::st_spread mod = 1.0 + spread;
        antara::st_price price = mid * mod;
        orders::order_level ol{price, quantity, pair, true};
        return ol;
    }

    template <class PS>
    orders::order_group strategy_manager<PS>::create_order_group(
            const market_making_strategy &strat, antara::st_price mid)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto pair = strat.pair;
        auto spread = strat.spread;
        auto quantity = strat.quantity;

        std::vector<orders::order_level> levels;
        auto level = make_bid(mid, spread, quantity, pair);
        levels.push_back(level);

        if (strat.both) {
            orders::order_level other_level = make_ask(mid, spread, quantity, pair);
            levels.push_back(other_level);
        }

        auto os = orders::order_group{pair.to_cross(), levels};
        return os;
    }

    template <class PS>
    orders::order_group strategy_manager<PS>::create_order_group(const market_making_strategy &strat)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto mid = ps_.get_price(strat.pair);
        return create_order_group(strat, mid);
    }

    template <class PS>
    void strategy_manager<PS>::refresh_orders(antara::cross pair)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto strat = registry_strategies_.at(pair);
        auto orders = create_order_group(strat);

        om_.cancel_orders(pair);
        om_.place_order(orders);
    }

    template <class PS>
    void strategy_manager<PS>::refresh_all_orders()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        for(const auto& [pair, strat] : registry_strategies_) {
            refresh_orders(pair);
        }
    }

    // Call refresh_all_orders on a loop
    template <class PS>
    void strategy_manager<PS>::start()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        // TODO
        // For the time being this loops with a sleep
        // But could run in response to price changes in the future
        while(running_) {
            // if there is latency in this function call
            // then we should run this on a new thread for each pair
            refresh_all_orders();
            // std::this_thread::sleep_for(1s);
        }
    }

    template<class PS>
    void strategy_manager<PS>::enable_sm_thread()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        using namespace std::literals;
        this->sm_thread_ = std::thread([this]() {
            loguru::set_thread_name("sm thread");
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            while (this->keep_thread_alive_) {
                DVLOG_F(loguru::Verbosity_INFO, "%s", "sm work started");
                for (auto&& [pair, strats]: registry_strategies_) {
                    auto og = this->create_order_group(strats);
                    this->om_.cancel_orders(pair);
                    this->om_.place_order(og);
                }
                DVLOG_F(loguru::Verbosity_INFO, "%s", "sm work finished");
                std::this_thread::sleep_for(10s);
            }
        });
    }

    template<class PS>
    strategy_manager<PS>::~strategy_manager()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        this->keep_thread_alive_ = false;
        if (sm_thread_.joinable()) {
            sm_thread_.join();
        }
    }

    template class strategy_manager<price_service_platform>;
    //template class strategy_manager<price_service_platform_mock>;
    using real_strategy_manager = strategy_manager<price_service_platform>;
}
