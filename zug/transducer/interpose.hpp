//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/apply.hpp>
#include <zug/detail/copy_traits.hpp>
#include <zug/detail/empty_transducer_error.hpp>
#include <zug/state_wrapper.hpp>
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

template <typename... ValueTs>
auto interpose(ValueTs&&... xs)
{
    // It seems GCC does not like capturing xs directly, causing it to
    // uninitialized usage problems, so we make a tuple out of it..
    return [value =
                std::make_tuple(std::forward<ValueTs>(xs)...)](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            using state_t  = decltype(s);
            using result_t = decltype(wrap_state(step(state_unwrap(s), is...)));
            using complete_t = decltype(state_complete(s));
            using wrapped_t =
                detail::copy_decay_t<state_t, std::decay_t<result_t>>;
            using unwrapped_t =
                detail::copy_decay_t<state_t, std::decay_t<complete_t>>;
            return with_state(
                ZUG_FWD(s),
                [&](unwrapped_t&& st) {
                    return wrap_state(
                        step(std::forward<unwrapped_t>(st), ZUG_FWD(is)...));
                },
                [&](wrapped_t&& st) {
                    // We can not use our implementation of std::apply included
                    // in zug/compat/apply.hpp because GCC crashes, lol.
                    auto next = detail::interp(
                        step,
                        state_unwrap(std::forward<wrapped_t>(st)),
                        value,
                        std::make_index_sequence<
                            std::tuple_size<decltype(value)>::value>{});
                    return wrap_state(
                        !state_is_reduced(next)
                            ? step(std::move(next), ZUG_FWD(is)...)
                            : std::move(next));
                });
        };
    };
}

} // namespace zug
