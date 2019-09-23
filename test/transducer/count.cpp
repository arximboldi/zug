//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/count.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("count, count")
{
    using tup = std::tuple<int, std::size_t>;
    auto v    = std::vector<int>{13, 42, 5};

    auto res = into_vector(count(), v);
    CHECK(res == (std::vector<tup>{{tup(13, 0u), tup(42, 1u), tup(5, 2u)}}));
}

TEST_CASE("count, generator")
{
    auto res = into_vector(comp(count('a'), take(3)));
    CHECK(res == (std::vector<char>{{'a', 'b', 'c'}}));
}

TEST_CASE("count, stepped")
{
    auto res = into_vector(comp(count(1.0, 0.5), take(3)));
    CHECK(res == (std::vector<double>{{1.0, 1.5, 2.0}}));
}
