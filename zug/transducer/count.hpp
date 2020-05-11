//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Generator transducer produces a sequence:
 *
 *   $$init, init+step, ..., init+step*n$$
 *
 * It is inspired by Python's `itertools.count` generator.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/count.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/count.cpp
 *      :language: c++
 *      :start-after: // example2
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename InitT = std::size_t, typename DeltaT = InitT>
constexpr auto count(InitT init = InitT{0}, DeltaT delta = DeltaT{1})
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto count = state_data(ZUG_FWD(s), [=] { return init; });
            auto next  = count;
            next += delta;
            return wrap_state(step(state_unwrap(ZUG_FWD(s)),
                                   ZUG_FWD(is)...,
                                   std::move(count)),
                              std::move(next));
        };
    });
}

} // namespace zug
