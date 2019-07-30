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
#include <st/type.hpp>
#include <st/traits.hpp>

namespace antara
{
    using st_endpoint = st::type<
            std::string,
            struct endpoint_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_key = st::type<
            std::string,
            struct key_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_quote = st::type<
            std::string,
            struct quote_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_base = st::type<
            std::string,
            struct base_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_spread = st::type<
            double,
            struct spread_tag,
            st::arithmetic
            >;

    using st_price = st::type<
            double,
            struct price_tag,
            st::arithmetic,
            st::addable_with<double>,
            st::multiplicable_with<double>
            >;

    using st_quantity = st::type<
            double,
            struct quantity_tag,
            st::arithmetic,
            st::addable_with<double>
            >;

    struct pair
    {
        st_quote quote;
        st_base base;

        std::pair<st_quote, st_base> to_std_pair();
        bool operator==(const pair &rhs) const;
    };
}
