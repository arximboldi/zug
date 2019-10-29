//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/compose.hpp>
#include <zug/skip.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Similar to clojure.core/drop-while$1
 */
template <typename PredicateT>
constexpr auto drop_while(PredicateT&& predicate)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto taking = state_data(ZUG_FWD(s), [] { return false; }) ||
                          !compat::invoke(predicate, is...);

            return wrap_state(
                taking ? call(step, state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...)
                       : skip(step, state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...),
                taking);
        };
    });
}

} // namespace zug
