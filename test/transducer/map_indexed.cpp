//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/transduce.hpp>
#include <zug/transducer/map_indexed.hpp>

using namespace zug;

TEST_CASE("map_indexed, mapping")
{
    // example1 {
    auto v      = std::vector<std::string>{"1", "2", "3", "6"};
    auto timesi = [](std::string x, std::size_t index) {
        return std::stoi(x) * static_cast<int>(index);
    };
    CHECK(transduce(map_indexed(timesi), std::plus<int>{}, 1, v) == 27);
    // }
}
