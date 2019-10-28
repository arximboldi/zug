//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/compat/invoke.hpp>
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
    static_assert(std::is_same<decltype(comp(comp(add_one, add_one), divide)),
                               detail::composed<decltype(add_one),
                                                decltype(add_one),
                                                decltype(divide)>>::value,
                  "comping a comp produces a flattened composed");

    auto result = comp(comp(add_one, add_one), divide)(12, 4);
    CHECK(result == 5);
}

TEST_CASE("comp: comp two comps")
{
    static_assert(
        std::is_same<
            decltype(comp(comp(add_one), comp(add_one))),
            detail::composed<decltype(add_one), decltype(add_one)>>::value,
        "comping two comps produces a flattened composed");

    auto result = comp(comp(add_one), comp(add_one))(10);
    CHECK(result == 12);
}

TEST_CASE("comp: comp an lvalue comp")
{
    auto comped = comp(add_one);
    auto result = comp(comped, comp(add_one))(10);
    CHECK(result == 12);
}

TEST_CASE("comp: comp a const lvalue comp")
{
    const auto comped = comp(add_one);
    auto result       = comp(comped, comped)(10);
    CHECK(result == 12);
}

auto c_add_one   = comp([](int x) { return x + 1; });
auto c_mult_five = comp([](int x) { return x * 5; });

TEST_CASE("operator|: call composed functions returns output of composition")
{
    auto result = (c_add_one | c_add_one)(10);
    CHECK(result == 12);
}

TEST_CASE("operator|: execution order is from right to left function")
{
    SECTION("add_one(mult_five(x))")
    {
        auto result = (c_add_one | c_mult_five)(10);
        CHECK(result == 51);
    }

    SECTION("mult_five(add_one(x))")
    {
        auto result = (c_mult_five | c_add_one)(10);
        CHECK(result == 55);
    }
}

TEST_CASE("operator|: composable non-composable type")
{
    auto composable     = comp([](auto x) { return x + 1; });
    auto non_composable = [](auto x) { return x + 1; };

    SECTION("composable | non-composable")
    {
        auto result = (composable | non_composable)(10);
        CHECK(result == 12);
    }

    SECTION("non-composable | composable")
    {
        auto result = (non_composable | composable)(10);
        CHECK(result == 12);
    }
}
