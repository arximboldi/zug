//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

template <typename T>
constexpr auto enumerate_from(T&& initial)
{
    return [=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto count = state_data(ZUG_FWD(s), constantly(initial));
            return wrap_state(
                step(
                    state_unwrap(ZUG_FWD(s)), std::move(count), ZUG_FWD(is)...),
                std::move(count) + static_cast<decltype(initial)>(1));
        };
    };
}

ZUG_INLINE_CONSTEXPR auto enumerate = enumerate_from(std::size_t{});

} // namespace zug
