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

#include "config.hpp"

namespace antara::mmbot
{
    void from_json(const nlohmann::json &j, cex_cfg &cfg)
    {
        cfg.cex_endpoint = st_endpoint{j.at("cex_endpoint").get<std::string>()};
        cfg.cex_public_key = st_endpoint{j.at("cex_api_public_key").get<std::string>()};
        cfg.cex_private_key = st_endpoint{j.at("cex_api_private_key").get<std::string>()};
    }

    void from_json(const nlohmann::json &j, api_prices_cfg &cfg)
    {
        cfg.api_endpoint = st_endpoint{j.at("endpoint").get<std::string>()};
    }

    void from_json(const nlohmann::json &j, config &cfg)
    {
        j.at("cex_infos").get_to(cfg.cex_registry);
        j.at("price_infos").get_to(cfg.api_prices_reigstry);
    }
}