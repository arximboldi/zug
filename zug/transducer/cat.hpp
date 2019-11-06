//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Similar to clojure.core/cat$1
 */
struct cat_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            return reduce_nested(step, ZUG_FWD(s), ZUG_FWD(is)...);
        };
    }
};

ZUG_INLINE_CONSTEXPR auto cat = comp(cat_t{});

} // namespace zug
