//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/copy_traits.hpp>
#include <zug/detail/pipeable.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/with_state.hpp>

namespace zug {

struct dedupe_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    auto last = tuplify(is...);
                    return wrap_state(
                        step(state_unwrap(ZUG_FWD(st)), ZUG_FWD(is)...),
                        std::move(last));
                },
                [&](auto&& st) {
                    auto next = tuplify(is...);
                    auto dupe = next == state_wrapper_data(st);
                    return dupe ? ZUG_FWD(st)
                                : wrap_state(step(state_unwrap(ZUG_FWD(st)),
                                                  ZUG_FWD(is)...),
                                             std::move(next));
                });
        };
    }
};

ZUG_INLINE_CONSTEXPR auto dedupe = make_pipeable(dedupe_t{});

} // namespace zug
