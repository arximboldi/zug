//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/tuplify.hpp>

namespace zug {

/*!
 * Reducing function that produces the tuplification of the last
 * inputs it received.
 *
 * @see tuplify
 */
constexpr auto last = [](auto&&, auto&&... ins) -> decltype(auto) {
    return tuplify(std::forward<decltype(ins)>(ins)...);
};

} // namespace zug
