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

    void from_json(const nlohmann::json &j, prices_cfg &cfg)
    {
        cfg.api_endpoint = st_endpoint{j.at("endpoint").get<std::string>()};
    }

    void from_json(const nlohmann::json &j, config &cfg)
    {
        j.at("cex_infos").get_to(cfg.cex_registry);
        j.at("price_infos").get_to(cfg.prices_registry);
    }

    void to_json(nlohmann::json &j, const cex_cfg &cfg)
    {
        j["cex_endpoint"] = cfg.cex_endpoint.value();
        j["cex_api_public_key"] = cfg.cex_public_key.value();
        j["cex_api_private_key"] = cfg.cex_private_key.value();
    }

    void to_json(nlohmann::json &j, const prices_cfg &cfg)
    {
        j["endpoint"] = cfg.api_endpoint.value();
    }

    void to_json(nlohmann::json &j, const config &cfg)
    {
        j["cex_infos"] = nlohmann::json::object();
        for (auto&&[key, value] : cfg.cex_registry) {
            j["cex_infos"][key] = value;
        }
        j["price_infos"] = nlohmann::json::object();
        for (auto&&[key, value] : cfg.prices_registry) {
            j["price_infos"][key] = value;
        }
    }

    bool config::operator==(const config &rhs) const
    {
        return cex_registry == rhs.cex_registry &&
               prices_registry == rhs.prices_registry;
    }

    bool config::operator!=(const config &rhs) const
    {
        return !(rhs == *this);
    }

    bool prices_cfg::operator==(const prices_cfg &rhs) const
    {
        return api_endpoint.value() == rhs.api_endpoint.value();
    }

    bool prices_cfg::operator!=(const prices_cfg &rhs) const
    {
        return !(rhs == *this);
    }

    bool cex_cfg::operator==(const cex_cfg &rhs) const
    {
        return cex_endpoint.value() == rhs.cex_endpoint.value() &&
               cex_public_key.value() == rhs.cex_public_key.value() &&
               cex_private_key.value() == rhs.cex_private_key.value();
    }

    bool cex_cfg::operator!=(const cex_cfg &rhs) const
    {
        return !(rhs == *this);
    }
}