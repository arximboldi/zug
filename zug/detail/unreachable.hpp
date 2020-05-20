//
// zug: transducers for C++
// Copyright (C) 2020 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#if defined(_MSC_VER)
#define ZUG_UNREACHABLE() __assume(0)
#elif defined(__GNUC__)
#define ZUG_UNREACHABLE() __builtin_unreachable()
#else
#error "__builtin_unreachable or equivalent support required"
#define ZUG_UNREACHABLE()
#endif
