//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/compose.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/remove.hpp>
#include <zug/util.hpp>

using namespace zug;

TEST_CASE("remove, simple")
{
    auto v      = std::vector<int>{1, 2, 3, 6};
    auto times2 = [](int x) { return x * 2; };
    auto odd    = [](int x) { return x % 2 == 0; };

    auto res =
        transduce(comp(remove(odd), map(times2)), std::plus<int>{}, 1, v);
    CHECK(res == 9);
}
