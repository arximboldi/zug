//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente (and Carl Bussey, maybe?)
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/inline_constexpr.hpp>

namespace zug {
namespace detail {

struct to_be_inlined
{};

ZUG_INLINE_CONSTEXPR auto inline_var = to_be_inlined{};

} // namespace detail
} // namespace zug
