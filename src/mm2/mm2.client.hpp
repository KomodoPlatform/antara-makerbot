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
#include <algorithm>
#include <string>
#include <thread>
#include <restclient-cpp/restclient.h>
#include <reproc++/reproc.hpp>
#include <reproc++/sink.hpp>
#include "http/http.endpoints.hpp"
#include "config/config.hpp"

namespace antara::mmbot
{
    namespace mm2
    {
        struct electrum_request
        {
            std::string coin_name;
            std::vector<electrum_server> servers;
            bool with_tx_history{true};
        };

        struct electrum_answer
        {
            std::string address;
            std::string balance;
            std::string result;
            int rpc_result_code;
        };

        void to_json(nlohmann::json &j, const electrum_request &cfg);

        void from_json(const nlohmann::json &j, electrum_answer &answer);

        struct orderbook_request
        {
            antara::pair trading_pair;
        };

        struct order_contents
        {
            antara::asset coin;
            std::string address;
            st_price price_as_integer;
            double price;
            std::size_t num_utxos;
            double ave_volume;
            double max_volume;
            double depth;
            std::string pub_key;
            int age;
            int zcredits;
        };

        struct orderbook_asks
        {
            order_contents ask_contents;
        };

        struct orderbook_bids
        {
            order_contents bids_contents;
        };

        struct orderbook_answer
        {
            std::size_t ask_depth;
            std::vector<orderbook_asks> asks;
            antara::asset base;
            std::size_t bid_depth;
            std::vector<orderbook_bids> bids;
            int net_id;
            std::size_t num_asks;
            std::size_t num_bids;
            antara::asset rel;
            int timestamp;
            int rpc_result_code;
            std::string result;
        };

        void to_json(nlohmann::json &j, const orderbook_request &cfg);

        void from_json(const nlohmann::json &j, order_contents &cfg);

        void from_json(const nlohmann::json &j, orderbook_bids &cfg);

        void from_json(const nlohmann::json &j, orderbook_asks &cfg);

        void from_json(const nlohmann::json &j, orderbook_answer &cfg);

        struct balance_request
        {
            antara::asset coin;
        };

        struct balance_answer
        {
            int rpc_result_code;
            std::string result;
            std::string address;
            std::string balance;
            antara::asset coin;
        };

        void to_json(nlohmann::json &j, const balance_request &cfg);

        void from_json(const nlohmann::json &j, balance_answer &cfg);

        struct version_answer
        {
            std::string version;
            int rpc_result_code;
            std::string result;
        };

        void from_json(const nlohmann::json &j, version_answer &cfg);

        struct setprice_request
        {
            antara::asset base;
            antara::asset rel;
            std::string price;
            std::string volume;
            std::optional<bool> max{std::nullopt};
            std::optional<bool> cancel_previous{std::nullopt};
        };

        struct setprice_result
        {
            antara::asset base;
            antara::asset rel;
            std::string price;
            std::string max_base_vol;
            std::string min_base_vol;
            int created_at;
            nlohmann::json matches;
            std::vector<std::string> started_swaps;
            std::string uuid;
        };
        struct setprice_answer
        {
            setprice_result result_setprice;
            int rpc_result_code;
            std::string result;
        };

        void to_json(nlohmann::json &j, const setprice_request &cfg);
        void from_json(const nlohmann::json &j, setprice_request &cfg);

        void from_json(const nlohmann::json &j, setprice_answer &cfg);

        struct cancel_order_request
        {
            std::string uuid;
        };

        struct cancel_order_answer
        {
            std::string result;
            int rpc_result_code;
        };

        void to_json(nlohmann::json &j, const cancel_order_request& cfg);
        void from_json(const nlohmann::json &j, cancel_order_request &cfg);
        void from_json([[maybe_unused]] const nlohmann::json &j, [[maybe_unused]] cancel_order_answer &cfg);
    }


    class mm2_client
    {
    public:
        explicit mm2_client(bool should_enable_coins = true);

        ~mm2_client() noexcept;

        mm2::electrum_answer rpc_electrum(mm2::electrum_request &&request);

        mm2::orderbook_answer rpc_orderbook(mm2::orderbook_request &&request);

        mm2::balance_answer rpc_balance(mm2::balance_request &&request);

        mm2::setprice_answer rpc_setprice(mm2::setprice_request &&request);

        mm2::cancel_order_answer rpc_cancel_order(mm2::cancel_order_request &&request);
        mm2::version_answer rpc_version();

    private:
        nlohmann::json template_request(std::string method_name) noexcept;

        bool enable_tests_coins();

        template<typename RpcReturnType>
        RpcReturnType rpc_process_call(const RestClient::Response &resp)
        {
            RpcReturnType answer;
            DVLOG_F(loguru::Verbosity_INFO, "resp: %s", resp.body.c_str());
            if (resp.code != 200) {
                answer.rpc_result_code = resp.code;
                answer.result = resp.body;
                return answer;
            }
            try {
                auto json_answer = nlohmann::json::parse(resp.body);
                mm2::from_json(json_answer, answer);
                answer.rpc_result_code = resp.code;
                answer.result = resp.body;
                return answer;
            }
            catch (const std::exception &error) {
                DVLOG_F(loguru::Verbosity_ERROR, "err: %s", error.what());
                answer.rpc_result_code = -1;
                answer.result = error.what();
                return answer;
            }
        }

    private:
        reproc::process background_{reproc::cleanup::terminate, reproc::milliseconds(2000), reproc::cleanup::kill,
                                    reproc::infinite};
        std::thread sink_thread_;
    };
}