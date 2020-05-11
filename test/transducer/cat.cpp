//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/cat.hpp>

#include <list>
#include <vector>

using namespace zug;

TEST_CASE("cat, cat")
{
    // example1 {
    auto v = std::vector<std::vector<int>>{{1, 2}, {3}, {4, 5, 6}};
    auto r = into_vector(cat, v);
    CHECK(r == (std::vector<int>{1, 2, 3, 4, 5, 6}));
    // }
}

TEST_CASE("cat, multiple")
{
    // example2 {
    auto v1 = std::vector<std::vector<int>>{{1, 2}, {3, 4}};
    auto v2 = std::vector<std::vector<char>>{{'a'}, {'c', 'd'}};
    auto r  = into_vector(cat, v1, v2);
    using t = std::vector<std::tuple<int, char>>;
    CHECK(r == (t{{1, 'a'}, {3, 'c'}, {4, 'd'}}));
    // }
}
