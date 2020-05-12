//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/is_non_empty.hpp>
#include <zug/detail/reduce_nested_non_empty.hpp>
#include <zug/skip.hpp>

namespace zug {

/*!
 * Similar to `reduce`, but it does not unwrap the final state before returning
 * it.  This is useful when calling performing a reduction recursively within a
 * transducer.
 *
 * @see The implementation of `cat` or `eager` for an example transducer that
 *      uses `reduce_nested`.
 */
template <typename ReducingFnT, typename StateT, typename... InputRangeTs>
auto reduce_nested(ReducingFnT&& step, StateT&& state, InputRangeTs&&... ranges)
    -> decltype(auto)
{
    return detail::is_non_empty(ranges...)
               ? call(
                     [&](auto&& state) {
                         return detail::reduce_nested_non_empty(
                             std::forward<ReducingFnT>(step),
                             ZUG_FWD(state),
                             std::forward<InputRangeTs>(ranges)...);
                     },
                     std::forward<StateT>(state))
               : skip(std::forward<StateT>(state));
}

} // namespace zug
