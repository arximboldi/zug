//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/tuplify.hpp>

#include <utility>

namespace zug {

/*!
 * Reducing function that outputs through the iterator that is carried
 * as state.
 */
constexpr struct output_t
{
    template <typename It, typename... Inputs>
    It operator()(It it, Inputs&&... ins) const
    {
        *it = tuplify(ZUG_FWD(ins)...);
        return ++it;
    };
} output{};

} // namespace zug
