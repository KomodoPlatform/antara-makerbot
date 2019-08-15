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

#include "mm2.client.hpp"

namespace antara::mmbot::mm2
{
    void to_json(nlohmann::json &j, const electrum_servers &cfg)
    {
        j["url"] = cfg.url;
        if (cfg.protocol.has_value()) {
            j["protocol"] = cfg.protocol.value();
        }
        if (cfg.disable_cert_verifications.has_value()) {
            j["disable_cert_verification"] = cfg.disable_cert_verifications.value();
        }
    }

    void to_json(nlohmann::json &j, const electrum_request &cfg)
    {
        j["coin"] = cfg.coin_name;
        j["servers"] = cfg.servers;
        j["tx_history"] = cfg.with_tx_history;
    }

    void from_json(const nlohmann::json &j, electrum_answer &answer)
    {
        j.at("address").get_to(answer.address);
        j.at("balance").get_to(answer.balance);
        j.at("result").get_to(answer.result);
    }
}
namespace antara::mmbot
{
    mm2_client::mm2_client(const antara::mmbot::config &cfg, bool should_enable_coins) : mmbot_cfg_(cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        using namespace std::literals;
        std::array<std::string, 1> args = {(std::filesystem::current_path() / "assets/mm2").string()};
        auto path = (std::filesystem::current_path() / "assets/").string();
        auto ec = background_.start(args, path.c_str());
        if (ec) {
            VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
        }
        ec = background_.wait(5s);
        if (ec != reproc::error::wait_timeout) {
            VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
        } else {
            VLOG_SCOPE_F(loguru::Verbosity_INFO, "mm2 successfully launched");
        }
        sink_thread_ = std::thread(
                [this]() { this->background_.drain(reproc::stream::out, reproc::sink::discard()); });
        if (should_enable_coins) {
            enable_coins();
        }
    }

    mm2_client::~mm2_client() noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto ec = background_.stop(reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                   reproc::infinite);
        if (ec) {
            VLOG_SCOPE_F(loguru::Verbosity_ERROR, "error: %s", ec.message().c_str());
        }
        sink_thread_.join();
    }

    mm2::electrum_answer mm2_client::rpc_electrum(mm2::electrum_request &&request)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto json_data = template_request("electrum");
        mm2::to_json(json_data, request);
        DVLOG_F(loguru::Verbosity_INFO, "request: %s", json_data.dump().c_str());
        auto resp = RestClient::post(antara::mmbot::mm2_endpoint, "application/json", json_data.dump());
        try {
            auto json_answer = nlohmann::json::parse(resp.body);
            DVLOG_F(loguru::Verbosity_INFO, "resp: %s", resp.body.c_str());
            mm2::electrum_answer answer;
            mm2::from_json(json_answer, answer);
            answer.rpc_result_code = resp.code;
            return answer;
        }
        catch (const std::exception &error) {
            DVLOG_F(loguru::Verbosity_ERROR, "err: %s", error.what());
            return {"", "", error.what(), -1};
        }
    }
}