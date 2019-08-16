//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/iter.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("iter, generator")
{
    auto v   = std::vector<int>{13, 42, 5};
    auto res = into_vector(iter(v));
    CHECK(res == (std::vector<int>{13, 42, 5}));
}

TEST_CASE("iter, variadic")
{
    auto v1  = std::vector<int>{13, 42, 5, 6, 7};
    auto v2  = std::vector<std::string>{"one", "two", "three"};
    auto res = into_vector(iter(v1, v2));
    CHECK(res == (decltype(res){std::make_tuple(13, "one"),
                                std::make_tuple(42, "two"),
                                std::make_tuple(5, "three")}));
}
