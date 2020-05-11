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
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

struct take_tag
{};

template <typename T>
bool state_wrapper_data_is_reduced(take_tag, T&& n)
{
    return std::forward<T>(n) <= 0;
}

/*!
 * Lets `n` elements through and finishes.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/take.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename IntegralT>
constexpr auto take(IntegralT n)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return wrap_state<take_tag>(
                step(state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...),
                state_data(ZUG_FWD(s), [=] {
                    if (n <= 0)
                        throw detail::empty_transducer_error{};
                    return n;
                }) - 1);
        };
    });
}

} // namespace zug
