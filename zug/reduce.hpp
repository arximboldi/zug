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
 * Similar to clojure.core/reduce.  Unlike `std::accumulate`, this
 * reduces over a range (doesn't take to distinct iterators) and can
 * reduce over several ranges at the same time.  It also supports
 * early termination for transducers.
 *
 * @note Reduce assumes that the ranges that are passed actually hold
 *       the values.  When the ranges are pased as r-values, the
 *       elements will be moved from the range into the reducing
 *       function.  Extra care has to be taken when using range
 *       adaptors or views -- if the adapted container needs to be
 *       used after the reduction, make sure to pass the view as an
 *       l-value, by giving it a name.  Example:
 *
 *       @code{.cpp}
 *           using namespace boost::range::adaptors;
 *           auto x = std::vector<std::string>{ "foo", ... };
 *           reduce(..., x | reversed);
 *           std::cout << x[0] << std::endl; // ooops!
 *       @endcode
 *
 *       One should instead:
 *
 *       @code{.cpp}
 *           using namespace boost::range::adaptors;
 *           auto x = std::vector<std::string>{ "foo", ... };
 *           auto y = x | reversed;
 *           reduce(..., y);
 *           std::cout << x[0] << std::endl; // ooops!
 *       @endcode
 */
template <typename ReducingFnT, typename StateT, typename... InputRangeTs>
auto reduce(ReducingFnT&& step, StateT&& state, InputRangeTs&&... ranges)
    -> std::decay_t<StateT>
{
    return detail::is_non_empty(ranges...)
               ? state_complete(detail::reduce_nested_non_empty(
                     std::forward<ReducingFnT>(step),
                     std::forward<StateT>(state),
                     std::forward<InputRangeTs>(ranges)...))
               : std::forward<StateT>(state);
}

} // namespace zug
