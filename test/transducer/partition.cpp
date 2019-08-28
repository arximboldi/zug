//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into.hpp>
#include <zug/reducing/first.hpp>
#include <zug/transducer/cat.hpp>
#include <zug/transducer/partition.hpp>
#include <zug/transducer/take.hpp>
#include <zug/transducer/transducer.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("partition, partition")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5, 6};

    auto res = into(std::vector<std::vector<int>>{}, partition(2u), v);
    CHECK(res == (std::vector<std::vector<int>>{{1, 2}, {3, 4}, {5, 6}}));
}

TEST_CASE("partition, partition flushing")
{
    auto v = std::vector<int>{1, 2, 3, 4, 5};

    auto res = into(std::vector<std::vector<int>>{}, partition(2u), v);
    CHECK(res == (std::vector<std::vector<int>>{{1, 2}, {3, 4}, {5}}));
}

namespace {
struct some_step_fn
{
    template <typename T, typename... Xs>
    T operator()(T x, Xs&&...) const
    {
        return x;
    }
};

} // namespace

TEST_CASE("partition, partition does not copy step function")
{
    auto step = testing::copy_spy<some_step_fn>{};

    auto v = std::vector<int>{1, 2, 3, 4, 5, 7, 8, 9};
    reduce(partition(2u)(step), 0, v);
    CHECK(step.copied.count() == 2);
}

TEST_CASE("partition, partition moves the state through")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto spy = reduce(partition(2u)(first), testing::copy_spy<>{}, v);
    CHECK(spy.copied.count() == 0);
}

TEST_CASE("partition, defining early termination with completion")
{
    auto v = std::vector<int>{{1, 2, 3, 4, 5}};
    auto res =
        into(std::vector<std::vector<int>>{}, comp(take(3), partition(2u)), v);
    CHECK(res == (std::vector<std::vector<int>>{{1, 2}, {3}}));
}

TEST_CASE("partition, reduce nested deals with empty sequence properly")
{
    auto v   = std::vector<std::vector<int>>{{{}, {1, 2, 3}, {}}};
    auto res = into(std::vector<std::vector<int>>{},
                    comp(cat, transducer<int, std::vector<int>>{partition(2u)}),
                    v);
    CHECK(res == (std::vector<std::vector<int>>{{1, 2}, {3}}));
}
