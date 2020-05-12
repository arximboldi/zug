//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/compose.hpp>
#include <zug/into_vector.hpp>
#include <zug/transducer/count.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("count, count")
{
    // example1 {
    auto v  = std::vector<int>{13, 42, 5};
    auto r  = into_vector(count(), v);
    using t = std::vector<std::tuple<int, std::size_t>>;
    CHECK(r == (t{{13, 0u}, {42, 1u}, {5, 2u}}));
    // }
}

TEST_CASE("count, generator")
{
    auto res = into_vector(comp(count('a'), take(3)));
    CHECK(res == (std::vector<char>{{'a', 'b', 'c'}}));
}

TEST_CASE("count, stepped")
{
    // example2 {
    auto r = into_vector(count(1.0, 0.5) | take(5));
    CHECK(r == (std::vector<double>{{1.0, 1.5, 2.0, 2.5, 3.0}}));
    // }
}
