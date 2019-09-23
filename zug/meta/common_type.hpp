//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta/pack.hpp>
#include <zug/meta/util.hpp>

#include <type_traits>
#include <utility>

namespace zug {
namespace meta {

/*!
 * Result of `CommonType` when no common type exists for types `Ts`
 */
template <typename... Ts>
struct could_not_find_common_type
{
    template <typename T>
    could_not_find_common_type(T&&)
    {}
    could_not_find_common_type(from_void&&) {}
};

namespace detail {

//!
// Removes r-value reference from type `ValT` if `T1` and `T2` is not an r-value
// reference.  This allows to remove unexpected r-references from a type that
// were added because of the use of `declval`.
//
template <typename ValT, typename T1, typename T2>
struct undeclval
    : std::conditional_t<std::is_rvalue_reference<ValT>::value &&
                             !std::is_rvalue_reference<T1>::value &&
                             !std::is_rvalue_reference<T2>::value,
                         std::remove_reference<ValT>,
                         identity<ValT>>
{};

template <typename T, typename U, typename Enable = void>
struct common_type_impl
{
    using type = could_not_find_common_type<T, U>;
};

template <typename T, typename U>
struct common_type_impl<
    T,
    U,
    void_t<decltype(true ? std::declval<T>() : std::declval<U>())>>
{
    using type = typename undeclval<decltype(true ? std::declval<T>()
                                                  : std::declval<U>()),
                                    T,
                                    U>::type;
};

} // namespace detail

/*!
 * Similar to `std::common_type` but addresses several issues.  First,
 * on Clang 3.4, `common_type` fails for `void`, where it should not.
 * Also, the standard common type removes qualification, which we want
 * to preserve. Also, `common_type` is SFINAE-friendly only in new
 * versions of GCC.
 *
 * This implementation preserves qualification when possible, and also
 * is total. When no common type is found, it returns the special type
 * `CouldNotFindCommonType`, which can be instantiated and converted
 * from anything.  This makes it easier to write functions that return
 * a common-type of other types, but that might be used in expressions
 * where the return type is to be discarded.  This erroneous type was
 * chosen instead of `void` to make debugging easier.
 */
template <typename... Types>
struct common_type;

template <>
struct common_type<>
{
    using type = could_not_find_common_type<>;
};

template <typename Acc>
struct common_type<Acc>
{
    using type = Acc;
};

template <typename Acc, typename Next, typename... Rest>
struct common_type<Acc, Next, Rest...>
    : common_type<typename detail::common_type_impl<Acc, Next>::type, Rest...>
{};

template <typename... Ts>
using common_type_t = typename common_type<Ts...>::type;

} // namespace meta
} // namespace zug
