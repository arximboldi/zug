//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/pipeable.hpp>
#include <zug/skip.hpp>
#include <zug/state_wrapper.hpp>

namespace zug {

struct iter_tag
{};

/*!
 * Generator transducer produces the sequence passed as parameter, by
 * iterating over it.
 */
template <typename InputRangeT>
auto iter(InputRangeT&& range)
{
    return make_pipeable([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            using std::cbegin;
            using std::cend;
            using std::get;
            auto data = state_data(ZUG_FWD(s), [&] {
                return std::make_tuple(cbegin(range), cend(range));
            });
            return get<0>(data) == get<1>(data)
                       ? wrap_state<iter_tag>(
                             skip_result_t<decltype(step),
                                           decltype(state_unwrap(ZUG_FWD(s))),
                                           decltype(ZUG_FWD(is))...,
                                           decltype(*get<0>(data))>(
                                 state_unwrap(ZUG_FWD(s))),
                             std::move(data))
                       : wrap_state<iter_tag>(call(step,
                                                   state_unwrap(ZUG_FWD(s)),
                                                   ZUG_FWD(is)...,
                                                   *get<0>(data)++),
                                              std::move(data));
        };
    });
}

template <typename T>
bool state_wrapper_data_is_reduced(iter_tag, T&& iters)
{
    using std::get;
    return get<0>(iters) == get<1>(iters);
}

auto iter() { return identity_; }

template <typename InputRangeT, typename... InputRangeTs>
constexpr auto iter(InputRangeT&& r, InputRangeTs&&... rs)
{
    return comp(iter(std::forward<InputRangeT>(r)),
                iter(std::forward<InputRangeTs>(rs))...);
}

} // namespace zug
