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

    st_price price_service_platform::get_price(antara::pair currency_pair)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        st_price asset_price{0.0};
        std::size_t nb_calls_succeed = 0u;
        asset_price = std::accumulate(begin(registry_platform_price_),
                        end(registry_platform_price_), asset_price,
                        [&currency_pair, &nb_calls_succeed](auto result, auto &&pair) {
                            auto current_price = pair.second->get_price(currency_pair);
                            nb_calls_succeed += current_price.value() != 0.0;
                            return result + current_price;
                        });
        if (nb_calls_succeed == 0) {
            throw errors::pair_not_available();
        }
        return st_price{asset_price.value() / nb_calls_succeed};
    }
}