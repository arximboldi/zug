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

#include <type_traits>
#include <utility>

namespace zug {

template <typename T>
struct enumerate_from_t
{
    template <typename Step>
    constexpr auto operator()(Step&& step) const
    {
        return [initial = initial, step = std::forward<Step>(step)](
                   auto&& s, auto&&... is) mutable {
            auto count = state_data(ZUG_FWD(s), constantly(initial));
            return wrap_state(step(state_unwrap(ZUG_FWD(s)),
                                   std::move(count),
                                   ZUG_FWD(is)...),
                              std::move(count) + static_cast<T>(1));
        };
    }

    T initial;
};

/*!
 * Enumerates elements in the stream, by prepending the index at in which they
 * appear, starting from `initial`.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/enumerate.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename T>
constexpr auto enumerate_from(T&& initial)
{
    return comp(enumerate_from_t<std::decay_t<T>>{std::forward<T>(initial)});
}

/*!
 * Enumerates elements in the stream, by prepending the index at in which they
 * appear, starting from `initial`.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/enumerate.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto enumerate = enumerate_from(std::size_t{});

} // namespace zug
