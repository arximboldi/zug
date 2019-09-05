//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <type_traits>
#include <utility>

namespace zug {
namespace detail {

// Lambdas are not assignable, which is fucking annoying... it makes the whole
// notion of trating them as regular values very hard.  I really hate this and
// is making me consider going back to the transducer_impl approach from atria,
// which maybe is better...
template <typename T>
struct lambda_wrapper
{
    lambda_wrapper()                        = default;
    lambda_wrapper(const lambda_wrapper& x) = default;
    lambda_wrapper(lambda_wrapper&& x)      = default;
    lambda_wrapper(const T& x)
        : value_{x}
    {}
    lambda_wrapper(T&& x)
        : value_{std::move(x)}
    {}

    lambda_wrapper& operator=(const lambda_wrapper& x) = default;
    lambda_wrapper& operator=(lambda_wrapper&& x) noexcept
    {
        static_assert(std::is_nothrow_move_constructible<T>::value, "");
        value_.T::~T();
        new (&value_) T{std::move(x.value_)};
        return *this;
    }

    const T& operator*() const& { return value_; }
    T&& operator*() && { return std::move(value_); }
    T& operator*() & { return value_; }

    const T& get() const& { return value_; }
    T&& get() && { return std::move(value_); }
    T& get() & { return value_; }

    template <class... Ts>
    decltype(auto) operator()(Ts&&... xs) &
    {
        return value_(std::forward<Ts>(xs)...);
    }
    template <class... Ts>
    decltype(auto) operator()(Ts&&... xs) const&
    {
        return value_(std::forward<Ts>(xs)...);
    }
    template <class... Ts>
    decltype(auto) operator()(Ts&&... xs) &&
    {
        return value_(std::forward<Ts>(xs)...);
    }

private:
    T value_;
};

template <typename T>
lambda_wrapper<std::decay_t<T>> wrap_lambda(T&& t)
{
    return {std::forward<T>(t)};
}

} // namespace detail
} // namespace zug
