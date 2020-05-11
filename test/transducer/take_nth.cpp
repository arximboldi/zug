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
#include <zug/transducer/take.hpp>
#include <zug/transducer/take_nth.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("take_nth, into")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5};

    {
        auto res = into(std::vector<int>{}, take_nth(1), v);
        CHECK(res == (decltype(res){1, 2, 3, 4, 5}));
    }

    {
        auto res = into(std::vector<int>{}, take_nth(2), v);
        CHECK(res == (decltype(res){1, 3, 5}));
    }

    {
        auto res = into(std::vector<int>{}, take_nth(3), v);
        CHECK(res == (decltype(res){1, 4}));
    }
}

TEST_CASE("take_nth, example")
{
    // example1 {
    auto v   = std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
    auto res = into_vector(take_nth(3), v);
    CHECK(res == std::vector<int>{1, 4, 7, 10});
    // }
}

TEST_CASE("take_nth, compose")
{
    using namespace std::placeholders;
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto res = into_vector(comp(take_nth(2), take(2)), v);
    CHECK(res == (decltype(res){1, 3}));
}
