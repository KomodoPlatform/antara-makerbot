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
    void from_json(const nlohmann::json &j, cex_config &cfg)
    {
        cfg.cex_endpoint = st_endpoint{j.at("cex_endpoint").get<std::string>()};
        cfg.cex_public_key = st_key{j.at("cex_api_public_key").get<std::string>()};
        cfg.cex_private_key = st_key{j.at("cex_api_private_key").get<std::string>()};
    }

    void from_json(const nlohmann::json &j, price_config &cfg)
    {
        cfg.price_endpoint = st_endpoint{j.at("price_endpoint").get<std::string>()};
        if (j.count("price_api_key") > 0) {
            cfg.price_api_key = st_key{j.at("price_api_key").get<std::string>()};
        }
    }

    void from_json(const nlohmann::json &j, config &cfg)
    {
        j.at("cex_infos_registry").get_to(cfg.cex_registry);
        j.at("price_infos_registry").get_to(cfg.price_registry);
        j.at("http_port").get_to(cfg.http_port);
    }

    void to_json(nlohmann::json &j, const cex_config &cfg)
    {
        j["cex_endpoint"] = cfg.cex_endpoint.value();
        j["cex_api_public_key"] = cfg.cex_public_key.value();
        j["cex_api_private_key"] = cfg.cex_private_key.value();
    }

    void to_json(nlohmann::json &j, const price_config &cfg)
    {
        j["price_endpoint"] = cfg.price_endpoint.value();
        if (cfg.price_api_key.has_value()) {
            j["price_api_key"] = cfg.price_api_key.value().value();
        }
    }

    void to_json(nlohmann::json &j, const config &cfg)
    {
        j["cex_infos_registry"] = nlohmann::json::object();
        for (auto&&[key, value] : cfg.cex_registry) {
            j["cex_infos"][key] = value;
        }
        j["price_infos_registry"] = nlohmann::json::object();
        for (auto&&[key, value] : cfg.price_registry) {
            j["price_infos"][key] = value;
        }
        j["http_port"] = cfg.http_port;
    }

    mmbot::config load_mmbot_config(std::filesystem::path &&config_path, std::string filename) noexcept
    {
        auto cfg = load_configuration<mmbot::config>(std::forward<std::filesystem::path>(config_path), std::move(filename));
        auto full_path = config_path / "coins.json";
        std::ifstream ifs(full_path);
        DCHECK_F(ifs.is_open(), "Failed to open: [%s]", full_path.string().c_str());
        nlohmann::json coins_json_data;
        ifs >> coins_json_data;
        for (auto &&current_element: coins_json_data) {
            std::size_t nb_decimals = 8u;
            if (current_element.find("etomic") != current_element.end() && current_element.find("decimals") == current_element.end()) {
                nb_decimals = 18;
            } else if (current_element.find("etomic") != current_element.end() && current_element.find("decimals") != current_element.end()) {
                nb_decimals = current_element["decimals"].get<int>();
            }
            cfg.precision_registry.emplace(current_element["coin"].get<std::string>(),
                                           nb_decimals);
        }
        cfg.precision_registry.emplace("EUR", 2u);
        cfg.precision_registry.emplace("USD", 2u);
        return cfg;
    }

    bool config::operator==(const config &rhs) const
    {
        return cex_registry == rhs.cex_registry &&
               price_registry == rhs.price_registry &&
               http_port == rhs.http_port;
    }

    bool config::operator!=(const config &rhs) const
    {
        return !(rhs == *this);
    }

    bool price_config::operator==(const price_config &rhs) const
    {
#ifdef _MSC_VER
        if (price_api_key.has_value() && rhs.price_api_key.has_value()) {
             return price_endpoint.value() == rhs.price_endpoint.value()
                    && price_api_key.value().value() == rhs.price_api_key.value().value();
        } else {
            return price_endpoint.value() == rhs.price_endpoint.value();
        }
#else
        return price_endpoint.value() == rhs.price_endpoint.value()
               && price_api_key == rhs.price_api_key;
#endif
    }

    bool price_config::operator!=(const price_config &rhs) const
    {
        return !(rhs == *this);
    }

    bool cex_config::operator==(const cex_config &rhs) const
    {
        return cex_endpoint.value() == rhs.cex_endpoint.value() &&
               cex_public_key.value() == rhs.cex_public_key.value() &&
               cex_private_key.value() == rhs.cex_private_key.value();
    }

    bool cex_config::operator!=(const cex_config &rhs) const
    {
        return !(rhs == *this);
    }
}