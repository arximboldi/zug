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

struct take_tag
{};

template <typename T>
bool state_wrapper_data_is_reduced(take_tag, T&& n)
{
    return std::forward<T>(n) <= 0;
}

constexpr auto take = [](auto n) {
    return [=](auto step) {
        return [=](auto&& s, auto&&... is) {
            return wrap_state<take_tag>(
                step(state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...),
                state_data(ZUG_FWD(s), [=] { return n; }) - 1);
        };
    };
};

} // namespace zug
