//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/transduce.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/take.hpp>
#include <zug/util.hpp>

#include <catch2/catch.hpp>

#include <vector>

using namespace zug;

TEST_CASE("transduce, identity")
{
    auto v = std::vector<int>{1, 2, 3, 6};
    CHECK(transduce(identity, std::plus<int>{}, 1, v) == 13);
}

TEST_CASE("transduce, variadic")
{
    auto v1 = std::vector<int>{1, 2, 3, 6};
    auto v2 = std::vector<int>{1, 2, 1, 2};

    CHECK(transduce(map(std::multiplies<int>{}), std::plus<int>{}, 1, v1, v2) ==
          21);
}

TEST_CASE("transduce, variadic_different_lengths")
{
    auto v1 = std::vector<int>{1, 2, 3, 6};
    auto v2 = std::vector<int>{1, 2, 1};

    CHECK(transduce(map(std::multiplies<int>{}), std::plus<int>{}, 1, v1, v2) ==
          9);
}

TEST_CASE("transduce, early termination does not leak")
{
    auto v1 = {1, 2, 3, 4};

    CHECK(transduce(take(2), std::plus<int>{}, 0, v1) == 3);
}
