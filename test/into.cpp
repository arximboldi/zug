//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/compose.hpp>
#include <zug/into.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/map.hpp>

#include <catch2/catch.hpp>

using namespace zug;

TEST_CASE("into, mutation")
{
    auto v   = std::vector<int>{1, 2, 3};
    auto res = std::vector<int>{};

    auto& res2 = into(res, identity, v);
    CHECK(res == (std::vector<int>{1, 2, 3}));
    CHECK(&res == &res2);
}

TEST_CASE("into, non mutation")
{
    auto v = std::vector<int>{1, 2, 3};

    auto res = into(std::vector<int>{}, identity, v);
    CHECK(res == (std::vector<int>{1, 2, 3}));
}

TEST_CASE("into, appends")
{
    auto v = std::vector<int>{1, 2, 3};

    auto res = into(std::vector<int>{0}, identity, v);
    CHECK(res == (std::vector<int>{0, 1, 2, 3}));
}

TEST_CASE("into, transduction")
{
    auto v = std::vector<int>{1, 2, 3, 4};

    auto res = into(std::vector<std::string>{},
                    comp(filter([](int x) { return x % 2 == 0; }),
                         map([](int x) { return std::to_string(x); })),
                    v);
    CHECK(res == (std::vector<std::string>{"2", "4"}));
}

TEST_CASE("into, zipping")
{
    auto v1 = std::vector<int>{1, 2, 3, 4};
    auto v2 = std::vector<std::string>{"a", "b"};

    using tup = std::tuple<int, std::string>;

    auto res = into(std::vector<tup>{}, identity, v1, v2);
    CHECK(res == (std::vector<tup>{tup(1, "a"), tup(2, "b")}));
}
