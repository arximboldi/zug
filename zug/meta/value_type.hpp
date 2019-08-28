//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta/detected.hpp>

namespace zug {
namespace meta {

namespace detail {

template <typename T>
using value_type_t = typename T::value_type;

template <typename T>
using iterator_value_t = typename std::iterator_traits<T>::value_type;

template <typename T>
using dereference_t = std::decay_t<decltype(*std::declval<T>())>;

template <typename T>
using getter_t = std::decay_t<decltype(std::declval<T>().get())>;

} // namespace detail

/*!
 * Utility metafunction for accessing an underlying *value type*.
 * This metafunction tries, in this order:
 *
 *   - Nested type: `T::value_type`
 *   - Iterator traits: `std::iterator_traits<T>::value_type`
 *   - Decayed dereference: `std::decay_t<decltype(*std::declval<T>())>`
 *   - Decayed getter: `std::decay_t<decltype(std::declval<T>().get())>`
 *
 * The concrete semantics of this underlying value type might
 * be context dependent.  It can be partially spetialized for
 * different types.
 */
template <typename T>
struct get_value_type
    : detected_any<T,
                   detail::value_type_t,
                   detail::iterator_value_t,
                   detail::dereference_t,
                   detail::getter_t>
{};

/*!
 * Convenient alias for `get_value_type`
 */
template <typename T>
using value_t = typename get_value_type<std::decay_t<T>>::type;

} // namespace meta
} // namespace zug
