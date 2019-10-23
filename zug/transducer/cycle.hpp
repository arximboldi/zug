//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/empty_transducer_error.hpp>
#include <zug/detail/pipeable.hpp>
#include <zug/state_wrapper.hpp>

namespace zug {

struct cycle_tag
{};

/*!
 * Generator transducer produces the sequence passed as parameter, by
 * cycling over it.
 */
template <typename InputRangeT>
constexpr auto cycle(InputRangeT range)
{
    return make_pipeable([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            using std::get;
            using std::begin;
            using std::end;

            auto data = state_data(ZUG_FWD(s), [&] {
                auto first = begin(range);
                auto last  = end(range);
                if (first == last)
                    throw detail::empty_transducer_error{};
                return std::make_tuple(first, last);
            });

            if (get<0>(data) == get<1>(data))
                get<0>(data) = begin(range);

            return wrap_state<cycle_tag>(
                step(state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)..., *get<0>(data)++),
                std::move(data));
        };
    });
}

template <typename InputRangeT, typename... InputRangeTs>
constexpr auto cycle(InputRangeT&& r, InputRangeTs&&... rs)
{
    return comp(cycle(std::forward<InputRangeT>(r)),
                cycle(std::forward<InputRangeTs>(rs))...);
}

} // namespace zug
