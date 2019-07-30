//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <iterator>

#include <zug/detail/reduce_nested_non_empty_nullary.hpp>
#include <zug/detail/reduce_nested_non_empty_variadic.hpp>

#if ZUG_REDUCE_TAIL_RECURSIVE
#include <zug/detail/reduce_nested_non_empty_tail_recursive.hpp>
#define ZUG_REDUCE_NESTED_NON_EMPTY_NON_VARIADIC_IMPL                          \
    ::zug::detail::reduce_nested_non_empty_tail_recursive

#elif ZUG_REDUCE_WITH_ACCUMULATE
#include <zug/detail/reduce_nested_non_empty_tail_accumulate.hpp>
#define ZUG_REDUCE_NESTED_NON_EMPTY_NON_VARIADIC_IMPL                          \
    ::zug::detail::reduce_nested_non_empty_accumulate

#elif ZUG_REDUCE_ALWAYS_VARIADIC
#include <zug/detail/reduce_nested_non_empty_variadic.hpp>
#define ZUG_REDUCE_NESTED_NON_EMPTY_NON_VARIADIC_IMPL                          \
    ::zug::detail::reduce_nested_non_empty_variadic

#else
#include <zug/detail/reduce_nested_non_empty_non_variadic.hpp>
#define ZUG_REDUCE_NESTED_NON_EMPTY_NON_VARIADIC_IMPL                          \
    ::zug::detail::reduce_nested_non_empty_non_variadic
#endif

namespace zug {
namespace detail {

template <typename ReducingFnT, typename StateT>
decltype(auto) reduce_nested_non_empty(ReducingFnT&& step, StateT&& state)
{
    return reduce_nested_non_empty_nullary(std::forward<ReducingFnT>(step),
                                           std::forward<StateT>(state));
}

template <typename ReducingFnT, typename StateT, typename InputRangeT>
decltype(auto)
reduce_nested_non_empty(ReducingFnT&& step, StateT&& state, InputRangeT&& range)
{
    return ZUG_REDUCE_NESTED_NON_EMPTY_NON_VARIADIC_IMPL(
        std::forward<ReducingFnT>(step),
        std::forward<StateT>(state),
        std::forward<InputRangeT>(range));
}

template <typename ReducingFnT,
          typename StateT,
          typename InputRangeT,
          typename... InputRangeTs>
decltype(auto) reduce_nested_non_empty(ReducingFnT&& step,
                                       StateT&& state,
                                       InputRangeT&& range,
                                       InputRangeTs&&... ranges)
{
    return reduce_nested_non_empty_variadic(
        std::forward<ReducingFnT>(step),
        std::forward<StateT>(state),
        std::forward<InputRangeT>(range),
        std::forward<InputRangeTs>(ranges)...);
}

} // namespace detail
} // namespace zug
