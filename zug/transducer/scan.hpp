//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

struct scan_tag
{};

template <typename T>
bool state_wrapper_data_is_reduced(scan_tag, T&& data)
{
    return state_is_reduced(data);
}

/*!
 * For each input use the provided _reducing function_ to combine items into a
 * value that will be fed into the sequence. Inspired by
 * [rxcpp::operators::scan](http://reactivex.io/RxCpp/namespacerxcpp_1_1operators.html#adf46faf25436d38c72e243e14a875d06).
 *
 * The reducing function may use the state interface that this library provide,
 * allowing it to be combined with stateful transducers.  The state is passed
 * *unwrapped* into the sequence, so the state of the transducers is not visible
 * into the outter stream.  However, when the inner reduction completes, it
 * completes the outter reduction as well.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/scan.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/scan.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename State, typename ReducingFunction>
constexpr auto scan(State init, ReducingFunction rf)
{
    return comp([=](auto&& step) {
        return [=, rf = rf](auto&& s, auto&&... is) mutable {
            auto data = rf(state_data(ZUG_FWD(s), [&] { return init; }),
                           ZUG_FWD(is)...);
            auto next = step(state_unwrap(ZUG_FWD(s)), state_unwrap_all(data));
            return wrap_state<scan_tag>(std::move(next), std::move(data));
        };
    });
}

} // namespace zug
