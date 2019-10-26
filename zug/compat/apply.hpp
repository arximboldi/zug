//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/meta/util.hpp>

#include <array>
#include <tuple>
#include <type_traits>
#include <utility>

namespace zug {
namespace compat {

namespace detail {

using std::get;

struct could_not_get_index_sequence
{};

template <typename T, typename I, I N, typename Enable = void>
struct get_integer_sequence_aux
{
    using type = std::conditional_t<(N == 0),
                                    could_not_get_index_sequence,
                                    std::make_integer_sequence<I, N>>;
};

template <typename T, typename I, I N>
struct get_integer_sequence_aux<
    T,
    I,
    N,
    meta::void_t<decltype(get<N>(std::declval<T>()))>>
    : get_integer_sequence_aux<T, I, N + 1>
{};

template <typename T, typename I>
struct get_integer_sequence : get_integer_sequence_aux<T, I, 0>
{};

template <typename... Ts, typename I>
struct get_integer_sequence<std::tuple<Ts...>, I>
{
    using type = std::make_integer_sequence<I, sizeof...(Ts)>;
};

template <typename T, typename U, typename I>
struct get_integer_sequence<std::pair<T, U>, I>
{
    using type = std::make_integer_sequence<I, 2>;
};

template <typename T, std::size_t N, typename I>
struct get_integer_sequence<std::array<T, N>, I>
{
    using type = std::make_integer_sequence<I, N>;
};

template <class F, class Tuple, std::size_t... I>
constexpr decltype(auto) apply_impl(F&& f, Tuple&& t, std::index_sequence<I...>)
{
    return invoke(std::forward<F>(f), std::get<I>(std::forward<Tuple>(t))...);
}

template <typename T, typename I>
using get_integer_sequence_t =
    typename get_integer_sequence<std::decay_t<T>, I>::type;

template <typename T>
using get_index_sequence_t = get_integer_sequence_t<T, std::size_t>;

} // namespace detail

template <class F, class Tuple>
constexpr decltype(auto) apply(F&& f, Tuple&& t)
{
    return detail::apply_impl(std::forward<F>(f),
                              std::forward<Tuple>(t),
                              detail::get_index_sequence_t<Tuple>{});
}

} // namespace compat
} // namespace zug
