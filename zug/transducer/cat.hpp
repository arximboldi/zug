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

/*!
 * Flattens the sequence one level.  If multiple inputs are received, it
 * flattens all inputs in parallel.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/cat.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/cat.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto cat = comp(cat_t{});

} // namespace zug
