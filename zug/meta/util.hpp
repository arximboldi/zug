//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/unreachable.hpp>

#include <type_traits>

namespace zug {
namespace meta {

namespace detail {

template <typename... Ts>
struct make_void
{
    using type = void;
};

} // namespace detail

/*!
 * Similar to C++17 std::void_t
 */
template <typename... Ts>
using void_t = typename detail::make_void<Ts...>::type;

/*!
 * Identity metafunction.
 */
template <typename T>
struct identity
{
    using type = T;
};

/*!
 * This is a type that pretends to be convertible to anything.  This
 * can be used as a placeholder for any parameter type in `result_of`
 * metacalls.
 *
 * @note Only use in non-evaluated contexes, otherwise behavior is undefined.
 */
struct bottom
{
    template <typename T>
    operator T&&() const
    {
        ZUG_UNREACHABLE();
    }

    template <typename T>
    operator T&() const
    {
        ZUG_UNREACHABLE();
    }
    
    template <typename T>
    operator const T&() const
    {
        ZUG_UNREACHABLE();
    }
};

/*!
 * Type to enable making a type convertible from `void`.  The problem
 * is that `void` can not be used as a parameter, so it is imposible
 * to define a convertion between `void` and some types, in the
 * degenerate cases where it is desirable.
 *
 * @see could_not_find_common_type
 */
struct from_void
{};

namespace detail {

struct disabled
{};

} // namespace detail

/*!
 * Lazy enable if.
 */
template <bool C, typename T>
struct lazy_enable_if : std::conditional_t<C, T, detail::disabled>
{};

template <bool C, typename T>
using lazy_enable_if_t = typename lazy_enable_if<C, T>::type;

} // namespace meta
} // namespace zug
