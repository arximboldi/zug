//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/pipeable.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/with_state.hpp>

namespace zug {

/*!
 * Similar to clojure.core/take_nth$1
 */
template <typename IntegralT>
auto take_nth(IntegralT nth)
{
    return make_pipeable([=](auto step) {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    return wrap_state(step(ZUG_FWD(st), ZUG_FWD(is)...), 1);
                },
                [&](auto&& st) {
                    auto count = state_wrapper_data(ZUG_FWD(st));
                    return count != nth
                               ? wrap_state(state_unwrap(ZUG_FWD(st)),
                                            count + 1)
                               : wrap_state(step(state_unwrap(ZUG_FWD(st)),
                                                 ZUG_FWD(is)...),
                                            1);
                });
        };
    });
};

} // namespace zug
