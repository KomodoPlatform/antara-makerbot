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
#include <filesystem>
#include <fstream>
#include <loguru.hpp>
#include "utils/pretty_function.hpp"
#include "utils/mmbot_strong_types.hpp"

namespace antara::mmbot
{
    struct cex_cfg
    {
        antara::st_endpoint cex_endpoint;
        antara::st_key cex_public_key;
        antara::st_key cex_private_key;
    };

    struct prices_cfg
    {
        antara::st_endpoint api_endpoint;
    };

    struct config
    {
        using cex_infos_registry = std::unordered_map<std::string, cex_cfg>;
        using prices_infos_registry = std::unordered_map<std::string, prices_cfg>;
        cex_infos_registry cex_registry;
        prices_infos_registry prices_registry;
    };

    void from_json(const nlohmann::json &j, cex_cfg &cfg);

    void from_json(const nlohmann::json &j, prices_cfg &cfg);

    void from_json(const nlohmann::json &j, config &cfg);

    namespace details
    {
        template<typename TConfig>
        TConfig create_configuration(const std::filesystem::path &config_path,
                                     const std::filesystem::path &full_path) noexcept
        {
            TConfig config_to_export{};
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            DVLOG_F(loguru::Verbosity_WARNING,
                    "path to configuration doesn't exist, creating directories + configuration for you");
            std::error_code ec;
            std::filesystem::create_directories(config_path, ec);
            if (ec) {
                VLOG_F(loguru::Verbosity_WARNING, "creating directories failed: %s, returning default configuration",
                       ec.message().c_str());
                return config_to_export;
            }
            std::ofstream ofs(full_path);
            DCHECK_F(ofs.is_open(), "Failed to open: [%s]", full_path.string().c_str());
            nlohmann::json config_json_data;
            config_json_data = config_to_export;
            DVLOG_F(loguru::Verbosity_INFO, "default config: [%s]", config_json_data.dump().c_str());
            ofs << config_json_data;
            return config_to_export;
        }

        template<typename TConfig>
        TConfig load_config(const std::filesystem::path &full_path) noexcept
        {
            TConfig config_to_fill{};
            VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
            std::ifstream ifs(full_path);
            DCHECK_F(ifs.is_open(), "Failed to open: [%s]", full_path.string().c_str());
            nlohmann::json config_json_data;
            ifs >> config_json_data;
            config_to_fill = config_json_data;
            DVLOG_F(loguru::Verbosity_INFO, "config loaded content: [%s]", config_json_data.dump().c_str());
            return config_to_fill;
        }
    }

    /**
    * @brief This function allows us to load a configuration through a `path` and `filename`.
    *        There are three different behaviors in this function:
    *        - if the parameter path does not exist the function will attempt to create the directories of the given `path`.
    *        - if the configuration does not exist a default one will be **created**.
    *        - if the `path` and the `name` of the file exists, the contents of the configuration will be **loaded**.
    *
    * @tparam TConfig the type of template you want to load
    * @param config_path the path to the configuration you want to load
    * @param filename the name of the configuration you want to load.
    * @return a loaded/created configuration.
    *
    *  Example:
    *  @code{.cpp}
    *   auto cfg = utils::load_configuration<client::config>(std::filesystem::current_path() / "assets", "mmbot_config.json");
    *  @endcode
    */
    template<typename TConfig>
    TConfig load_configuration(std::filesystem::path &&config_path, std::string filename) noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        const auto &full_path = config_path / std::move(filename);
        DVLOG_F(loguru::Verbosity_INFO, "path to configuration -> %s", full_path.string().c_str());
        if (!std::filesystem::exists(config_path)) {
            return details::create_configuration<TConfig>(config_path, full_path);
        }
        return details::load_config<TConfig>(full_path);
    }
}