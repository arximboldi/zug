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

template <class F, class G>
struct composed
{
    F f;
    G g;

    template <class... T>
    decltype(auto) operator()(T&&... xs) &
    {
        return invoke(f, invoke(g, std::forward<T>(xs)...));
    }
    template <class... T>
    decltype(auto) operator()(T&&... xs) const&
    {
        return invoke(f, invoke(g, std::forward<T>(xs)...));
    }
    template <class... T>
    decltype(auto) operator()(T&&... xs) &&
    {
        return invoke(std::move(f), invoke(g, std::forward<T>(xs)...));
    }
};

template <typename... Fns>
struct get_composed;

template <typename... Ts>
using get_composed_t = typename get_composed<Ts...>::type;

template <typename F>
struct get_composed<F>
{
    using type = F;
};

template <typename F, typename... Fs>
struct get_composed<F, Fs...>
{
    using type = composed<F, get_composed_t<Fs...>>;
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
    -> detail::get_composed_t<std::decay_t<Fn>, std::decay_t<Fns>...>
{
    using result_t =
        detail::get_composed_t<std::decay_t<Fn>, std::decay_t<Fns>...>;
    return result_t{std::forward<Fn>(f), comp(std::forward<Fns>(fns)...)};
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
