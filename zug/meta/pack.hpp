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
namespace meta {

//! @defgroup meta
//! @{

/*!
 * MPL-compatible sequence that just holds a vector of types as a
 * paremeter pack.
 */
template <typename... Ts>
struct pack
{};

//! @}

/*!
 * Two packs are equal if they are of the same type.
 */
template <typename... Ts1, typename... Ts2>
constexpr bool operator==(const pack<Ts1...>&, const pack<Ts2...>&)
{
    return std::is_same<pack<Ts1...>, pack<Ts2...>>{};
}

/*!
 * Two packs are different if they are of different types.
 */
template <typename... Ts1, typename... Ts2>
constexpr bool operator!=(const pack<Ts1...>&, const pack<Ts2...>&)
{
    return !std::is_same<pack<Ts1...>, pack<Ts2...>>{};
}

namespace detail {

template <template <typename...> class MF, typename ArgT>
struct unpack
{
    using type = MF<ArgT>;
};

template <template <typename...> class MF, typename... ArgTs>
struct unpack<MF, meta::pack<ArgTs...>>
{
    using type = MF<ArgTs...>;
};

} // namespace detail

//! @defgroup meta
//! @{

/*!
 * Metafunction that given a variadic template `MF` and a type `ArgT`,
 * returns `MF<ArgT>`, or if ArgT is of the form `pack<Args...>` then
 * returns `MF<Args...>`.
 */
template <template <typename...> class MF, typename T>
using unpack = typename detail::unpack<MF, T>::type;

template <template <typename...> class MF, typename T>
using unpack_t = typename unpack<MF, T>::type;

//! @}

} // namespace meta
} // namespace zug

#if ZUG_ENABLE_BOOST

#include <boost/mpl/at_fwd.hpp>
#include <boost/mpl/back_fwd.hpp>
#include <boost/mpl/begin_end_fwd.hpp>
#include <boost/mpl/clear_fwd.hpp>
#include <boost/mpl/deref.hpp>
#include <boost/mpl/empty_fwd.hpp>
#include <boost/mpl/front_fwd.hpp>
#include <boost/mpl/iterator_tags.hpp>
#include <boost/mpl/next_prior.hpp>
#include <boost/mpl/pop_back_fwd.hpp>
#include <boost/mpl/pop_front_fwd.hpp>
#include <boost/mpl/push_back_fwd.hpp>
#include <boost/mpl/push_front_fwd.hpp>
#include <boost/mpl/sequence_tag.hpp>
#include <boost/mpl/size_fwd.hpp>

namespace zug {
namespace meta {

struct pack_tag;

template <class... Args>
struct pack_iterator;

template <class... Args>
struct pack_iterator<zug::meta::pack<Args...>>
{
    typedef zug::meta::pack_tag tag;
    typedef boost::mpl::forward_iterator_tag category;
};

} // namespace meta
} // namespace zug

namespace boost {
namespace mpl {

template <class... Args>
struct sequence_tag<zug::meta::pack<Args...>>
{
    typedef zug::meta::pack_tag type;
};

template <>
struct front_impl<zug::meta::pack_tag>
{
    template <typename PackT>
    struct apply;
    template <typename T, typename... Ts>
    struct apply<zug::meta::pack<T, Ts...>>
    {
        using type = T;
    };
};

template <>
struct empty_impl<zug::meta::pack_tag>
{
    template <typename PackT>
    struct apply;
    template <typename... Ts>
    struct apply<zug::meta::pack<Ts...>>
        : std::integral_constant<bool, sizeof...(Ts) == 0>
    {};
};

template <>
struct pop_front_impl<zug::meta::pack_tag>
{
    template <typename PackT>
    struct apply;
    template <class First, class... Types>
    struct apply<zug::meta::pack<First, Types...>>
    {
        typedef zug::meta::pack<Types...> type;
    };
};

template <>
struct push_front_impl<zug::meta::pack_tag>
{
    template <typename PackT, typename T>
    struct apply;
    template <typename T, typename... Args>
    struct apply<zug::meta::pack<Args...>, T>
    {
        typedef zug::meta::pack<T, Args...> type;
    };
};

template <>
struct push_back_impl<zug::meta::pack_tag>
{
    template <typename PackT, typename T>
    struct apply;
    template <typename T, typename... Args>
    struct apply<zug::meta::pack<Args...>, T>
    {
        typedef zug::meta::pack<Args..., T> type;
    };
};

template <>
struct size_impl<zug::meta::pack_tag>
{
    template <typename PackT>
    struct apply;
    template <typename... Ts>
    struct apply<zug::meta::pack<Ts...>>
        : std::integral_constant<std::size_t, sizeof...(Ts)>
    {};
};

template <>
struct at_impl<zug::meta::pack_tag>
{
    template <typename T>
    struct apply
    {
        static_assert(std::is_same<T, T>::value, "not implemented");
    };
};

template <>
struct back_impl<zug::meta::pack_tag>
{
    template <typename T>
    struct apply
    {
        static_assert(std::is_same<T, T>::value, "not implemented");
    };
};

template <>
struct clear_impl<zug::meta::pack_tag>
{
    template <typename PackT>
    struct apply
    {
        typedef zug::meta::pack<> type;
    };
};

template <>
struct pop_back_impl<zug::meta::pack_tag>
{
    template <typename T>
    struct apply
    {
        static_assert(std::is_same<T, T>::value, "not implemented");
    };
};

template <>
struct begin_impl<zug::meta::pack_tag>
{
    template <class PackT>
    struct apply
    {
        typedef zug::meta::pack_iterator<PackT> type;
    };
};

template <>
struct end_impl<zug::meta::pack_tag>
{
    template <typename>
    struct apply
    {
        typedef zug::meta::pack_iterator<zug::meta::pack<>> type;
    };
};

template <typename First, class... Args>
struct deref<zug::meta::pack_iterator<zug::meta::pack<First, Args...>>>
{
    typedef First type;
};

template <typename First, class... Args>
struct next<zug::meta::pack_iterator<zug::meta::pack<First, Args...>>>
{
    typedef zug::meta::pack_iterator<zug::meta::pack<Args...>> type;
};

} // namespace mpl
} // namespace boost

#endif
