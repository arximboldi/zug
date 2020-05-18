//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/apply.hpp>
#include <zug/compose.hpp>
#include <zug/detail/empty_transducer_error.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>
#include <zug/with_state.hpp>

namespace zug {

namespace detail {

template <typename Step, typename State, typename Value, std::size_t... I>
decltype(auto)
interp(Step&& step, State&& st, Value&& value, std::index_sequence<I...>)
{
    return std::forward<Step>(step)(std::forward<State>(st),
                                    std::get<I>(std::forward<Value>(value))...);
}

} // namespace detail

/*!
 * Feeds the values passed as arguments in between the elements of the stream.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/interpose.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename... ValueTs>
auto interpose(ValueTs&&... xs)
{
    // It seems GCC does not like capturing xs directly, causing it to
    // uninitialized usage problems, so we make a tuple out of it..
    return comp(
        [value = std::make_tuple(std::forward<ValueTs>(xs)...)](auto&& step) {
            return [=](auto&& s, auto&&... is) mutable {
                return with_state(
                    ZUG_FWD(s),
                    [&](auto&& st) {
                        return wrap_state(step(ZUG_FWD(st), ZUG_FWD(is)...));
                    },
                    [&](auto&& st) {
                        // We can not use our implementation of std::apply
                        // included in zug/compat/apply.hpp because GCC crashes,
                        // lol.
                        auto next = detail::interp(
                            step,
                            state_unwrap(ZUG_FWD(st)),
                            value,
                            std::make_index_sequence<
                                std::tuple_size<decltype(value)>::value>{});
                        return wrap_state(
                            !state_is_reduced(next)
                                ? step(std::move(next), ZUG_FWD(is)...)
                                : std::move(next));
                    });
            };
        });
}

} // namespace zug
