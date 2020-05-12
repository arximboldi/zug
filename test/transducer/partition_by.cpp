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
#include <zug/reducing/first.hpp>
#include <zug/transducer/cat.hpp>
#include <zug/transducer/partition_by.hpp>
#include <zug/transducer/take.hpp>
#include <zug/transducer/transducer.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("partition_by, partition_by")
{
    auto v = std::vector<int>{1, 1, 2, 2, 2, 3};

    auto res = into_vector(partition_by(identity), v);
    CHECK(res == (decltype(res){{1, 1}, {2, 2, 2}, {3}}));
}

namespace {
int free_mod2(int x) { return x % 2; }
} // namespace

TEST_CASE("partition_by, invoke")
{
    auto v = std::vector<int>{1, 1, 2, 4, 2, 3};

    auto res = into_vector(partition_by(&free_mod2), v);
    CHECK(res == (decltype(res){{1, 1}, {2, 4, 2}, {3}}));
}

TEST_CASE("partition_by, example")
{
    // example1 {
    auto v   = std::vector<int>{1, 1, 2, 4, 2, 3};
    auto res = into_vector(partition_by([](int x) { return x % 2; }), v);
    using t  = std::vector<std::vector<int>>;
    CHECK(res == t{{1, 1}, {2, 4, 2}, {3}});
    // }
}

TEST_CASE("partition_by, more example")
{
    // example2 {
    auto v   = std::vector<int>{1, 1, 2, 2, 2, 3};
    auto res = into_vector(partition_by(identity), v);
    CHECK(res == decltype(res){{1, 1}, {2, 2, 2}, {3}});
    // }
}

TEST_CASE("partition_by, partition_by does not copy step function")
{
    auto step = testing::copy_spy<first_t>{};

    auto v = std::vector<int>{1, 2, 3, 4, 5, 7, 8, 9};
    reduce(partition_by(identity)(step), 0, v);
    CHECK(step.copied.count() == 2);
}

TEST_CASE("partition_by, partition by moves the state through")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto spy = reduce(partition_by(identity)(first), testing::copy_spy<>{}, v);
    CHECK(spy.copied.count() == 0);
}

TEST_CASE("partition_by, reduce nested deals with empty sequence properly")
{
    auto v    = std::vector<std::vector<int>>{{{}, {1, 1, 2}, {}}};
    auto part = transducer<int, std::vector<int>>{partition_by(identity)};
    auto res  = into_vector(comp(cat, part), v);
    CHECK(res == (std::vector<std::vector<int>>{{1, 1}, {2}}));
}
