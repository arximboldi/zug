//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/filter.hpp>

namespace zug {

/*!
 * Removes elements that pass the predicate.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/remove.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename PredicateT>
auto remove(PredicateT&& pred)
{
    return filter([=](auto&&... xs) { return !pred(ZUG_FWD(xs)...); });
}

} // namespace zug
