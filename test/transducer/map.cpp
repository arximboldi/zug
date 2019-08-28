//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/into.hpp>
#include <zug/reducing/first.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/take.hpp>

#include <catch2/catch.hpp>

using namespace zug;

TEST_CASE("map, mapping")
{
    auto v      = std::vector<int>{1, 2, 3, 6};
    auto times2 = [](int x) { return x * 2; };
    CHECK(transduce(map(times2), std::plus<int>{}, 1, v) == 25);
}

namespace {

int free_times2(int x) { return x * 2; }

} // anonymous namespace

TEST_CASE("map, mapping invoke")
{
    auto v = std::vector<int>{1, 2, 3, 6};
    CHECK(transduce(map(&free_times2), std::plus<int>{}, 1, v) == 25);
}
