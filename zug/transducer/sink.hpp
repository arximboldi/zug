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
 * Transducer that evaluates `action` on each input.  The input is
 * forwarded into the action and discarded.  The next transducer is
 * excited for every input, but with no arguments.
 *
 * @note This is very similar to `each`, but can perform better
 *       since the arguments can be moved into the action.  If we are
 *       not interested in the inputs after the action, this one
 *       should be preferred.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/sink.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/sink.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst

 */
template <typename ActionT>
auto sink(ActionT&& action)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            compat::invoke(action, ZUG_FWD(is)...);
            return step(ZUG_FWD(s));
        };
    });
}

} // namespace zug
