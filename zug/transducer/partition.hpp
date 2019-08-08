//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/reduce.hpp>
#include <zug/skip.hpp>
#include <zug/state_wrapper.hpp>

#include <vector>

namespace zug {

struct partition_tag
{};

template <typename... InputTs>
using partition_container_t =
    std::vector<std::decay_t<decltype(tuplify(std::declval<InputTs>()...))>>;

constexpr auto partition = [](auto size) {
    return [=](auto step) mutable {
        return [size, step = std::move(step)](auto&& s, auto&&... is) mutable {
            auto data         = state_data(ZUG_FWD(s), [&] {
                auto v = partition_container_t<decltype(is)...>{};
                v.reserve(size);
                return make_tuple(std::move(v), step);
            });
            auto& next_vector = std::get<0>(data);
            auto& next_step   = std::get<1>(data);

            next_vector.push_back(tuplify(ZUG_FWD(is)...));
            const auto complete_group = next_vector.size() == size;

            auto next_state =
                complete_group
                    ? call(step, state_unwrap(ZUG_FWD(s)), next_vector)
                    : skip(step, state_unwrap(ZUG_FWD(s)), next_vector);

            if (complete_group)
                next_vector.clear();

            return wrap_state<partition_tag>(
                std::move(next_state),
                std::make_tuple(std::move(next_vector), std::move(next_step)));
        };
    };
};

template <typename T>
auto state_wrapper_complete(partition_tag, T&& wrapper)
{
    auto next = std::get<0>(state_wrapper_data(std::forward<T>(wrapper)));
    auto step = std::get<1>(state_wrapper_data(std::forward<T>(wrapper)));
    return state_complete(
        next.empty()
            ? state_unwrap(std::forward<T>(wrapper))
            : step(state_unwrap(std::forward<T>(wrapper)), std::move(next)));
}

} // namespace zug
