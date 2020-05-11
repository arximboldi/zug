//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/transducer/take.hpp>
#include <zug/util.hpp>

namespace zug {

template <typename ValueT>
constexpr auto repeat(ValueT&& value)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return step(ZUG_FWD(s), ZUG_FWD(is)..., value);
        };
    });
}

/*!
 * Generator transducer produces the values passed as parameter
 * infinitely.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/repeat.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/repeat.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename ValueT, typename... ValueTs>
constexpr auto repeat(ValueT&& r, ValueTs&&... rs)
{
    return comp(repeat(std::forward<ValueT>(r)),
                repeat(std::forward<ValueTs>(rs))...);
}

/*!
 * Generator transducer produces the values passed as parameter
 * up to `n` times.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/repeat.cpp
 *      :language: c++
 *      :start-after: // example3 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename IntegralT, typename... ValueTs>
constexpr auto repeatn(IntegralT&& n, ValueTs&&... rs)
{
    return repeat(std::forward<ValueTs>(rs)...) |
           take(std::forward<IntegralT>(n));
}

} // namespace zug
