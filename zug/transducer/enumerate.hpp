//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/pipeable.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

template <typename T>
struct enumerate_t
{
    T initial;

    template <typename Step>
    auto operator()(Step&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            auto count = state_data(ZUG_FWD(s), constantly(initial));
            return wrap_state(
                step(
                    state_unwrap(ZUG_FWD(s)), std::move(count), ZUG_FWD(is)...),
                std::move(count) + static_cast<decltype(initial)>(1));
        };
    }
};

template <typename T>
constexpr auto enumerate_from(T&& initial)
{
    return make_pipeable(
        enumerate_t<std::decay_t<T>>{std::forward<T>(initial)});
}

ZUG_INLINE_CONSTEXPR auto enumerate = enumerate_from(std::size_t{});

} // namespace zug
