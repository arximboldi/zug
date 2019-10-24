//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/detail/inline_constexpr.hpp>

#include <tuple>
#include <type_traits>
#include <utility>

#define ZUG_FWD(x) std::forward<decltype(x)>(x)

namespace zug {

/*!
 * Does nothing.
 */
ZUG_INLINE_CONSTEXPR struct noop_t
{
    template <typename... T>
    void operator()(T&&...) const
    {}
} noop{};

/*!
 * Similar to clojure.core/identity
 */
ZUG_INLINE_CONSTEXPR struct identity_t
{
    template <typename T>
    decltype(auto) operator()(T&& x) const
    {
        return ZUG_FWD(x);
    };
} identity{};

/*!
 * Similar to @a identity, but it never returns a reference
 * to the pased in value.
 */
ZUG_INLINE_CONSTEXPR struct identity__t
{
    template <typename T>
    auto operator()(T&& x) const
    {
        return ZUG_FWD(x);
    };
} identity_{};

namespace detail {

template <std::size_t Index, std::size_t Last>
struct invoke_composition_impl
{
    template <typename Fns, typename... Args>
    static constexpr decltype(auto) apply(Fns&& fns, Args&&... args)
    {
        return compat::invoke(
            std::get<Index>(std::forward<Fns>(fns)),
            invoke_composition_impl<Index + 1, Last>::apply(
                std::forward<Fns>(fns), std::forward<Args>(args)...));
    }
};

template <std::size_t Last>
struct invoke_composition_impl<Last, Last>
{
    template <typename Fns, typename... Args>
    static constexpr decltype(auto) apply(Fns&& fns, Args&&... args)
    {
        return compat::invoke(std::get<Last>(std::forward<Fns>(fns)),
                              std::forward<Args>(args)...);
    }
};

template <typename Fns, typename... Args>
constexpr decltype(auto) invoke_composition(Fns&& fns, Args&&... args)
{
    return invoke_composition_impl<0,
                                   std::tuple_size<std::decay_t<Fns>>::value -
                                       1>::apply(std::forward<Fns>(fns),
                                                 std::forward<Args>(args)...);
}

template <typename Fn, typename... Fns>
struct composed
{
    std::tuple<Fn, Fns...> fns;

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) &
    {
        return invoke_composition(fns, std::forward<T>(xs)...);
    }

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) const&
    {
        return invoke_composition(fns, std::forward<T>(xs)...);
    }

    template <typename... T>
    constexpr decltype(auto) operator()(T&&... xs) &&
    {
        return invoke_composition(std::move(fns), std::forward<T>(xs)...);
    }
};

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
auto comp(F&& f) -> F&&
{
    return std::forward<F>(f);
}

template <typename Fn, typename... Fns>
auto comp(Fn&& f, Fns&&... fns)
    -> detail::composed<std::decay_t<Fn>, std::decay_t<Fns>...>
{
    return {std::make_tuple(std::forward<Fn>(f), std::forward<Fns>(fns)...)};
}

/*!
 * @see constantly
 */
template <typename T>
struct constantly_t
{
    T value;

    template <typename... ArgTs>
    auto operator()(ArgTs&&...) & -> T&
    {
        return value;
    }

    template <typename... ArgTs>
    auto operator()(ArgTs&&...) const& -> const T&
    {
        return value;
    }

    template <typename... ArgTs>
    auto operator()(ArgTs&&...) && -> T&&
    {
        return std::move(value);
    }
};

/*!
 * Similar to clojure.core/constantly
 */
template <typename T>
auto constantly(T&& value) -> constantly_t<std::decay_t<T>>
{
    return constantly_t<std::decay_t<T>>{std::forward<T>(value)};
}

} // namespace zug
