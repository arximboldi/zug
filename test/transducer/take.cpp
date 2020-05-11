//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/compose.hpp>
#include <zug/into.hpp>
#include <zug/into_vector.hpp>
#include <zug/reducing/first.hpp>
#include <zug/transducer/cat.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/take.hpp>

#include "../spies.hpp"

#include <vector>

using namespace zug;

TEST_CASE("take, take")
{
    // example1 {
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto res = into_vector(take(3), v);
    CHECK(res == std::vector<int>{1, 2, 3});
    // }
}

TEST_CASE("take, take cat terminates early")
{
    auto v = std::vector<std::vector<int>>{{1, 2}, {3}, {4, 5, 6}};

    auto res = into(std::vector<int>{},
                    comp(cat,
                         map([](int x) {
                             CHECK(x < 5);
                             return x;
                         }),
                         take(4)),
                    v);
    CHECK(res == (std::vector<int>{1, 2, 3, 4}));
}

TEST_CASE("take, take stops early enough")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5, 6};

    auto res = into(std::vector<int>{},
                    comp(map([](int x) {
                             if (x > 4)
                                 throw std::runtime_error("bad!");
                             return x;
                         }),
                         take(3)),
                    v);
    CHECK(res == (std::vector<int>{1, 2, 3}));
}

TEST_CASE("take, take stops early enough2")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5, 6};

    auto res = into(std::vector<int>{},
                    comp(take(3), map([](int x) {
                             if (x > 4)
                                 throw std::runtime_error("bad!");
                             return x;
                         })),
                    v);
    CHECK(res == (std::vector<int>{1, 2, 3}));
}

TEST_CASE("take, take moves the state through")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto spy = reduce(take(5)(first), testing::copy_spy<>{}, v);
    CHECK(spy.copied.count() == 0);
}

TEST_CASE("take, take reduce nested finished")
{
    auto v = std::vector<std::vector<int>>{{1}, {2}, {3}};

    auto res = into(std::vector<int>{}, comp(cat, take(1)), v);
    CHECK(res == (std::vector<int>{1}));
}

TEST_CASE("take, take zero not supported")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5};

    CHECK_THROWS_AS(into(std::vector<int>{}, take(0), v),
                    detail::empty_transducer_error);
}
