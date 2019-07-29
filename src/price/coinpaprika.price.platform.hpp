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

#include <unordered_map>
#include "abstract.price.platform.hpp"

namespace antara::mmbot
{
    class coinpaprika_price_platform : public abstract_price_platform
    {
    public:
        explicit coinpaprika_price_platform(const config &cfg) : abstract_price_platform(cfg)
        {

        }

        st_price get_price(antara::pair currency_pair) final;

        ~coinpaprika_price_platform() override = default;

    private:
        using coinpaprika_coin_id_translation_registry = std::unordered_map<std::string, std::string>;
        coinpaprika_coin_id_translation_registry coin_id_translation_{{"KMD", "kmd-komodo"}};
    };
}