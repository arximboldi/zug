//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into.hpp>
#include <zug/transducer/mapcat.hpp>

using namespace zug;

TEST_CASE("mapcat, mapcat")
{
    auto v = std::vector<std::vector<int>>{{1, 2}, {3}, {4, 5, 6}};

    auto res = into(std::vector<int>{}, mapcat([](int x) { return x * 2; }), v);
    CHECK(res == (std::vector<int>{2, 4, 6, 8, 10, 12}));
}
