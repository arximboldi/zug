//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/compose.hpp>
#include <zug/run.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/read.hpp>
#include <zug/transducer/write.hpp>

#include <sstream>
#include <vector>

using namespace zug;

TEST_CASE("run, nullary")
{
    auto input  = std::stringstream{"1 2 3 4 5"};
    auto output = std::stringstream{};

    run(comp(read<int>(input), write(output, ';')));
    CHECK(output.str() == "1;2;3;4;5");
}

TEST_CASE("run, unary")
{
    auto v      = std::vector<int>{1, 2, 3, 4, 5};
    auto output = std::stringstream{};

    run(write(output, ';'), v);
    CHECK(output.str() == "1;2;3;4;5");
}

TEST_CASE("run, variadic")
{
    auto v1     = std::vector<int>{1, 2, 3, 4, 5};
    auto v2     = std::vector<char>{'h', 'e', 'l', 'l', 'o'};
    auto output = std::stringstream{};

    run(write(output, ';', ' '), v1, v2);
    CHECK(output.str() == "1 h;2 e;3 l;4 l;5 o");
}
