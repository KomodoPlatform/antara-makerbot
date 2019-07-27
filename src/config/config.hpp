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
#include <nlohmann/json.hpp>
#include "utils/mmbot_strong_types.hpp"

namespace antara::mmbot
{
    struct cex_cfg
    {
        antara::st_endpoint cex_endpoint;
        antara::st_key cex_public_key;
        antara::st_key cex_private_key;
    };

    struct api_prices_cfg
    {
        antara::st_endpoint api_endpoint;
    };

    struct config
    {
        using cex_infos_registry = std::unordered_map<std::string, cex_cfg>;
        using api_prices_infos_registry = std::unordered_map<std::string, api_prices_cfg>;
        cex_infos_registry cex_registry;
        api_prices_infos_registry api_prices_reigstry;
    };

    void from_json(const nlohmann::json &j, cex_cfg &cfg);
    void from_json(const nlohmann::json &j, api_prices_cfg &cfg);

    void from_json(const nlohmann::json &j, config &cfg);
}