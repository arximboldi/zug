//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/tuplify.hpp>
#include <zug/util.hpp>

namespace zug {

struct last_t
{
    template <typename StateT, typename... InputTs>
    decltype(auto) operator()(StateT&&, InputTs&&... ins) const
    {
        return tuplify(ZUG_FWD(ins)...);
    }
};

/*!
 * Reducing function that produces the tuplification of the last
 * inputs it received.
 *
 * @see tuplify
 */
ZUG_INLINE_CONSTEXPR last_t last{};

} // namespace zug
