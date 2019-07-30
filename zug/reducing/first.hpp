//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

namespace zug {

/*!
 * Reducing function that always returns the initial state.
 */
constexpr auto first = [](auto&& st, auto&&...) -> decltype(auto) {
    return ZUG_FWD(st);
};

} // namespace zug
