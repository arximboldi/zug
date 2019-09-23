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

struct partition_by_tag
{};

/*!
 * Similar to clojure.core/partition-by$1
 */
template <typename MappingT>
auto partition_by(MappingT&& mapping)
{
    return [=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            using container_t =
                std::vector<std::decay_t<decltype(tuplify(is...))>>;

            auto mapped = compat::invoke(mapping, ZUG_FWD(is)...);
            auto data   = state_data(ZUG_FWD(s), [&] {
                auto v = container_t{};
                return make_tuple(mapped, std::move(v), step);
            });

            auto& last_mapped = std::get<0>(data);
            auto& next_vector = std::get<1>(data);
            auto& next_step   = std::get<2>(data);

            const auto complete_group = mapped != last_mapped;

            auto next_state =
                complete_group
                    ? call(step, state_unwrap(ZUG_FWD(s)), next_vector)
                    : skip(step, state_unwrap(ZUG_FWD(s)), next_vector);

            if (complete_group)
                next_vector.clear();

            next_vector.push_back(tuplify(ZUG_FWD(is)...));

            return wrap_state<partition_by_tag>(
                std::move(next_state),
                make_tuple(std::move(mapped),
                           std::move(next_vector),
                           std::move(next_step)));
        };
    };
};

template <typename T>
auto state_wrapper_complete(partition_by_tag, T&& wrapper)
{
    auto next = std::get<1>(state_wrapper_data(std::forward<T>(wrapper)));
    auto step = std::get<2>(state_wrapper_data(std::forward<T>(wrapper)));
    return state_complete(
        next.empty()
            ? state_unwrap(std::forward<T>(wrapper))
            : step(state_unwrap(std::forward<T>(wrapper)), std::move(next)));
}

} // namespace zug
