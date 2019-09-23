//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <tuple>
#include <type_traits>

namespace zug {
namespace detail {

template <std::size_t Index, std::size_t Max>
struct tuple_all_neq_t
{
    template <typename Tuple1T, typename Tuple2T>
    bool operator()(Tuple1T&& t1, Tuple2T&& t2) const
    {
        return std::get<Index>(std::forward<Tuple1T>(t1)) !=
                   std::get<Index>(std::forward<Tuple2T>(t2)) &&
               tuple_all_neq_t<Index + 1, Max>{}(std::forward<Tuple1T>(t1),
                                                 std::forward<Tuple2T>(t2));
    }
};

template <std::size_t Max>
struct tuple_all_neq_t<Max, Max>
{
    template <typename Tuple1T, typename Tuple2T>
    bool operator()(Tuple1T&&, Tuple2T&&) const
    {
        return true;
    }
};

template <typename Tuple1T, typename Tuple2T>
bool tuple_all_neq(Tuple1T&& t1, Tuple2T&& t2)
{
    constexpr auto size1 = std::tuple_size<std::decay_t<Tuple1T>>{};
    constexpr auto size2 = std::tuple_size<std::decay_t<Tuple2T>>{};
    static_assert(size1 == size2, "");

    using impl_t = tuple_all_neq_t<0u, (size1 > size2 ? size2 : size1)>;
    return impl_t{}(std::forward<Tuple1T>(t1), std::forward<Tuple2T>(t2));
}

// Code from boost. Reciprocal of the golden ratio helps spread entropy and
// handles duplicates.  See Mike Seymour in magic-numbers-in-boosthash-combine:
// http://stackoverflow.com/questions/4948780
template <class T>
inline void hash_combine(std::size_t& seed, T const& v)
{
    seed ^= std::hash<T>()(v) + 0x9e3779b9 + (seed << 6) + (seed >> 2);
}

template <class Tuple, size_t Index = std::tuple_size<Tuple>::value - 1>
struct hash_tuple_impl
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        hash_tuple_impl<Tuple, Index - 1>::apply(seed, tuple);
        hash_combine(seed, std::get<Index>(tuple));
    }
};

template <class Tuple>
struct hash_tuple_impl<Tuple, 0>
{
    static void apply(size_t& seed, Tuple const& tuple)
    {
        hash_combine(seed, std::get<0>(tuple));
    }
};

struct tuple_hash
{
    template <typename... Ts>
    std::size_t operator()(const std::tuple<Ts...>& x) const noexcept
    {
        auto result = std::size_t{};
        hash_tuple_impl<std::tuple<Ts...>>::apply(result, x);
        return result;
    }
};

} // namespace detail
} // namespace zug
