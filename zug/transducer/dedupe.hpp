//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>
#include <zug/with_state.hpp>

#include <utility>

namespace zug {

struct dedupe_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    auto last = tuplify(is...);
                    return wrap_state(
                        step(state_unwrap(ZUG_FWD(st)), ZUG_FWD(is)...),
                        std::move(last));
                },
                [&](auto&& st) {
                    auto next = tuplify(is...);
                    auto dupe = next == state_wrapper_data(st);
                    return dupe ? ZUG_FWD(st)
                                : wrap_state(step(state_unwrap(ZUG_FWD(st)),
                                                  ZUG_FWD(is)...),
                                             std::move(next));
                });
        };
    }
};

/*!
 * Transducer that removes consecutive duplicates from a sequence.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/dedupe.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto dedupe = comp(dedupe_t{});

} // namespace zug
