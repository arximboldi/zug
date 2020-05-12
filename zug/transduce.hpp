//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/reduce.hpp>

namespace zug {

/*!
 * Similar to
 * [clojure.core/transduce](https://clojuredocs.org/clojure.core/transduce).
 *
 * Given a transducer `xform`, this is equivalent to: `reduce(xform(step),
 * state, ranges...)`
 */
template <typename XformT,
          typename ReducingFnT,
          typename StateT,
          typename... InputRangeTs>
auto transduce(XformT&& xform,
               ReducingFnT&& step,
               StateT&& state,
               InputRangeTs&&... ranges) -> std::decay_t<StateT>
{
    return reduce(xform(std::forward<ReducingFnT>(step)),
                  std::forward<StateT>(state),
                  std::forward<InputRangeTs>(ranges)...);
}

} // namespace zug
