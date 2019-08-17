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
    void to_json(nlohmann::json &j, const electrum_request &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        j["coin"] = cfg.coin_name;
        j["servers"] = cfg.servers;
        j["tx_history"] = cfg.with_tx_history;
    }

    void from_json(const nlohmann::json &j, electrum_answer &answer)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        j.at("address").get_to(answer.address);
        j.at("balance").get_to(answer.balance);
        j.at("result").get_to(answer.result);
    }

    void to_json(nlohmann::json &j, const orderbook_request &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        j["base"] = cfg.trading_pair.base.symbol.value();
        j["rel"] = cfg.trading_pair.quote.symbol.value();
    }

    void from_json(const nlohmann::json &j, order_contents &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        std::string coin;
        j.at("coin").get_to(coin);
        cfg.coin.symbol = st_symbol{coin};
        j.at("address").get_to(cfg.address);
        j.at("price").get_to(cfg.price);
        j.at("numutxos").get_to(cfg.num_utxos);
        j.at("avevolume").get_to(cfg.ave_volume);
        j.at("maxvolume").get_to(cfg.max_volume);
        j.at("depth").get_to(cfg.depth);
        j.at("pubkey").get_to(cfg.pub_key);
        j.at("age").get_to(cfg.age);
        j.at("zcredits").get_to(cfg.zcredits);
    }

    void from_json(const nlohmann::json &j, orderbook_bids &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        from_json(j, cfg.bids_contents);
    }

    void from_json(const nlohmann::json &j, orderbook_asks &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        from_json(j, cfg.ask_contents);
    }

    void from_json(const nlohmann::json &j, orderbook_answer &cfg)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        j.at("askdepth").get_to(cfg.ask_depth);
        j.at("biddepth").get_to(cfg.bid_depth);
        j.at("netid").get_to(cfg.net_id);
        j.at("numasks").get_to(cfg.num_asks);
        j.at("numbids").get_to(cfg.num_bids);
        j.at("timestamp").get_to(cfg.timestamp);
        j.at("bids").get_to(cfg.bids);
        j.at("asks").get_to(cfg.asks);
        cfg.base = antara::asset{st_symbol{j.at("base").get<std::string>()}};
        cfg.rel = antara::asset{st_symbol{j.at("rel").get<std::string>()}};
    }

    void to_json(nlohmann::json &j, const balance_request &cfg)
    {
        j["coin"] = cfg.coin.symbol.value();
    }

    void from_json(const nlohmann::json &j, balance_answer &cfg)
    {
        j.at("address").get_to(cfg.address);
        j.at("balance").get_to(cfg.balance);
        cfg.coin = antara::asset{st_symbol{j.at("coin").get<std::string>()}};
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
            enable_tests_coins();
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
        return rpc_process_call<mm2::electrum_answer>(resp);
    }

    mm2::orderbook_answer mm2_client::rpc_orderbook(mm2::orderbook_request&& request)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto json_data = template_request("orderbook");
        mm2::to_json(json_data, request);
        DVLOG_F(loguru::Verbosity_INFO, "request: %s", json_data.dump().c_str());
        auto resp = RestClient::post(antara::mmbot::mm2_endpoint, "application/json", json_data.dump());
        return rpc_process_call<mm2::orderbook_answer>(resp);
    }

    mm2::balance_answer mm2_client::rpc_balance(mm2::balance_request &&request)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto json_data = template_request("my_balance");
        mm2::to_json(json_data, request);
        DVLOG_F(loguru::Verbosity_INFO, "request: %s", json_data.dump().c_str());
        auto resp = RestClient::post(antara::mmbot::mm2_endpoint, "application/json", json_data.dump());
        return rpc_process_call<mm2::balance_answer>(resp);
    }

    bool mm2_client::enable_tests_coins()
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        bool res = true;
        for (auto&&[current_coin, current_coin_data] : mmbot_cfg_.registry_additional_coin_infos) {
            if (current_coin_data.is_mm2_compatible) {
                if (current_coin_data.is_electrum_compatible && (current_coin == "RICK" || current_coin == "MORTY")) {
                    std::vector<electrum_server> servers;
                    std::copy(begin(current_coin_data.servers_electrum), end(current_coin_data.servers_electrum),
                              std::back_inserter(servers));
                    mm2::electrum_request request{current_coin, servers};
                    auto answer = rpc_electrum(std::move(request));
                    res &= answer.rpc_result_code == 200;
                }
            }
        }
        return res;
    }

    nlohmann::json mm2_client::template_request(std::string method_name) noexcept
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        return {{"method",   method_name},
                {"userpass", this->mmbot_cfg_.mm2_rpc_password}};
    }
}