//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <numeric>
#include <type_traits>

namespace zug {
namespace detail {

template <typename ReducingFnT, typename StateT, typename InputRangeT>
auto reduce_nested_non_empty_accumulate(ReducingFnT&& step,
                                        StateT&& state,
                                        InputRangeT&& range)
    -> std::decay_t<StateT>
{
    return std::accumulate(std::begin(range),
                           std::end(range),
                           std::forward<StateT>(state),
                           std::forward<ReducingFnT>(step));
}

} // namespace detail
} // namespace zug
