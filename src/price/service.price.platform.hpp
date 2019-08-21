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

#include <thread>
#include <algorithm>
#include <memory>
#include <vector>
#include <unordered_set>
#include <unordered_map>
#include "factory.price.platform.hpp"
#include "abstract.price.platform.hpp"

namespace antara::mmbot
{
    using registry_price_result = std::unordered_map<antara::pair, st_price>;

    class price_service_platform
    {
    public:
        explicit price_service_platform() noexcept;
        ~price_service_platform() noexcept;
        st_price get_price(antara::pair currency_pair) const;
        void enable_price_service_thread();
        nlohmann::json get_all_price_pairs_of_given_coin(const antara::asset &asset);

        nlohmann::json fetch_all_price();

    private:
        using registry_platform_price = std::unordered_map<price_platform_name, price_platform_ptr>;
        std::unordered_set<std::string> coins_to_track_{"BTC", "BCH", "DASH", "LTC", "DOGE", "QTUM", "DGB", "RVN",
                                                        "ETH", "USDC", "BAT", "KMD", "RFOX", "ZILLA", "VRSC"};
        registry_platform_price registry_platform_price_{};
        std::thread price_service_fetcher_;
        std::atomic_bool keep_thread_alive_{true};
        std::mutex price_service_mutex_;
        nlohmann::json price_registry_;
    };
}