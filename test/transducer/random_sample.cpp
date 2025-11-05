//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <cstdint>
#include <limits>

struct deterministic_engine
{
    using result_type = std::uint64_t;

    std::uint64_t count = 0;

    constexpr static std::uint64_t max()
    {
        return std::numeric_limits<std::uint64_t>::max();
    }

    constexpr static std::uint64_t min()
    {
        return std::numeric_limits<std::uint64_t>::min();
    }

    std::uint64_t operator()() { return count += max() / 6; }
};

#define ZUG_DEFAULT_RANDOM_ENGINE deterministic_engine

#include <zug/compose.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/random_sample.hpp>
#include <zug/transducer/range.hpp>

using namespace zug;

TEST_CASE("random_sample, simple")
{
    // example1 {
    CHECK(transduce(range(20) | random_sample(0.5),
                    std::plus<>{},
                    std::size_t{}) == 100);

    CHECK(transduce(range(20) | random_sample(1),
                    std::plus<>{},
                    std::size_t{}) == 190);

    CHECK(transduce(range(20) | random_sample(0), //
                    std::plus<>{},
                    std::size_t{}) == 0);
    // }
}

TEST_CASE("random_sample, custom generator")
{
    auto eng = deterministic_engine{};
    auto gen = [eng]() mutable { return double(eng()) * 2 / double(eng.max()); };

    CHECK(transduce(comp(range(20), random_sample(0.5, gen)),
                    std::plus<std::size_t>{},
                    std::size_t{}) == 36);

    CHECK(transduce(comp(range(20), random_sample(1, gen)),
                    std::plus<std::size_t>{},
                    std::size_t{}) == 100);

    CHECK(transduce(comp(range(20), random_sample(0, gen)),
                    std::plus<std::size_t>{},
                    std::size_t{}) == 0);
}
