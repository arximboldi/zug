//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/reduce.hpp>
#include <zug/reducing/first_rf.hpp>
#include <zug/reducing/last_rf.hpp>
#include <zug/state_wrapper.hpp>

#include <functional>

#include "spies.hpp"

#include <catch2/catch.hpp>

TEST_CASE("reduce protects against moved self assignment")
{
    auto v = std::vector<int>{1, 2, 3, 6};
    CHECK(zug::reduce(zug::first_rf, v, v) == v);
}

namespace {

struct non_default
{
    non_default() = delete;
    explicit non_default(int) {}
    non_default(const non_default&) = default;
    non_default(non_default&&)      = default;
    non_default& operator=(const non_default&) = default;
    non_default& operator=(non_default&&) = default;
};

struct foo_tag
{};

constexpr auto foo = [](auto step) {
    return [=](auto&& s, auto&&... is) {
        return zug::wrap_state<foo_tag>(
            step(zug::state_unwrap(std::forward<decltype(s)>(s)),
                 std::forward<decltype(is)>(is)...),
            non_default{42});
    };
};

} // namespace

TEST_CASE("reduce does not construct state wrapper from state")
{
    auto v = std::vector<int>{1, 2, 3, 6};
    auto r = zug::reduce(foo(zug::first_rf), 13, v);
    CHECK(r == 13);
}

TEST_CASE("reduce copies values out of lvalue container")
{
    using elem = testing::copy_spy<>;

    auto x      = elem{};
    auto v      = std::vector<elem>{x, x, x, x};
    auto copies = x.copied.count();
    zug::reduce(zug::last_rf, x, v);
    CHECK(x.copied.count() == copies + 4);
}

TEST_CASE("reduce moves values out of rvalue container")
{
    using elem = testing::copy_spy<>;

    auto x      = elem{};
    auto v      = std::vector<elem>{x, x, x, x};
    auto copies = x.copied.count();
    zug::reduce(zug::last_rf, std::move(x), std::move(v));
    CHECK(x.copied.count() == copies);
}

TEST_CASE("moves values out of rvalue container variadic")
{
    using elem = testing::copy_spy<>;

    auto x      = elem{};
    auto v1     = std::vector<elem>{x, x, x, x};
    auto v2     = v1;
    auto init   = std::make_tuple(x, x);
    auto copies = x.copied.count();
    zug::reduce(zug::last_rf, std::move(init), std::move(v1), std::move(v2));
    CHECK(x.copied.count() == copies);
}
