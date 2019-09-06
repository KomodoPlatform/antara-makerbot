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

#include <string>
#include <unordered_set>
#include <absl/numeric/int128.h>
#include <st/type.hpp>
#include <st/traits.hpp>

namespace antara
{
    static constexpr const int g_factor = 100000000;
    static constexpr const long double g_rounding = 0.000000004999;
    using st_endpoint = st::type<
            std::string,
            struct endpoint_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_key = st::type<
            std::string,
            struct key_tag,
            st::equality_comparable>;

    using st_symbol = st::type<
            std::string,
            struct symbol_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_spread = st::type<
            double,
            struct spread_tag,
            st::arithmetic,
            st::addable_with<double>
    >;

    using st_price = st::type<
            absl::uint128,
            struct price_tag //,
            // st::arithmetic
    >;

    st_price operator*(const st_price &price, const st_spread &spread);
    bool operator==(const st_price &price, const st_price &other);
    bool operator!=(const st_price &price, const st_price &other);
    bool operator<(const st_price &price, const st_price &other);

    using st_order_id = std::string;

    using st_execution_id = std::string;

    using st_quantity = st::type<
            double,
            struct quantity_tag,
            st::arithmetic,
            st::addable_with<double>
    >;

    using maker = bool;

    struct asset
    {
        st_symbol symbol;

        bool operator==(const asset &rhs) const
        {
            return symbol.value() == rhs.symbol.value();
        }

        bool operator!=(const asset &rhs) const
        {
            return !(*this == rhs);
        }
    };

    struct cross
    {
        asset base;
        asset quote;

        bool operator==(const cross &rhs) const
            {
                return (base == rhs.base && quote == rhs.quote)
                    || (base == rhs.quote && quote == rhs.base);
            }

        bool operator!=(const cross &rhs) const
            {
                return !(*this == rhs);
            }

        static cross of(std::string a, std::string b);
    };

    struct pair
    {
        asset bought;
        asset sold;

        bool operator==(const pair &rhs) const
        {
            return bought == rhs.bought && sold == rhs.sold;
        }

        bool operator!=(const pair &rhs) const
        {
            return !(*this == rhs);
        }

        static pair of(std::string a, std::string b);

        antara::pair flip()
        {
            return pair{sold, bought};
        }

        antara::cross to_cross() const
        {
            return { bought, sold };
        }
    };

    enum side
    {
        buy, sell, both
    };
}

namespace std
{
    template<>
    struct hash<antara::pair>
    {
        std::size_t operator()(const antara::pair &p) const
        {
            using std::size_t;
            using std::hash;

            std::size_t h1 = std::hash<std::string>{}(p.bought.symbol.value());
            std::size_t h2 = std::hash<std::string>{}(p.sold.symbol.value());

            return h1 ^ (h2 << 1);
        }
    };

    template<>
    struct hash<antara::cross>
    {
        std::size_t operator()(const antara::cross &x) const
        {
            // using std::size_t;
            // using std::hash;

            decltype(x.base) big = x.base.symbol.value() > x.quote.symbol.value() ? x.base : x.quote ;
            decltype(x.base) small = x.base.symbol.value() < x.quote.symbol.value() ? x.base : x.quote ;

            std::size_t h1 = std::hash<std::string>{}(big.symbol.value());
            std::size_t h2 = std::hash<std::string>{}(small.symbol.value());

            return h1 ^ (h2 << 1);
        }
    };

    template<>
    struct hash<antara::st_symbol>
    {
        std::size_t operator()(const antara::st_symbol &symbol) const
        {
            return std::hash<std::string>()(symbol.value());
        }
    };
}
