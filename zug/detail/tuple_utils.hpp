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
    bool operator()(Tuple1T&& t1, Tuple2T&& t2)
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
    bool operator()(Tuple1T&&, Tuple2T&&)
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

} // namespace detail
} // namespace zug
