//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/eager.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/map.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("eager, into_vector")
{
    auto v = std::vector<int>{1, 2, 3, 4};
    auto r = into_vector(eager(identity), v);
    CHECK(r == (decltype(r){1, 2, 3, 4}));
}

TEST_CASE("eager, sorted")
{
    auto v      = std::vector<int>{6, 2, 1, 12, 3};
    auto times2 = [](int x) { return x * 2; };
    auto div3   = [](int x) { return x % 3 == 0; };

    auto r =
        into(std::vector<int>{}, comp(map(times2), sorted, filter(div3)), v);
    CHECK(r == (decltype(r){6, 12, 24}));
}

TEST_CASE("eager, reversed")
{
    auto v      = std::vector<int>{1, 2, 3, 6, 12};
    auto times2 = [](int x) { return x * 2; };
    auto div3   = [](int x) { return x % 3 == 0; };

    auto r =
        into(std::vector<int>{}, comp(map(times2), reversed, filter(div3)), v);
    CHECK(r == (decltype(r){24, 12, 6}));
}

TEST_CASE("eager, moves_data_around")
{
    using elem = testing::copy_spy<>;

    auto x      = elem{};
    auto v      = std::vector<elem>{x, x, x, x};
    auto copies = x.copied.count();

    into_vector(reversed, std::move(v));
    CHECK(x.copied.count() == copies);
}
