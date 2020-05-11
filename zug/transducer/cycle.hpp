//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/detail/empty_transducer_error.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

struct cycle_tag
{};

template <typename InputRangeT>
constexpr auto cycle(InputRangeT&& range)
{
    return comp([=](auto&& step) {
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

/*!
 * Generator transducer produces the sequence passed as parameter, by
 * cycling over it.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/cycle.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/cycle.cpp
 *      :language: c++
 *      :start-after: // example2
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename... InputRangeTs>
constexpr auto cycle(InputRangeTs&&... rs)
{
    return comp(cycle(std::forward<InputRangeTs>(rs))...);
}

} // namespace zug
