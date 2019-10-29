//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/inline_constexpr.hpp>
#include <zug/meta/dispatch.hpp>

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
