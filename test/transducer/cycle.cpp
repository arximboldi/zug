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
#include <zug/transducer/cycle.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("cycle, generator")
{
    using tup = std::tuple<int, int>;
    auto v1   = std::vector<int>{13, 42, 5, 6, 15};
    auto v2   = std::vector<int>{0, 1};
    auto res  = into_vector(cycle(v2), v1);
    CHECK(res ==
          (decltype(res){
              tup(13, 0), tup(42, 1), tup(5, 0), tup(6, 1), tup(15, 0)}));
}

TEST_CASE("cycle, variadic")
{
    using tup = std::tuple<int, std::string>;
    auto v1   = std::vector<int>{0, 1};
    auto v2   = std::vector<std::string>{"one", "two", "three"};
    auto res  = into_vector(comp(cycle(v1, v2), take(5)));
    CHECK(res == (decltype(res){tup(0, "one"),
                                tup(1, "two"),
                                tup(0, "three"),
                                tup(1, "one"),
                                tup(0, "two")}));
}
