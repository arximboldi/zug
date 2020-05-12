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
#include <zug/transducer/repeat.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("repeat, repeat")
{
    using tup = std::tuple<int, const char*>;
    auto hola = "hola";
    auto v    = std::vector<int>{13, 42, 5};

    auto res = into_vector(repeat(hola), v);
    CHECK(res ==
          (std::vector<tup>{{tup(13, hola), tup(42, hola), tup(5, hola)}}));
}

TEST_CASE("repeat, generator")
{
    // example1 {
    auto res = into_vector(repeat('a') | take(3));
    CHECK(res == (std::vector<char>{{'a', 'a', 'a'}}));
    // }
}

TEST_CASE("repeat, parallel")
{
    // example2 {
    auto v   = std::vector<int>{1, 2, 3};
    auto res = into_vector(repeat('a'), v);
    CHECK(res == decltype(res){{1, 'a'}, {2, 'a'}, {3, 'a'}});
    // }
}

TEST_CASE("repeatn, generator")
{
    // example3 {
    auto res = into_vector(repeatn(3, 'a'));
    CHECK(res == (std::vector<char>{{'a', 'a', 'a'}}));
    // }
}
