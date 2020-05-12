//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <utility>

namespace zug {
namespace compat {

namespace detail {

#define ZUG_DETAIL_DECLTYPE_RETURN(body_expr)                                  \
    decltype(body_expr) { return (body_expr); }

template <class F, class... Args>
inline auto do_invoke(F&& f, Args&&... args) -> ZUG_DETAIL_DECLTYPE_RETURN(
    std::forward<F>(f)(std::forward<Args>(args)...));

template <class Base, class T, class Derived>
inline auto do_invoke(T Base::*pmd, Derived&& ref)
    -> ZUG_DETAIL_DECLTYPE_RETURN(std::forward<Derived>(ref).*pmd);

template <class PMD, class Pointer>
inline auto do_invoke(PMD pmd, Pointer&& ptr)
    -> ZUG_DETAIL_DECLTYPE_RETURN((*std::forward<Pointer>(ptr)).*pmd);

template <class Base, class T, class Derived, class... Args>
inline auto do_invoke(T Base::*pmf, Derived&& ref, Args&&... args)
    -> ZUG_DETAIL_DECLTYPE_RETURN((std::forward<Derived>(ref).*
                                   pmf)(std::forward<Args>(args)...));

template <class PMF, class Pointer, class... Args>
inline auto do_invoke(PMF pmf, Pointer&& ptr, Args&&... args)
    -> ZUG_DETAIL_DECLTYPE_RETURN(((*std::forward<Pointer>(ptr)).*
                                   pmf)(std::forward<Args>(args)...));

} // namespace detail

/*!
 * Like C++17 `std::invoke`
 */
template <class F, class... ArgTypes>
auto invoke(F&& f, ArgTypes&&... args)
    -> decltype(detail::do_invoke(std::forward<F>(f),
                                  std::forward<ArgTypes>(args)...))
{
    return detail::do_invoke(std::forward<F>(f),
                             std::forward<ArgTypes>(args)...);
}

} // namespace compat
} // namespace zug
