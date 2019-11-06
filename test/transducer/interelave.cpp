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
#include <zug/transducer/interleave.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("interleave, interleave")
{
    auto v1 = std::vector<int>{{1, 2, 3}};
    auto v2 = std::vector<int>{{4, 5, 6}};

    auto res = into_vector(interleave, v1, v2);
    CHECK(res == (std::vector<int>{1, 4, 2, 5, 3, 6}));
}

TEST_CASE("interleave, interleave termineates early enough")
{
    auto v1 = std::vector<int>{{1, 2, 3}};
    auto v2 = std::vector<int>{{4, 5, 6}};

    auto res = into_vector(comp(interleave, take(3)), v1, v2);
    CHECK(res == (std::vector<int>{1, 4, 2}));
}
