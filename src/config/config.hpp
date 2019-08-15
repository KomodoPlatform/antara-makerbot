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

#include <optional>
#include <utility>
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
    struct cex_config
    {
        bool operator==(const cex_config &rhs) const;

        bool operator!=(const cex_config &rhs) const;

        antara::st_endpoint cex_endpoint;
        antara::st_key cex_public_key;
        antara::st_key cex_private_key;
    };

    struct price_config
    {
        bool operator==(const price_config &rhs) const;

        bool operator!=(const price_config &rhs) const;

        antara::st_endpoint price_endpoint;
        std::optional<antara::st_key> price_api_key{std::nullopt};
    };

    struct electrum_server
    {
        std::string url;
        std::optional<std::string> protocol{"TCP"};
        std::optional<bool> disable_cert_verification{false};
    };

    void to_json(nlohmann::json &j, const electrum_server &cfg);

    struct additional_coin_info
    {
        std::size_t nb_decimals;
        bool is_mm2_compatible;
        bool is_electrum_compatible;
        std::vector<electrum_server> servers_electrum;
    };

    struct config
    {
        bool operator==(const config &rhs) const;

        bool operator!=(const config &rhs) const;

        using cex_infos_registry = std::unordered_map<std::string, cex_config>;
        using price_infos_registry = std::unordered_map<std::string, price_config>;
        using additional_coin_infos_registry = std::unordered_map<std::string, additional_coin_info>;
        cex_infos_registry cex_registry;
        price_infos_registry price_registry;
        using st_http_port = unsigned short;
        st_http_port http_port;
        additional_coin_infos_registry registry_additional_coin_infos;
        std::string mm2_rpc_password{""};
    };

    void from_json(const nlohmann::json &j, cex_config &cfg);

    void to_json(nlohmann::json &j, const cex_config &cfg);

    void to_json(nlohmann::json &j, const price_config &cfg);

    void from_json(const nlohmann::json &j, price_config &cfg);

    void to_json(nlohmann::json &j, const config &cfg);

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

    mmbot::config load_mmbot_config(std::filesystem::path &&config_path, std::string filename) noexcept;

    void fill_with_coins_cfg(const std::filesystem::path &config_path, config &cfg);

    void extract_from_electrum_file(const std::filesystem::path &path, const std::string &coin,
                                    additional_coin_info &additional_info);
}