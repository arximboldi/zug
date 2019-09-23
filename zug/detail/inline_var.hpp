//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente (and Carl Bussey?)
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#if defined(__cpp_inline_variables)
#define DETAIL_ZUG_INLINE_CONSTEXPR inline constexpr
#else // defined(__cpp_inline_variables)
#if defined(_MSC_VER)
#define DETAIL_ZUG_INLINE_CONSTEXPR extern constexpr __declspec(selectany)
#elif defined(__GNUC__) // defined(_MSC_VER)
#define DETAIL_ZUG_INLINE_CONSTEXPR extern constexpr __attribute__((weak))
#else // defined(_MSC_VER)
#define DETAIL_ZUG_INLINE_CONSTEXPR constexpr /* not inline */
#endif // defined(_MSC_VER)
#endif // defined(__cpp_inline_variables)
