//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/copy_traits.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/with_state.hpp>

namespace zug {

constexpr auto dedupe = [](auto&& step) {
    return [=](auto&& s, auto&&... is) mutable {
        using result_t =
            decltype(wrap_state(step(state_unwrap(s), is...), tuplify(is...)));
        using complete_t = decltype(state_complete(s));
        using state_t    = decltype(s);
        using wrapped_t = detail::copy_decay_t<state_t, std::decay_t<result_t>>;
        using unwrapped_t =
            detail::copy_decay_t<state_t, std::decay_t<complete_t>>;

        return with_state(
            ZUG_FWD(s),
            [&](unwrapped_t&& st) {
                auto last = tuplify(is...);
                return wrap_state(
                    step(state_unwrap(std::forward<unwrapped_t>(st)),
                         ZUG_FWD(is)...),
                    std::move(last));
            },
            [&](wrapped_t&& st) {
                auto next = tuplify(is...);
                auto dupe = next == state_wrapper_data(st);
                return dupe
                           ? std::forward<wrapped_t>(st)
                           : wrap_state(
                                 step(state_unwrap(std::forward<wrapped_t>(st)),
                                      ZUG_FWD(is)...),
                                 std::move(next));
            });
    };
};

} // namespace zug
