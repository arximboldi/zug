//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/maybe_reduced.hpp>
#include <zug/skip.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Lets items through until the predicate does not pass, then stops.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/take_while.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename PredicateT>
constexpr auto take_while(PredicateT predicate)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return compat::invoke(predicate, is...)
                       ? not_reduced(call(
                             step, state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...))
                       : reduced(skip(
                             step, state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...));
        };
    });
}

} // namespace zug
