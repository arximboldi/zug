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

/*!
 * Generator transducer version of Python `range`
 */
template <typename StopT>
constexpr decltype(auto) range(StopT&& stop)
{
    return count() | take(std::forward<StopT>(stop));
}

template <typename StartT, typename StopT>
constexpr decltype(auto) range(StartT&& start, StopT&& stop)
{
    return count(start) |
           take(std::max(decltype(stop - start){}, stop - start));
}

template <typename StartT, typename StopT, typename StepT>
constexpr decltype(auto) range(StartT&& start, StopT&& stop, StepT&& step)
{
    return count(start, step) | take(std::max(decltype((stop - start) / step){},
                                              (stop - start) / step));
}

} // namespace zug
