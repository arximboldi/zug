//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/skip.hpp>
#include <zug/util.hpp>

namespace zug {

namespace detail {

/*!
 * Implementation of the most inner lambda body in `filter`
 *
 * @note `predicate` and `step` are passed by forwarding reference to preserve
 *       cv-qualification, they never become moved-from and can only be mutated
 *       if `operator()` mutates.
 */
template <typename PredicateT,
          typename ReducingFnT,
          typename StateT,
          typename... InputTs>
auto filter_step_dispatched(std::false_type,
                            PredicateT&& predicate,
                            ReducingFnT&& step,
                            StateT&& state,
                            InputTs&&... ins)
{
    return compat::invoke(predicate, ins...)
               ? call(step, ZUG_FWD(state), ZUG_FWD(ins)...)
               : skip(step, ZUG_FWD(state), ZUG_FWD(ins)...);
}

/*!
 * This overload exists only to query it's return type.
 */
template <typename PredicateT,
          typename ReducingFnT,
          typename StateT,
          typename... InputTs>
auto filter_step_dispatched(std::true_type,
                            PredicateT&&,
                            ReducingFnT&& step,
                            StateT&& state,
                            InputTs&&... ins)
{
    return call(step, ZUG_FWD(state), ZUG_FWD(ins)...);
}

} // namespace detail

/*!
 * Transducer that removes all inputs that do not pass the `predicate`.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/filter.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst

 */
template <typename PredicateT>
auto filter(PredicateT&& predicate)
{
    return comp([p = ZUG_FWD(predicate)](auto&& step) {
        return [p = std::move(p), step = ZUG_FWD(step)](auto&& s,
                                                        auto&&... is) mutable {
            return detail::filter_step_dispatched(
                std::is_same<std::decay_t<decltype(s)>, meta::bottom>{},
                p,
                step,
                ZUG_FWD(s),
                ZUG_FWD(is)...);
        };
    });
}

} // namespace zug
