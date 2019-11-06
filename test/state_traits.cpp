//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/any_state.hpp>
#include <zug/compose.hpp>
#include <zug/reducing/last.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/transducer/enumerate.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/take.hpp>
#include <zug/transducer/transducer.hpp>
#include <zug/util.hpp>

#include <catch2/catch.hpp>

using namespace zug;

TEST_CASE("state_traits, unwrap all wrap state")
{
    CHECK(state_unwrap_all(42) == 42);
    CHECK(state_unwrap_all(wrap_state(42, {})) == 42);
    CHECK(state_unwrap_all(wrap_state(wrap_state(42, {}), "")) == 42);
}

TEST_CASE("state_traits, unwrap all skip state")
{
    auto odd = [](int x) { return x % 2; };
    auto rf  = comp(filter(odd), take(10))(last);

    auto s = rf(int{}, 41);
    CHECK(state_unwrap_all(s) == 41);
    s = rf(s, 13);
    CHECK(state_unwrap_all(s) == 13);
}

TEST_CASE("state_traits, unwrap all type erased")
{
    auto rf = transducer<int>{identity}(last);

    auto s = rf(int{}, 41);
    CHECK(state_unwrap_all(s) == 41);
    s = rf(s, 13);
    CHECK(state_unwrap_all(s) == 13);
}

TEST_CASE("state_traits, unwrap all complex")
{
    auto odd = [](int x) { return x % 2; };
    auto rf  = transducer<int>{comp(filter(odd), take(10))}(last);

    auto s = rf(int{}, 41);
    CHECK(state_unwrap_all(s) == 41);
    s = rf(s, 13);
    CHECK(state_unwrap_all(s) == 13);
}

TEST_CASE("state_traits, unwrap all moar complex")
{
    auto odd = [](std::size_t x) { return x % 2; };
    auto rf  = transducer<meta::pack<>, std::size_t>{
        comp(enumerate, filter(odd), take(10))}(last);

    auto s = rf(std::size_t{});
    CHECK(state_unwrap_all(s) == 0);
    s = rf(s);
    CHECK(state_unwrap_all(s) == 1);
    s = rf(s);
    CHECK(state_unwrap_all(s) == 1);
    s = rf(s);
    CHECK(state_unwrap_all(s) == 3);
}

TEST_CASE("state_traits, rewrap wrap state")
{
    struct t1 : meta::pack<t1>
    {};
    struct t2 : meta::pack<t2>
    {};
    CHECK(state_rewrap(42, 13) == 13);
    CHECK(state_rewrap(wrap_state<t1>(42, {}), 13) == wrap_state<t1>(13, {}));
    CHECK(state_rewrap(wrap_state<t2>(wrap_state<t1>(42, {}), ""), 13) ==
          wrap_state<t2>(wrap_state<t1>(13, {}), ""));
}

TEST_CASE("state_traits, rewrap skip state")
{
    auto odd = [](int x) { return x % 2; };
    auto rf  = comp(filter(odd), take(3))(last);

    auto s = rf(int{}, 41);
    s      = state_rewrap(s, 13);
    CHECK(state_unwrap_all(s) == 13);
    s = rf(rf(rf(s, 1), 2), 3);
    CHECK(state_unwrap_all(s) == 3);
    CHECK(state_is_reduced(s));
}

TEST_CASE("state_traits, rewrap type erased")
{
    auto rf = transducer<int>{identity}(last);

    auto s = rf(int{}, 41);
    s      = state_rewrap(s, 13);
    CHECK(state_unwrap_all(s) == 13);
}

TEST_CASE("state_traits, rewrap moar complex")
{
    auto odd = [](std::size_t x) { return x % 2; };
    auto rf  = transducer<meta::pack<>, std::size_t>{
        comp(enumerate, filter(odd), take(10))}(last);

    auto s = rf(std::size_t{});
    s      = rf(s);
    s      = state_rewrap(s, std::size_t{13});
    CHECK(state_unwrap_all(s) == 13);
    s = rf(s);
    CHECK(state_unwrap_all(s) == 13);
    s = rf(s);
    CHECK(state_unwrap_all(s) == 3);
}
