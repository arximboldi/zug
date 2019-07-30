//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/copy_traits.hpp>
#include <zug/state_traits.hpp>

#include <iterator>
#include <utility>

namespace zug {
namespace detail {

template <typename ReducingFnT, typename StateT, typename InputRangeT>
auto reduce_nested_non_empty_non_variadic(ReducingFnT&& step,
                                          StateT&& initial,
                                          InputRangeT&& range)
    -> std::decay_t<decltype(step(initial, *std::begin(range)))>
{
    using input_t =
        copy_decay_t<InputRangeT,
                     std::remove_reference_t<decltype(*std::begin(range))>>;

    auto first = std::begin(range);
    auto last  = std::end(range);
    auto state =
        step(std::forward<StateT>(initial), std::forward<input_t>(*first));
    for (++first; !state_is_reduced(state) && first != last; ++first) {
        // `x = std::move(x)` is undefined behaviour, hence the two
        // steps approach to protect for when `step` just forwards
        // the state back.
        auto new_state = step(std::move(state), std::forward<input_t>(*first));
        state          = std::move(new_state);
    }
    return state;
}

} // namespace detail
} // namespace zug
