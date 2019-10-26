//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/util.hpp>

#include <catch2/catch.hpp>

#include <array>
#include <functional>
#include <type_traits>

using namespace zug;

auto add_one   = [](int x) { return x + 1; };
auto mult_five = [](int x) { return x * 5; };
auto fst       = [](auto&& x) { return x[0]; };
auto divide    = [](int x, int y) { return x / y; };

TEST_CASE("comp: call composed functions returns output of composition")
{
    auto result = comp(add_one, add_one, add_one)(5);
    CHECK(result == 8);
}

TEST_CASE("comp: execution order is from right to left function")
{
    SECTION("add_one(mult_five(x))")
    {
        auto result = comp(add_one, mult_five)(10);
        CHECK(result == 51);
    }

    SECTION("mult_five(add_one(x))")
    {
        auto result = comp(mult_five, add_one)(10);
        CHECK(result == 55);
    }
}

TEST_CASE("comp: supports transformation between input and output type")
{
    static_assert(
        std::is_same<
            std::result_of_t<decltype(comp(add_one, fst))(std::array<int, 4>)>,
            int>::value,
        "comp should support transformation from array to int");

    auto result = comp(add_one, fst)(std::array<int, 4>{1, 2, 3, 4});
    CHECK(result == 2);
}

TEST_CASE("comp: supports multiple input arguments")
{
    auto result = comp(add_one, divide)(10, 2);
    CHECK(result == 6);
}

TEST_CASE("comp: supports single function with multiple args")
{
    auto result = comp(divide)(10, 2);
    CHECK(result == 5);
}

TEST_CASE("comp: comp a comp")
{
    auto result = comp(comp(add_one, add_one), divide)(12, 4);
    CHECK(result == 5);
}

TEST_CASE("comp: comp two comps")
{
    auto result = comp(comp(add_one), comp(add_one))(10);
    CHECK(result == 12);
}
