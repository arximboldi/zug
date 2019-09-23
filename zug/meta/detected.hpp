//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta/util.hpp>

#include <type_traits>

namespace zug {
namespace meta {

namespace detail {

template <class Default,
          class AlwaysVoid,
          template <class...>
          class Op,
          class... Args>
struct detector
{
    using value_t = std::false_type;
    using type    = Default;
};

template <class Default, template <class...> class Op, class... Args>
struct detector<Default, meta::void_t<Op<Args...>>, Op, Args...>
{
    using value_t = std::true_type;
    using type    = Op<Args...>;
};

} // namespace detail

/*!
 * Similar to TS Fundamentals 2 `std::nonesuch`
 */
struct nonesuch
{
    nonesuch()                = delete;
    ~nonesuch()               = delete;
    nonesuch(nonesuch const&) = delete;
    void operator=(nonesuch const&) = delete;
};

/*!
 * Similar to TS Fundamentals 2 `std::is_detected`
 */
template <template <class...> class Op, class... Args>
using is_detected =
    typename detail::detector<nonesuch, void, Op, Args...>::value_t;

/*!
 * Similar to TS Fundamentals 2 `std::detected_t`
 */
template <template <class...> class Op, class... Args>
using detected_t = typename detail::detector<nonesuch, void, Op, Args...>::type;

/*!
 * Similar to TS Fundamentals 2 `std::dected_or`
 */
template <class Default, template <class...> class Op, class... Args>
using detected_or = detail::detector<Default, void, Op, Args...>;

/*!
 * Similar to TS Fundamentals 2 `std::dected_or_t`
 */
template <class Default, template <class...> class Op, class... Args>
using detected_or_t = typename detected_or<Default, Op, Args...>::type;

/*!
 * Similar to TS Fundamentals 2 `std::is_detected_exact`
 */
template <class Expected, template <class...> class Op, class... Args>
using is_detected_exact = std::is_same<Expected, detected_t<Op, Args...>>;

/*!
 * Similar to TS Fundamentals 2 `std::is_detected_exact_v`
 */
template <class To, template <class...> class Op, class... Args>
using is_detected_convertible =
    std::is_convertible<detected_t<Op, Args...>, To>;

/*!
 * Similar to TS Fundamentals 2 `std::dected_or`
 */
template <template <class...> class Op, class... Args>
constexpr bool is_detected_v = is_detected<Op, Args...>::value;

/*!
 * Similar to TS Fundamentals 2 `std::is_detected_exact_v`
 */
template <class Expected, template <class...> class Op, class... Args>
constexpr bool is_detected_exact_v =
    is_detected_exact<Expected, Op, Args...>::value;

/*!
 * Similar to TS Fundamentals 2 `std::is_detected_convertible_v`
 */
template <class To, template <class...> class Op, class... Args>
constexpr bool is_detected_convertible_v =
    is_detected_convertible<To, Op, Args...>::value;

/*!
 * Like `estd::detected_or`, but evaluates `D` lazily.
 */
template <typename D, template <class...> class Op, typename T>
struct lazy_detected_or
{
    template <typename X>
    using operation_t = identity<Op<X>>;
    using type        = typename meta::detected_or_t<D, operation_t, T>::type;
};

template <typename D, template <class...> class Op, typename T>
using lazy_detected_or_t = typename lazy_detected_or<D, Op, T>::type;

/*!
 * Metafunction that returns the first type that is detected via the
 * metafunctions in `Ops...`
 */
template <typename T, template <class...> class... Ops>
struct detected_any;

template <typename T, template <class...> class Op>
struct detected_any<T, Op>
{
    using type = meta::detected_t<Op, T>;
};

template <typename T,
          template <class...>
          class Op,
          template <class...>
          class... Ops>
struct detected_any<T, Op, Ops...>
{
    using type = lazy_detected_or_t<detected_any<T, Ops...>, Op, T>;
};

template <typename T, template <class...> class... Ops>
using detected_any_t = typename detected_any<T, Ops...>::type;

} // namespace meta
} // namespace zug
