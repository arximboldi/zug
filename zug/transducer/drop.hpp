//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/skip.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Removes the first `n` elements from the sequence.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/drop.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename IntegralT>
constexpr auto drop(IntegralT n)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto count = state_data(ZUG_FWD(s), [&] { return n; });
            return count != 0 ? wrap_state(skip(step,
                                                state_unwrap(ZUG_FWD(s)),
                                                ZUG_FWD(is)...),
                                           count - 1)
                              : wrap_state(call(step,
                                                state_unwrap(ZUG_FWD(s)),
                                                ZUG_FWD(is)...),
                                           count);
        };
    });
}

} // namespace zug
