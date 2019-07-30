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

#include <numeric>
#include "utils/antara.algorithm.hpp"
#include "exceptions.price.platform.hpp"
#include "service.price.platform.hpp"

namespace antara::mmbot
{
    price_service_platform::price_service_platform(const config &cfg) noexcept : mmbot_config_(cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        for (auto[platform_name, platform_cfg]: cfg.prices_registry) {
            auto current_price_platform_ptr = factory_price_platform::create(platform_name, cfg);
            if (current_price_platform_ptr != nullptr) {
                registry_platform_price_.emplace(platform_name, std::move(current_price_platform_ptr));
            }
        }
    }

    st_price price_service_platform::get_price(antara::pair currency_pair) const
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        std::atomic_size_t nb_calls_succeed = 0u;
        double price = 0.0;
        std::mutex mutex;
        auto price_functor = [&nb_calls_succeed, &mutex, &price, &currency_pair](auto &&current_platform_price) {
            auto &&[platform_name, platform_ptr] = current_platform_price;
            auto current_price = platform_ptr->get_price(currency_pair).value();
            if( current_price != 0.0 ) {
                ++nb_calls_succeed;
            }
            {
                auto const lck = std::lock_guard(mutex);
                price = price + current_price;
            }
        };

        if (!registry_platform_price_.empty()) {
            antara::par_for_each(begin(registry_platform_price_), end(registry_platform_price_), price_functor);
        }

        if (nb_calls_succeed == 0) {
            throw errors::pair_not_available();
        }
        return st_price{price / nb_calls_succeed.load()};
    }
}