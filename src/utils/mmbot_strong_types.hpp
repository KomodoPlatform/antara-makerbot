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
            struct endpoint_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_quote = st::type<
            std::string,
            struct endpoint_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    using st_base = st::type<
            std::string,
            struct endpoint_tag,
            st::equality_comparable,
            st::addable_with<char *>,
            st::addable_with<const char *>>;

    struct pair
    {
        st_quote quote;
        st_base base;
    };
}
