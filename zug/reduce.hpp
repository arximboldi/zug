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
#include <zug/state_traits.hpp>

namespace zug {

/*!
 * Similar to
 * [std::accumulate](https://en.cppreference.com/w/cpp/algorithm/accumulate) and
 * [clojure.core/reduce](https://clojuredocs.org/clojure.core/reduce).
 *
 * Evaluates the *reducing function* `step` over every element of the input
 * `ranges`, passing as first argument always its last returned state.
 *
 * Unlike `std::accumulate`, this reduces over a range (doesn't take two
 * distinct iterators) and can reduce over several ranges at the same time.
 * Also note the different order of arguments.
 *
 * @rst
 *
 * Also, reduce supports *reducing functions* with the state interface described
 * in the :ref:`design` section. This means that the reducing function can wrap
 * the state in the first iteration, and the state can signal early termination.
 * This means, also, that this function can also take no input `range`.  In this
 * case, the reducing function will be evaluated indefinitelly, and the reducing
 * function may generates its own data until it eventually terminates.
 *
 * .. note:: This function uses `state_complete` before returning the final
 *    state, thus returning a state of type `StateT`. Use `reduce_nested` when
 *    this is not desirable.
 *
 * .. warning:: Reduce assumes that the ranges that are passed actually hold the
 *    values.  When the ranges are pased as *r-values*, the elements will be
 *    moved from the range into the reducing function.  Extra care has to be
 *    taken when using range adaptors or views---if the adapted container needs
 *    to be used after the reduction, make sure to pass the view as an
 *    *l-value*, by giving it a name.  Example:
 *
 *    .. code-block:: c++
 *
 *       using namespace boost::range::adaptors;
 *       auto x = std::vector<std::string>{ "foo", ... };
 *       reduce(..., x | reversed);
 *       std::cout << x[0] << std::endl; // CRASH!
 *
 *    To avoid this, you may give names to intermediate adaptors:
 *
 *    .. code-block:: c++
 *
 *       using namespace boost::range::adaptors;
 *       auto x = std::vector<std::string>{ "foo", ... };
 *       auto y = x | reversed;
 *       reduce(..., y);
 *       std::cout << x[0] << std::endl; // fine!
 *
 * @endrst
 */
template <typename ReducingFnT, typename StateT, typename... InputRangeTs>
auto reduce(ReducingFnT&& step, StateT&& state, InputRangeTs&&... ranges)
{
    return detail::is_non_empty(ranges...)
               ? state_complete(detail::reduce_nested_non_empty(
                     std::forward<ReducingFnT>(step),
                     std::forward<StateT>(state),
                     std::forward<InputRangeTs>(ranges)...))
               : std::forward<StateT>(state);
}

} // namespace zug
