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
#include <zug/reducing/last.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/mapcat.hpp>
#include <zug/transducer/partition.hpp>
#include <zug/transducer/take.hpp>
#include <zug/transducer/take_while.hpp>
#include <zug/transducer/transducer.hpp>

namespace {

bool free_odd(int x) { return x % 2 == 0; }

} // namespace

using namespace zug;

TEST_CASE("filter, example")
{
    // example1 {
    auto v   = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto res = into_vector(filter([](int x) { return x % 2; }), v);
    CHECK(res == decltype(v){1, 3, 5});
    // }
}

TEST_CASE("filter, invoke")
{
    auto v   = std::vector<int>{1, 2, 3, 6};
    auto res = transduce(filter(&free_odd), std::plus<int>{}, 1, v);
    CHECK(res == 9);
}

TEST_CASE("filter, composition")
{
    auto v      = std::vector<int>{1, 2, 3, 6};
    auto times2 = [](int x) { return x * 2; };
    auto odd    = [](int x) { return x % 2 == 0; };

    // transducers compose from left to right, this is equivalent to
    // Haskell-like expression:
    //
    //   foldl (+) $ map times2 $ filter odd $ v
    //
    auto res =
        transduce(comp(filter(odd), map(times2)), std::plus<int>{}, 1, v);
    CHECK(res == 17);
}

TEST_CASE("filter, operator| composition")
{
    auto v      = std::vector<int>{1, 2, 3, 6};
    auto times2 = [](int x) { return x * 2; };
    auto odd    = [](int x) { return x % 2 == 0; };

    // transducers compose from left to right, this is equivalent to
    // Haskell-like expression:
    //
    //   foldl (+) $ map times2 $ filter odd $ v
    //
    auto res = transduce(filter(odd) | map(times2), std::plus<int>{}, 1, v);
    CHECK(res == 17);
}

TEST_CASE("filter, make sure inputs cant be doubly sinked")
{
    auto orig = std::vector<int>{{1, 2, 3}};
    auto pred = [](std::vector<int> x) { return x.size() > 2; };

    auto v = orig;
    auto x = filter(pred)(last)(std::vector<int>{}, std::move(v));
    CHECK(x == orig);
}

TEST_CASE("filter, compose with stateful transducer")
{
    auto v    = std::vector<int>{{1, 2, 3, 4, 5, 6}};
    auto even = [](int x) { return x % 2 == 0; };

    auto res = transduce(comp(filter(even), take(2)), std::plus<int>{}, 1, v);
    CHECK(res == 7);
}

TEST_CASE("filter, compose with stateful transducer type erased")
{
    auto v    = std::vector<int>{{1, 2, 3, 4, 5, 6}};
    auto even = [](int x) { return x % 2 == 0; };

    auto res = transduce(
        comp(filter(even), transducer<int>(take(2))), std::plus<int>{}, 1, v);
    CHECK(res == 7);
}

TEST_CASE("filter, compose type erased with stateful transducer")
{
    auto v    = std::vector<int>{{1, 2, 3, 4, 5, 6}};
    auto even = [](int x) { return x % 2 == 0; };

    auto res = transduce(
        comp(transducer<int>(filter(even)), take(2)), std::plus<int>{}, 1, v);
    CHECK(res == 7);
}

TEST_CASE("filter, type erase and compose with stateful transducer")
{
    auto v    = std::vector<int>{{1, 2, 3, 4, 5, 6}};
    auto even = [](int x) { return x % 2 == 0; };

    auto res = transduce(
        transducer<int>{comp(filter(even), take(2))}, std::plus<int>{}, 1, v);
    CHECK(res == 7);
}

TEST_CASE("filter, type erasure triple mortal back flip")
{
    auto even    = [](int x) { return x % 2 == 0; };
    auto gt3     = [](int x) { return x > 3; };
    auto lt8     = [](int x) { return x < 8; };
    auto plus1   = [](int x) { return x + 1; };
    auto times2  = [](int x) { return x * 2; };
    auto longer3 = [](std::vector<int> x) { return x.size() > 3u; };
    auto fstlt10 = [](std::vector<int> x) { return !x.empty() && x[0] < 10; };

    auto v     = std::vector<int>{{1, 2, 3, 4, 5, 6, 7, 8, 9}};
    auto xform = transducer<int>{
        comp(comp(take_while(lt8),
                  partition(3u),
                  take(10),
                  mapcat(plus1),
                  filter(even),
                  comp(transducer<int>{map(times2)}, filter(gt3))),
             partition(6u),
             transducer<std::vector<int>, int>{
                 comp(transducer<std::vector<int>>{filter(longer3)},
                      take_while(fstlt10),
                      cat,
                      transducer<int>{take(2)})},
             filter(gt3),
             comp(take(5), transducer<int>{map(times2)}, filter(even)))};

    {
        auto res = transduce(xform, std::plus<int>{}, 1, v);
        CHECK(res == 25);
    }

    {
        auto res = into_vector(xform, v);
        CHECK(res == (decltype(res){8, 16}));
    }
}
