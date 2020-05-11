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
#include <zug/util.hpp>

namespace zug {

/*!
 * Transducer that removes all inputs that do not pass the `predicate`.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/filter.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst

 */
template <typename PredicateT>
auto filter(PredicateT&& predicate)
{
    return comp([=](auto&& step) {
        return [=, p = predicate](auto&& s, auto&&... is) mutable {
            return compat::invoke(p, is...)
                       ? call(step, ZUG_FWD(s), ZUG_FWD(is)...)
                       : skip(step, ZUG_FWD(s), ZUG_FWD(is)...);
        };
    });
}

} // namespace zug
