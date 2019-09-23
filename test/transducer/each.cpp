//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/each.hpp>

using namespace zug;

TEST_CASE("each, each")
{
    auto v  = std::vector<int>{1, 2, 3, 6};
    auto r1 = std::vector<int>{};
    auto r2 = into_vector(each([&](int x) { r1.push_back(x); }), v);
    CHECK(v == r1);
    CHECK(v == r2);
}
