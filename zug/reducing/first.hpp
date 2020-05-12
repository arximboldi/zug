//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/util.hpp>

namespace zug {

struct first_t
{
    template <typename StateT, typename... Xs>
    decltype(auto) operator()(StateT&& st, Xs&&...) const
    {
        return ZUG_FWD(st);
    };
};

/*!
 * Reducing function that always returns the initial state.
 */
ZUG_INLINE_CONSTEXPR first_t first{};

} // namespace zug
