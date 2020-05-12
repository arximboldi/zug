//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/chain.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("chain, append")
{
    // example1 {
    auto v1  = std::vector<int>{1, 2};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chain(v2), v1);
    CHECK(res == (std::vector<int>{1, 2, 13, 42, 5}));
    // }
}

TEST_CASE("chain, variadic")
{
    // example2 {
    auto v1  = std::vector<int>{1, 2};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chain(v2, v1), v1);
    CHECK(res == (std::vector<int>{1, 2, 13, 42, 5, 1, 2}));
    // }
}

TEST_CASE("chain, no chaining if no input")
{
    // example22 {
    // This is a limitation of the pure stateful transducer design
    auto v1  = std::vector<int>{};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chain(v2), v1);
    CHECK(res == (std::vector<int>{}));
    // }
}

TEST_CASE("chainl, prepend")
{
    // example3 {
    auto v1  = std::vector<int>{1, 2};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chainl(v2), v1);
    CHECK(res == (std::vector<int>{13, 42, 5, 1, 2}));
    // }
}

TEST_CASE("chainl, variadic")
{
    // example4 {
    auto v1  = std::vector<int>{1, 2};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chainl(v2, v1), v1);
    CHECK(res == (std::vector<int>{1, 2, 13, 42, 5, 1, 2}));
    // }
}

TEST_CASE("chainl, no chaining if no input")
{
    // example5 {
    // This is a limitation of the pure stateful transducer design
    auto v1  = std::vector<int>{};
    auto v2  = std::vector<int>{13, 42, 5};
    auto res = into_vector(chainl(v2), v1);
    CHECK(res == (std::vector<int>{}));
    // }
}
