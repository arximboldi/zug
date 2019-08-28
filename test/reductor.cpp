//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/reducing/first.hpp>
#include <zug/reducing/last.hpp>
#include <zug/reductor.hpp>
#include <zug/transducer/enumerate.hpp>
#include <zug/transducer/take.hpp>

#include "spies.hpp"

using namespace zug;

TEST_CASE("reductor, reductor")
{
    auto r = reductor(std::plus<>{}, 0, 1);
    r(2);
    r(3);
    r(4);
    CHECK(r.complete() == 10);
}

TEST_CASE("reductor, empty reductor")
{
    auto r = empty_reductor<int>(std::plus<>{}, 0);
    r(2);
    r(3);
    r(4);
    CHECK(r.complete() == 9);
}

TEST_CASE("reductor, reductor no move")
{
    auto r = reductor(std::plus<>{}, std::string{}, "");
    r("hello");
    r(" ");
    r("world");
    CHECK(r.complete() == "hello world");
    CHECK(r.complete() == "hello world");
}

TEST_CASE("reductor, reductor chaining")
{
    auto result = reductor(std::plus<>{}, std::string{}, "")("hello")(" ")(
                      "my")(" ")("friend")("!")
                      .complete();
    CHECK(result == "hello my friend!");
}

TEST_CASE("reductor, reductor move")
{
    auto s = testing::copy_spy<>{};
    auto r = reductor(first, std::move(s), 0);
    r(1);
    r(2);
    r(3);
    auto c = std::move(r).complete();
    CHECK(c.copied.count() == 0);
}

TEST_CASE("reductor, generator")
{
    auto r = reductor(enumerate(last), -1);
    CHECK(r.complete() == 0);
    r();
    CHECK(r.complete() == 1);
    r();
    CHECK(r.complete() == 2);
}

TEST_CASE("reductor, generator empty")
{
    auto r = empty_reductor(enumerate(last), std::size_t{42});
    CHECK(r.complete() == 42u);
    r();
    CHECK(r.complete() == 0u);
    r();
    CHECK(r.complete() == 1u);
}

TEST_CASE("reductor, termination")
{
    auto r = reductor(take(4)(enumerate(last)), 0);
    CHECK(r());
    CHECK(r());
    CHECK(!r());
    CHECK(!r);
}

TEST_CASE("reductor, termination empty")
{
    auto r = empty_reductor(take(4)(enumerate(last)), 0);
    CHECK(r());
    CHECK(r());
    CHECK(r());
    CHECK(!r());
    CHECK(!r);
}

TEST_CASE("reductor, current")
{
    auto r = empty_reductor(take(4)(enumerate(last)), std::size_t{0});
    CHECK(r());
    CHECK(r.current() == 0);
    r.current(std::size_t{10});
    CHECK(r.current() == 10);
    CHECK(r());
    CHECK(r.current() == 1);
    CHECK(r());
    CHECK(r.current() == 2);
    CHECK(!r());
    CHECK(!r);
}

TEST_CASE("reductor, constant")
{
    const auto r1  = reductor(enumerate(last), 0);
    const auto r2  = r1();
    const auto r2_ = r1();
    const auto r3  = r2();

    CHECK(r2.complete() == 1);
    CHECK(r2_.complete() == 1);
    CHECK(r3.complete() == 2);
}
