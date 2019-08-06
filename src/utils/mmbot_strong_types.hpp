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
            struct price_tag,
            st::arithmetic
        >;

    using st_order_id = st::type<
            std::string,
            struct order_id_tag
        >;

    using st_quantity = st::type<
            double,
            struct quantity_tag,
            st::arithmetic,
            st::addable_with<double>
        >;

    using st_maker = st::type<
            bool,
            struct maker_tag,
            st::arithmetic
        >;

    struct asset
    {
        st_symbol symbol;

        bool operator==(const asset &rhs) const;

        bool operator!=(const asset &rhs) const;
    };

    struct pair
    {
        asset quote;
        asset base;

        bool operator==(const pair &rhs) const;
        pair of(std::string a, std::string b);
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

            std::size_t h1 = std::hash<std::string>{}(p.base.symbol.value());
            std::size_t h2 = std::hash<std::string>{}(p.quote.symbol.value());

            return h1 ^ (h2 << 1);
        }
    };

    template<>
    struct hash<antara::st_order_id>
    {
        std::size_t operator()(const antara::st_order_id &id) const
        {
            return std::hash<std::string>{}(id.value());
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
