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

#include <ctime>
#include <loguru.hpp>
#include <nlohmann/json.hpp>
#include <openssl/hmac.h>
#include <openssl/md5.h>
#include <openssl/sha.h>
#include <restclient-cpp/connection.h>
#include <restclient-cpp/restclient.h>
#include <string.h>
#include <string>
#include <utils/antara.utils.hpp>
#include <utils/pretty_function.hpp>

#include <orders/orders.hpp>
#include <cex/cex.hpp>

namespace antara::mmbot::cex
{
    class binance : abstract_cex
    {
    public:
        binance (std::string api_key, std::string sec_key)
            : api_key_(api_key), sec_key_(sec_key)
        {}

        std::string hmac (std::string key, std::string data);
        static bool must_flip(antara::pair p);
        std::string to_symbol (antara::pair p);

        std::optional<orders::order> place_order(const orders::order_level &o);
        std::optional<orders::order> mirror(const orders::execution &e);

        struct order_request
        {
            std::string symbol;
            std::string side;
            std::string type;
            std::string quantity;
            std::string price;
            long long int ts;
        };

        struct order_result
        {
            std::string symbol;
            int order_id;
            std::string client_order_id;
            int transact_time;
            std::string price;
            std::string quantity;
            std::string filled_quantity;
            std::string status;
            std::string type;
            std::string side;
        };

        static void from_json(const nlohmann::json &j, order_result &res);
        static orders::order to_order(const order_result &res);
        order_request to_request(const orders::order_level &o);
        order_request to_request(const orders::execution &e);

    private:
        std::string api_key_;
        std::string sec_key_;

        std::optional<orders::order> send_req (binance::order_request req);
    };
}
