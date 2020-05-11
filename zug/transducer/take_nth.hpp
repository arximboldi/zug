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
#include <zug/with_state.hpp>

namespace zug {

/*!
 * Lets every nth elements of the sequence through.
 *
 * Similar to
 * [clojure.core/take-nth](https://clojuredocs.org/clojure.core/take-nth).
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/take_nth.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename IntegralT>
auto take_nth(IntegralT nth)
{
    return comp([=](auto step) {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    return wrap_state(step(ZUG_FWD(st), ZUG_FWD(is)...), 1);
                },
                [&](auto&& st) {
                    auto count = state_wrapper_data(ZUG_FWD(st));
                    return count != nth
                               ? wrap_state(state_unwrap(ZUG_FWD(st)),
                                            count + 1)
                               : wrap_state(step(state_unwrap(ZUG_FWD(st)),
                                                 ZUG_FWD(is)...),
                                            1);
                });
        };
    });
};

} // namespace zug
