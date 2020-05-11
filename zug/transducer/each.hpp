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
#include <zug/util.hpp>

namespace zug {

/*!
 * Transducer that evaluates the function `action` on each input, forwarding the
 * input down the original inputs down the transducer chain.  The provided
 * function is used only for side-effects.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/each.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename ActionT>
constexpr auto each(ActionT&& action)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            compat::invoke(action, is...);
            return step(ZUG_FWD(s), ZUG_FWD(is)...);
        };
    });
}

} // namespace zug
