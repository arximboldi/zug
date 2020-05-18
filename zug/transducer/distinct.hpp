//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/detail/tuple_utils.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>
#include <zug/with_state.hpp>

#include <unordered_set>

namespace zug {

struct distinct_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            using cache_t =
                std::unordered_set<std::tuple<std::decay_t<decltype(is)>...>,
                                   detail::tuple_hash>;

            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    return wrap_state(
                        step(state_unwrap(ZUG_FWD(st)), ZUG_FWD(is)...),
                        cache_t{{std::make_tuple(is...)}});
                },
                [&](auto&& st) {
                    auto& cache = state_wrapper_data(st);
                    auto unique = cache.emplace(is...).second;
                    return !unique ? st
                                   : wrap_state(step(state_unwrap(ZUG_FWD(st)),
                                                     ZUG_FWD(is)...),
                                                std::move(cache));
                });
        };
    }
};

/*!
 * Transducer that removes duplicates from a sequence.
 *
 * Note that this transducer uses @f$ O(n) @f$ space (with @f$ n @f$ being the
 * size of the resulting sequence) since it needs to remember every element in
 * the sequence to mark whether it's been seen.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/distinct.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto distinct = comp(distinct_t{});

} // namespace zug
