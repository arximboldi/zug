//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <type_traits>

namespace zug {
namespace detail {

template <typename OrigT, typename DestT>
struct copy_cv {
    using type = DestT;
};

template <typename OrigT, typename DestT>
struct copy_cv<volatile OrigT, DestT> {
    using type = volatile DestT;
};

template <typename OrigT, typename DestT>
struct copy_cv<const OrigT, DestT> {
    using type = const DestT;
};

template <typename OrigT, typename DestT>
struct copy_cv<const volatile OrigT, DestT> {
    using type = const volatile DestT;
};

template <typename OrigT, typename DestT>
using copy_cv_t = typename copy_cv<OrigT, DestT>::type;

template <typename OrigT, typename DestT>
struct copy_reference {
    using type = DestT;
};

template <typename OrigT, typename DestT>
struct copy_reference<OrigT&, DestT> {
    using type = DestT&;
};

template <typename OrigT, typename DestT>
struct copy_reference<OrigT&&, DestT> {
    using type = DestT&&;
};

template <typename OrigT, typename DestT>
using copy_reference_t = typename copy_reference<OrigT, DestT>::type;

/*!
 * Adds reference and cv-qualifications from `OrigT` to `DestT`.  In
 * spite of the name, `is_same<copy_decay<T, decay_t<T>>, T>::value`
 * is not true for every possible `T`, since rules about extent and
 * function decay can not be unambiguously inversed.
 */
template <typename OrigT, typename DestT>
struct copy_decay
    : copy_reference<OrigT, copy_cv_t<std::remove_reference_t<OrigT>, DestT>>
{};
template <typename OrigT, typename DestT>
using copy_decay_t = typename copy_decay<OrigT, DestT>::type;

} // namespace detail
} // namespace zug
