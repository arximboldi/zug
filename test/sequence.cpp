//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/sequence.hpp>
#include <zug/transducer/enumerate.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/interleave.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/take.hpp>

#include <boost/range/algorithm.hpp>
#include <boost/variant.hpp>

#include <vector>

using namespace zug;

TEST_CASE("sequence, identity")
{
    auto v   = std::vector<int>{1, 2, 3, 4};
    auto r   = std::vector<int>{};
    auto seq = sequence(identity, v);
    boost::copy(seq, std::back_inserter(r));
    CHECK(r == v);
}

TEST_CASE("sequence, filter_map")
{
    auto v   = std::vector<int>{1, 2, 3, 4};
    auto r   = std::vector<int>{};
    auto seq = sequence<int>(comp(filter([](int x) { return x % 2 == 0; }),
                                  map([](int x) { return x * 2; })),
                             v);
    boost::copy(seq, std::back_inserter(r));
    CHECK(r == (std::vector<int>{{4, 8}}));
}

TEST_CASE("sequence, rvalues")
{
    auto r = std::vector<int>{};
    boost::copy(sequence(identity, std::vector<int>{1, 2, 3, 4}),
                std::back_inserter(r));
    CHECK(r == (std::vector<int>{1, 2, 3, 4}));
}

TEST_CASE("sequence, constant")
{
    const auto v = std::vector<int>{1, 2, 3, 4};
    auto r       = std::vector<int>{};
    boost::copy(sequence(identity, v), std::back_inserter(r));
    CHECK(r == v);
}

TEST_CASE("sequence, no shared iterator state")
{
    const auto v = std::vector<int>{1, 2, 3, 4};
    auto seq     = sequence(take(3), v);

    auto f2 = seq.begin();
    auto e  = seq.end();
    auto f1 = f2++;

    auto r1 = std::vector<int>{};
    std::copy(f1, e, std::back_inserter(r1));
    CHECK(r1 == (decltype(r1){1, 2, 3}));

    auto r2 = std::vector<int>{};
    std::copy(f2, e, std::back_inserter(r2));
    CHECK(r2 == (decltype(r2){2, 3}));
}

TEST_CASE("sequence, empty")
{
    const auto v = std::vector<int>{};
    auto seq     = sequence(take(3), v);
    auto r       = std::vector<int>{};
    boost::copy(seq, std::back_inserter(r));
    CHECK(r == (decltype(r){}));
}

TEST_CASE("sequence, variadic growing no type deduction")
{
    using res_t = boost::variant<int, std::string>;

    const auto v1 = std::vector<int>{1, 2, 3, 4};
    const auto v2 = std::vector<std::string>{"a", "b", "c"};
    auto seq      = sequence<res_t>(interleave, v1, v2);

    auto r = std::vector<res_t>{};
    boost::copy(seq, std::back_inserter(r));
    CHECK(
        r ==
        (decltype(r){
            res_t{1}, res_t{"a"}, res_t{2}, res_t{"b"}, res_t{3}, res_t{"c"}}));
}

TEST_CASE("sequence, generator")
{
    auto seq = sequence(enumerate);

    auto r = std::vector<std::size_t>{};
    std::copy_n(seq.begin(), 4u, std::back_inserter(r));
    CHECK(r == (decltype(r){0u, 1u, 2u, 3u}));
}
