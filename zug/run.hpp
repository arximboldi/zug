//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/reducing/first.hpp>
#include <zug/transduce.hpp>

namespace zug {

namespace detail {

struct run_state_t
{};

} // namespace detail

/*!
 * Runs a transducer composed with no significant reduction over zero of more
 * ranges. It is useful to execute a transducer when we are only interested on
 * its side effects.
 */
template <typename XformT, typename... InputRangeTs>
void run(XformT&& xform, InputRangeTs&&... ranges)
{
    transduce(std::forward<XformT>(xform),
              first,
              detail::run_state_t{},
              std::forward<InputRangeTs>(ranges)...);
}

} // namespace zug
