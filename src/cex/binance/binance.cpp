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

#include <string>

#include "binance.hpp"

namespace antara::mmbot::cex
{
    bool binance::order_request::operator==(const binance::order_request &other) const
    {
        return symbol == other.symbol
            && side == other.side
            && type == other.type
            && quantity == other.quantity
            && price == other.price;
    }

    bool binance::order_request::operator!=(const binance::order_request &other) const
    {
        return !(*this == other);
    }

    std::string binance::order_request::to_string() const
    {
        std::string s;
        s.append("\n");
        s.append(symbol);
        s.append("\n");
        s.append(side);
        s.append("\n");
        s.append(type);
        s.append("\n");
        s.append(quantity);
        s.append("\n");
        s.append(price);
        s.append("\n");
        s.append(std::to_string(ts));
        s.append("\n");
        return s;
    }

    std::string binance::hmac (std::string key, std::string data)
    {
        auto digest = (char *) HMAC(
            EVP_sha256(),
            key.c_str(), key.size(),
            (unsigned char *) data.c_str(), data.size(),
            NULL, NULL);

        static const char* const lut = "0123456789ABCDEF";
        size_t len = 32;

        std::string output;
        output.reserve(2 * len);
        for (size_t i = 0; i < len; ++i)
        {
            const unsigned char c = digest[i];
            output.push_back(lut[c >> 4]);
            output.push_back(lut[c & 15]);
        }
        return output;
    }

    bool binance::must_flip (antara::pair p)
    {
        auto values = std::map<std::string, int>();
        values.emplace("USDT", 0);
        values.emplace("BTC", 1);
        values.emplace("ETH", 2);

        auto base = p.base.symbol.value();
        int base_value;
        if (values.find(base) == values.end()) {
            base_value = 9;
        } else {
            base_value = values.at(base);
        }

        auto quote = p.quote.symbol.value();
        int quote_value;
        if (values.find(quote) == values.end()) {
            quote_value = 9;
        } else {
            quote_value = values.at(quote);
        }

        return quote_value > base_value;
    }

    std::string binance::to_symbol (antara::pair p)
    {
        auto base = p.base.symbol.value();
        auto quote = p.quote.symbol.value();
        if (must_flip(p)) {
            return quote.append(base);
        } else {
            return base.append(quote);
        }
    }

    std::optional<orders::order> binance::place_order(const orders::order_level &o)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto r = to_request(o, false);
        return send_req(r);
    }

    binance::order_request binance::to_request(const orders::order_level &o, bool mirror)
    {
        const auto& cfg = get_mmbot_config();

        std::string symbol = to_symbol(o.pair);
        // A != B is equivalent to A XOR B
        std::string side = mirror != must_flip(o.pair) ? "SELL" : "BUY" ;
        std::string type = "LIMIT";
        std::string quantity = std::to_string(o.quantity.value());
        std::string price = get_price_as_string_decimal(
            cfg, o.pair.quote.symbol, o.pair.quote.symbol, o.price);
        long long int ts = std::time(0) * 1000;

        return binance::order_request{
            symbol, side, type, quantity, price, ts
        };
    }

    std::optional<orders::order> binance::mirror(const orders::execution &e)
    {
        VLOG_SCOPE_F(loguru::Verbosity_INFO, pretty_function);
        auto r = to_request(e, true);
        return send_req(r);
    }

    binance::order_request binance::to_request(const orders::execution &e, bool mirror)
    {
        const auto& cfg = get_mmbot_config();

        std::string symbol = to_symbol(e.pair);
        // A != B is equivalent to A XOR B
        std::string side = mirror != must_flip(e.pair) ? "SELL" : "BUY" ;
        std::string type = "LIMIT";
        std::string quantity = std::to_string(e.quantity.value());
        std::string price = get_price_as_string_decimal(
            cfg, e.pair.quote.symbol, e.pair.quote.symbol, e.price);
        long long int ts = std::time(0) * 1000;

        return binance::order_request{
            symbol,
            side,
            type,
            quantity,
            price,
            ts
        };
    }

    std::optional<orders::order> binance::send_req(const binance::order_request &req)
    {
        auto url = "https://api.binance.com";

        std::string data("symbol=");
        data.append(req.symbol);

        data.append("&side=");
        data.append(req.side);

        data.append("&type=");
        data.append(req.type);

        data.append("&quantity=");
        data.append(req.quantity);

        data.append("&price=");
        data.append(req.price);

        data.append("&timeInForce=");
        data.append("GTC");

        data.append("&timestamp=");
        data.append(std::to_string(req.ts));

        std::string signature = hmac(sec_key_, data);
        data.append("&signature=");
        data.append(signature);

        DVLOG_F(loguru::Verbosity_INFO, "req: %s", data.c_str());

        RestClient::Response resp;
        try {
            RestClient::init();
            RestClient::Connection* conn = new RestClient::Connection(url);
            conn->AppendHeader("X-MBX-APIKEY", api_key_);
            std::string c_type = "application/x-www-form-urlencoded";
            conn->AppendHeader("Content-Type", c_type);
            resp = conn->post("/api/v3/order/test", data);
        } catch (const std::exception &error) {
            DVLOG_F(loguru::Verbosity_ERROR, "Request error: %s", error.what());
            return std::nullopt;
        }

        DVLOG_F(loguru::Verbosity_INFO, "resp: %s", resp.body.c_str());

        if (resp.code != 200) {
            DVLOG_F(loguru::Verbosity_ERROR, "HTTP error code: %d", resp.code);
            DVLOG_F(loguru::Verbosity_ERROR, "resp: %s", resp.body.c_str());
            return std::nullopt;
        }

        order_result res;
        try {
            auto json_answer = nlohmann::json::parse(resp.body);
            from_json(json_answer, res);
        } catch (const std::exception &error) {
            DVLOG_F(loguru::Verbosity_ERROR, "object parse err: %s", error.what());
            return std::nullopt;
        }

        auto order = to_order(res);
        return std::optional<orders::order>(order);
    }

    void binance::from_json(const nlohmann::json &j, binance::order_result &res)
    {
        j.at("symbol").get_to(res.symbol);
        j.at("orderId").get_to(res.order_id);
        j.at("clientOrderId").get_to(res.client_order_id);
        j.at("transactTime").get_to(res.transact_time);
        j.at("price").get_to(res.price);
        j.at("origQty").get_to(res.quantity);
        j.at("executedQty").get_to(res.filled_quantity);
        j.at("status").get_to(res.status);
        j.at("type").get_to(res.type);
        j.at("side").get_to(res.side);
    }

    orders::order binance::to_order(const binance::order_result &res)
    {
        const auto& cfg = get_mmbot_config();

        auto base = res.symbol.substr(0, 3);
        auto quote = res.symbol.substr(3, 4);
        // Flip if sell?
        auto pair = antara::pair::of(base, quote);

        auto b = orders::order_builder(res.client_order_id, pair);

        auto price = generate_st_price_from_api_price(cfg, pair.base.symbol, res.price);
        b.price(price);
        b.quantity(st_quantity{std::stod(res.quantity)});
        b.filled(st_quantity{std::stod(res.filled_quantity)});

        if (res.status == "NEW" || res.status == "PARTIALLY_FILLED") {
            b.status(orders::order_status::live);
        } else if (res.status == "FILLED") {
            b.status(orders::order_status::filled);
        } else if (res.status == "CANCELED" || res.status == "REJECTED") {
            b.status(orders::order_status::cancelled);
        }

        return b.build();
    }
}
