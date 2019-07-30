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

/*!
 * Metafunction that given a metafunction `TraitCheckMf` to check
 * whether a type has a trait, and another `TraitAddMf` to add it to a
 * type, returns `TraitAddMf<DestT>` if `TraitCheckMf<OrigT>`, else
 * `OrigT`.
 */
template <template <typename> class TraitCheckMF,
          template <typename>
          class TraitAddMf,
          typename OrigT,
          typename DestT>
struct copy_trait
    : std::conditional<TraitCheckMF<OrigT>{},
                       typename TraitAddMf<DestT>::type,
                       DestT>
{};

#define ZUG_DETAIL_DEFINE_COPY_STD_TRAIT(name__)                               \
    template <typename OrigT, typename DestT>                                  \
    struct copy_##name__                                                       \
        : copy_trait<::std::is_##name__, ::std::add_##name__, OrigT, DestT>    \
    {};                                                                        \
    template <typename OrigT, typename DestT>                                  \
    using copy_##name__##_t = typename copy_##name__<OrigT, DestT>::type;      \
    /**/

ZUG_DETAIL_DEFINE_COPY_STD_TRAIT(lvalue_reference);
ZUG_DETAIL_DEFINE_COPY_STD_TRAIT(rvalue_reference);
ZUG_DETAIL_DEFINE_COPY_STD_TRAIT(volatile);
ZUG_DETAIL_DEFINE_COPY_STD_TRAIT(const);

template <typename OrigT, typename DestT>
struct copy_cv : copy_const<OrigT, copy_volatile_t<OrigT, DestT>>
{};
template <typename OrigT, typename DestT>
using copy_cv_t = typename copy_cv<OrigT, DestT>::type;

template <typename OrigT, typename DestT>
struct copy_reference
    : copy_lvalue_reference<OrigT, copy_rvalue_reference_t<OrigT, DestT>>
{};
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
