//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/invoke.hpp>

#define ZUG_FWD(x) std::forward<decltype(x)>(x)

namespace zug {

/*!
 * Does nothing.
 */
constexpr auto noop = [](auto&&...) {};

/*!
 * Similar to clojure.core/identity
 */
constexpr auto identity = [](auto&& x) -> decltype(auto) { return ZUG_FWD(x); };

/*!
 * Similar to @a identity, but it never returns a reference
 * to the pased in value.
 */
constexpr auto identity_ = [](auto&& x) { return ZUG_FWD(x); };

} // namespace zug
