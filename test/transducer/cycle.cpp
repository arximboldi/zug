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
#include <zug/transducer/cycle.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("cycle, generator")
{
    // example1 {
    auto v1 = std::vector<int>{13, 42, 5, 6, 15};
    auto v2 = std::vector<int>{0, 1};
    auto r  = into_vector(cycle(v2), v1);
    using t = std::vector<std::tuple<int, int>>;
    CHECK(r == (t{{13, 0}, {42, 1}, {5, 0}, {6, 1}, {15, 0}}));
    // }
}

TEST_CASE("cycle, variadic")
{
    // example2 {
    auto v1 = std::vector<int>{0, 1};
    auto v2 = std::vector<std::string>{"one", "two", "three"};
    auto r  = into_vector(cycle(v1, v2) | take(5));
    using t = std::vector<std::tuple<int, std::string>>;
    CHECK(r == (t{{0, "one"}, //
                  {1, "two"},
                  {0, "three"},
                  {1, "one"},
                  {0, "two"}}));
    // }
}
