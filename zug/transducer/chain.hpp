//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/empty_transducer_error.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/with_state.hpp>

#include <zug/detail/copy_traits.hpp>

namespace zug {

struct chainr_tag
{};

/*!
 * Transducer produces the sequence passed as parameter after all
 * other input has finished.
 */
template <typename InputRange>
auto chainr(InputRange range)
{
    return [=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto data = state_data(
                ZUG_FWD(s), [&] { return std::make_pair(&step, &range); });
            return wrap_state<chainr_tag>(
                (*data.first)(state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...),
                std::move(data));
        };
    };
}

template <typename... InputRangeTs>
auto chainr(InputRangeTs... rs)
{
    return comp(chainr(std::forward<InputRangeTs>(rs))...);
}

template <typename... InputRangeTs>
auto chain(InputRangeTs&&... rs)
{
    return chainr(std::forward<InputRangeTs>(rs)...);
}

template <typename T>
decltype(auto) state_wrapper_complete(chainr_tag, T&& wrapper)
{
    return identity_(state_complete(
        reduce_nested(*state_wrapper_data(std::forward<T>(wrapper)).first,
                      state_unwrap(std::forward<T>(wrapper)),
                      *state_wrapper_data(std::forward<T>(wrapper)).second)));
}

/*!
 * Transducer produces the sequence passed as parameter before
 * processing the first input.
 */
template <typename InputRange>
auto chainl(InputRange range)
{
    return [=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    using std::begin;
                    using std::end;
                    return wrap_state(
                        begin(range) != end(range)
                            ? step(detail::reduce_nested_non_empty(
                                       step, ZUG_FWD(st), range),
                                   ZUG_FWD(is)...)
                            : step(ZUG_FWD(st), ZUG_FWD(is)...));
                },
                [&](auto&& st) {
                    return wrap_state(
                        step(state_unwrap(ZUG_FWD(st)), ZUG_FWD(is)...));
                });
        };
    };
}

template <typename... InputRangeTs>
auto chainl(InputRangeTs... rs)
{
    return comp(chainl(std::forward<InputRangeTs>(rs))...);
}

} // namespace zug
