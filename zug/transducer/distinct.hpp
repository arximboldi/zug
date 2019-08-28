//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/copy_traits.hpp>
#include <zug/detail/tuple_utils.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/with_state.hpp>

#include <unordered_set>

namespace zug {

constexpr auto distinct = [](auto&& step) {
    return [=](auto&& s, auto&&... is) mutable {
        using state_t = decltype(s);
        using cache_t =
            std::unordered_set<std::tuple<std::decay_t<decltype(is)>...>,
                               detail::tuple_hash>;
        using result_t =
            decltype(wrap_state(step(state_unwrap(s), is...), cache_t{}));
        using complete_t = decltype(state_complete(s));
        using wrapped_t = detail::copy_decay_t<state_t, std::decay_t<result_t>>;
        using unwrapped_t =
            detail::copy_decay_t<state_t, std::decay_t<complete_t>>;

        return with_state(
            ZUG_FWD(s),
            [&](unwrapped_t&& st) {
                return wrap_state(
                    step(state_unwrap(std::forward<unwrapped_t>(st)),
                         ZUG_FWD(is)...),
                    cache_t{{std::make_tuple(is...)}});
            },
            [&](wrapped_t st) {
                auto& cache = state_wrapper_data(st);
                auto unique = cache.emplace(is...).second;
                return !unique ? st
                               : wrap_state(step(std::move(state_unwrap(st)),
                                                 ZUG_FWD(is)...),
                                            std::move(cache));
            });
    };
};

} // namespace zug
