//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/state_wrapper.hpp>
#include <zug/transducer/count.hpp>
#include <zug/transducer/take.hpp>

namespace zug {

template <typename StopT>
constexpr auto range(StopT&& stop)
{
    return count() | take(std::forward<StopT>(stop));
}

template <typename StartT, typename StopT>
constexpr auto range(StartT&& start, StopT&& stop)
{
    return count(start) |
           take(std::max(decltype(stop - start){}, stop - start));
}

/*!
 * Produces numbers in the range @f$ [start, stop) @f$ in steps of size @f$ step
 * @f$. `start` and `step` are optional.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/range.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/range.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/range.cpp
 *      :language: c++
 *      :start-after: // example3 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/range.cpp
 *      :language: c++
 *      :start-after: // example4 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename StartT, typename StopT, typename StepT>
constexpr auto range(StartT&& start, StopT&& stop, StepT&& step)
{
    return count(start, step) | take(std::max(decltype((stop - start) / step){},
                                              (stop - start) / step));
}

} // namespace zug
