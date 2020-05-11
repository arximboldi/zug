//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/detail/empty_transducer_error.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/util.hpp>

namespace zug {

namespace detail {

template <typename ReducingFnT,
          typename StateT,
          typename InputRangeT,
          typename... ValueTs>
auto reduce_nested_non_empty_product(ReducingFnT&& step,
                                     StateT&& initial,
                                     InputRangeT&& range,
                                     ValueTs&&... values)
    -> std::decay_t<decltype(step(initial, values..., *std::begin(range)))>
{
    auto first = std::begin(range);
    auto last  = std::end(range);
    auto state = step(std::forward<StateT>(initial), values..., *first);
    for (++first; !state_is_reduced(state) && first != last; ++first) {
        auto new_state = step(std::move(state), values..., *first);
        state          = std::move(new_state);
    }
    return state;
}

} // namespace detail

template <typename InputRangeT>
constexpr auto product(InputRangeT&& r)
{
    detail::check_non_empty(r);
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable -> decltype(auto) {
            return detail::reduce_nested_non_empty_product(
                step, ZUG_FWD(s), r, ZUG_FWD(is)...);
        };
    });
}

/*!
 * Transducer combines every element that passes by with every element in the
 * range that it takes as argument.  If multiple ranges are passed, it generates
 * all combinations of elements from all of them.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/product.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/product.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename InputRangeT1, typename... InputRangeTs>
constexpr auto product(InputRangeT1&& r1, InputRangeTs&&... rs)
{
    return comp(product(std::forward<InputRangeT1>(r1)),
                product(std::forward<InputRangeTs>(rs))...);
}

} // namespace zug
