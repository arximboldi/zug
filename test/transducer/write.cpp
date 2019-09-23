//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/reducing/first.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/write.hpp>

#include <sstream>
#include <vector>

using namespace zug;

TEST_CASE("write, write")
{
    auto v      = std::vector<int>{1, 2, 3, 4};
    auto stream = std::stringstream{};
    transduce(write(stream), first, 0, v);
    CHECK(stream.str() == "1234");
}

TEST_CASE("write, in separator")
{
    auto v      = std::vector<int>{1, 2, 3, 4};
    auto stream = std::stringstream{};
    transduce(write(stream, ' '), first, 0, v);
    CHECK(stream.str() == "1 2 3 4");
}

TEST_CASE("write, in separator variadic is arg separator")
{
    auto v1     = std::vector<int>{1, 2, 3, 4};
    auto v2     = std::vector<char>{'y', 'e', 'a', 'h'};
    auto stream = std::stringstream{};
    transduce(write(stream, ' '), first, 0, v1, v2);
    CHECK(stream.str() == "1 y 2 e 3 a 4 h");
}

TEST_CASE("write, in separator and arg separator")
{
    auto v1     = std::vector<int>{1, 2, 3, 4};
    auto v2     = std::vector<char>{'y', 'e', 'a', 'h'};
    auto stream = std::stringstream{};
    transduce(write(stream, ' ', ','), first, 0, v1, v2);
    CHECK(stream.str() == "1,y 2,e 3,a 4,h");
}
