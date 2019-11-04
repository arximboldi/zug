//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente and Carl Bussey
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/detail/inline_constexpr.hpp>
#include <zug/meta/dispatch.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

namespace zug {
namespace detail {

template <std::size_t Index>
struct invoke_composition_impl
{
    template <typename Fns, typename... Args>
    static constexpr decltype(auto) apply(Fns&& fns, Args&&... args)
    {
        return invoke_composition_impl<Index - 1>::apply(
            std::forward<Fns>(fns),
            compat::invoke(std::get<Index>(std::forward<Fns>(fns)),
                           std::forward<Args>(args)...));
    }
};

template <>
struct invoke_composition_impl<0>
{
    template <typename Fns, typename... Args>
    static constexpr decltype(auto) apply(Fns&& fns, Args&&... args)
    {
        return compat::invoke(std::get<0>(std::forward<Fns>(fns)),
                              std::forward<Args>(args)...);
    }
};

template <typename Fns, typename... Args>
constexpr decltype(auto) invoke_composition(Fns&& fns, Args&&... args)
{
    constexpr auto Size = std::tuple_size<std::decay_t<Fns>>::value;
    return invoke_composition_impl<Size - 1>::apply(
        std::forward<Fns>(fns), std::forward<Args>(args)...);
}

struct pipeable
{};

} // namespace detail

template <typename Fn, typename... Fns>
struct composed
    : detail::pipeable
    , std::tuple<Fn, Fns...>
{
    using base_t = std::tuple<Fn, Fns...>;

    template <
        typename TupleFns,
        std::enable_if_t<
            !std::is_same<composed, std::decay_t<TupleFns>>::value>* = nullptr>
    constexpr composed(TupleFns&& fns)
        : base_t{std::forward<TupleFns>(fns)}
    {}

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) &
    {
        return detail::invoke_composition(as_tuple(), std::forward<T>(xs)...);
    }

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) const&
    {
        return detail::invoke_composition(as_tuple(), std::forward<T>(xs)...);
    }

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) &&
    {
        return detail::invoke_composition(std::move(*this).as_tuple(),
                                          std::forward<T>(xs)...);
    }

    base_t& as_tuple() & { return *this; }

    const base_t& as_tuple() const& { return *this; }

    base_t&& as_tuple() && { return std::move(*this); }
};

namespace detail {

template <typename T>
constexpr bool is_composed_v = false;

template <typename... Fns>
constexpr bool is_composed_v<composed<Fns...>> = true;

template <typename T, typename = void>
constexpr bool is_pipeable_impl_v = false;

template <typename T>
constexpr bool
    is_pipeable_impl_v<T,
                       std::enable_if_t<std::is_base_of<pipeable, T>::value>> =
        true;

template <typename T>
constexpr bool is_pipeable_v = detail::is_pipeable_impl_v<T>;

template <typename Composed,
          std::enable_if_t<is_composed_v<std::decay_t<Composed>>>* = nullptr>
decltype(auto) to_function_tuple(Composed&& c, meta::try_t)
{
    return std::forward<Composed>(c).as_tuple();
}

template <typename Fn>
auto to_function_tuple(Fn&& fn, meta::catch_t)
{
    return std::make_tuple(std::forward<Fn>(fn));
}

template <typename T>
decltype(auto) to_function_tuple(T&& t)
{
    return to_function_tuple(std::forward<T>(t), meta::try_t{});
}

template <typename T>
struct make_composed_result;

template <typename... Fns>
struct make_composed_result<std::tuple<Fns...>>
{
    using type = composed<Fns...>;
};

template <typename T>
using make_composed_result_t = typename make_composed_result<T>::type;

template <typename TupleFns>
auto make_composed(TupleFns&& fns)
    -> make_composed_result_t<std::decay_t<TupleFns>>
{
    return {std::forward<TupleFns>(fns)};
}

} // namespace detail

/*!
 * Right-to left function composition. Returns an object *g* that
 * composes all the given functions @f$ f_i @f$, such that
 * @f[
 *                g(x) = f_1(f_2(...f_n(x)))
 * @f]
 *
 * Functions are invoked via standard *INVOKE*, allowing to compose
 * function pointers, member functions, etc.
 */
template <typename F>
constexpr auto comp(F&& f) -> composed<std::decay_t<F>>
{
    return {std::forward<F>(f)};
}

template <typename Fn, typename... Fns>
constexpr auto comp(Fn&& f, Fns&&... fns)
{
    return detail::make_composed(
        std::tuple_cat(detail::to_function_tuple(std::forward<Fn>(f)),
                       detail::to_function_tuple(std::forward<Fns>(fns))...));
}

template <typename Lhs,
          typename Rhs,
          std::enable_if_t<detail::is_pipeable_v<std::decay_t<Lhs>> ||
                           detail::is_pipeable_v<std::decay_t<Rhs>>>* = nullptr>
constexpr auto operator|(Lhs&& lhs, Rhs&& rhs)
{
    return comp(std::forward<Lhs>(lhs), std::forward<Rhs>(rhs));
}

} // namespace zug
