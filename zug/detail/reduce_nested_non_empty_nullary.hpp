//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/state_traits.hpp>

namespace zug {
namespace detail {

template <typename ReducingFnT, typename StateT>
auto reduce_nested_non_empty_nullary(ReducingFnT&& step, StateT&& initial)
    -> std::decay_t<decltype(step(initial))>
{
    auto state = step(std::forward<StateT>(initial));
    while (!state_is_reduced(state)) {
        // `x = std::move(x)` is undefined behaviour, hence the two
        // steps approach to protect for when `step` just forwards
        // the state back.
        auto new_state = step(std::move(state));
        state          = std::move(new_state);
    }
    return state;
}

} // namespace detail
} // namespace zug
