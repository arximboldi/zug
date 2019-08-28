//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/iterator_range.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/state_wrapper.hpp>

#include <vector>

namespace zug {

struct eager_tag
{};

/*!
 * Similar to clojure.core/eager-all$1
 */
template <typename Mapping>
auto eager(Mapping mapping)
{
    return [=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            using container_t =
                std::vector<std::decay_t<decltype(tuplify(is...))>>;
            auto data = state_data(ZUG_FWD(s), [&] {
                return std::make_tuple(container_t{}, &step, &mapping);
            });
            std::get<0>(data).push_back(tuplify(ZUG_FWD(is)...));
            return wrap_state<eager_tag>(state_unwrap(ZUG_FWD(s)),
                                         std::move(data));
        };
    };
}

template <typename T>
decltype(auto) state_wrapper_complete(eager_tag, T&& wrapper)
{
    return identity_(state_complete(reduce_nested(
        *std::get<1>(state_wrapper_data(std::forward<T>(wrapper))),
        state_unwrap(std::forward<T>(wrapper)),
        (*std::get<2>(state_wrapper_data(std::forward<T>(wrapper))))(
            std::get<0>(state_wrapper_data(std::forward<T>(wrapper)))))));
}

/*!
 * Eager transducer that sorts the input sequence.
 * @see eager
 */
auto sorted = eager([](auto&& range) -> decltype(auto) {
    std::sort(range.begin(), range.end());
    return ZUG_FWD(range);
});

/*!
 * Eager transducer that reverses the input sequence.
 * @see eager
 */
auto reversed = eager([](auto&& range) -> decltype(auto) {
    return detail::make_iterator_range(range.rbegin(), range.rend());
});

} // namespace zug
