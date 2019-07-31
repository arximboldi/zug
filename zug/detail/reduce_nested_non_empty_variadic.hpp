//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/copy_traits.hpp>
#include <zug/detail/tuple_utils.hpp>
#include <zug/meta/pack.hpp>
#include <zug/state_traits.hpp>
#include <zug/util.hpp>

#include <iterator>
#include <tuple>
#include <utility>

namespace zug {
namespace detail {

template <typename ReducingFnT,
          typename StateT,
          std::size_t... Indices,
          typename... InputTs,
          typename... InputRangeTs>
auto reduce_nested_non_empty_variadic_impl(std::index_sequence<Indices...>,
                                           meta::pack<InputTs...>,
                                           ReducingFnT&& step,
                                           StateT&& initial,
                                           InputRangeTs&&... ranges)
    -> std::decay_t<decltype(step(initial, *std::begin(ranges)...))>
{
    auto firsts = std::make_tuple(std::begin(ranges)...);
    auto lasts  = std::make_tuple(std::end(ranges)...);
    auto state  = step(std::forward<StateT>(initial),
                      std::forward<InputTs>(*std::get<Indices>(firsts))...);

    noop(++std::get<Indices>(firsts)...);
    while (!state_is_reduced(state) && detail::tuple_all_neq(firsts, lasts)) {
        auto new_state =
            step(std::move(state),
                 std::forward<InputTs>(*std::get<Indices>(firsts))...);
        state = std::move(new_state);
        noop(++std::get<Indices>(firsts)...);
    }

    return state;
}

template <typename ReducingFnT, typename StateT, typename... InputRangeTs>
decltype(auto) reduce_nested_non_empty_variadic(ReducingFnT&& step,
                                                StateT&& state,
                                                InputRangeTs&&... ranges)
{
    return reduce_nested_non_empty_variadic_impl(
        std::make_index_sequence<sizeof...(InputRangeTs)>{},
        meta::pack<copy_decay_t<
            InputRangeTs,
            std::remove_reference_t<decltype(*std::begin(ranges))>>...>{},
        std::forward<ReducingFnT>(step),
        std::forward<StateT>(state),
        std::forward<InputRangeTs>(ranges)...);
}

} // namespace detail
} // namespace zug
