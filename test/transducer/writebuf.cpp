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
#include <zug/transducer/partition.hpp>
#include <zug/transducer/writebuf.hpp>

#include <sstream>

using namespace zug;

TEST_CASE("writebuf, writebuf partitions")
{
    auto s      = std::string{"123456"};
    auto stream = std::stringstream{};
    run(comp(partition(3u), writebuf(stream)), s);
    CHECK(stream.str() == s);
}

TEST_CASE("writebuf, writebuf strings")
{
    // example1 {
    auto v      = std::vector<std::string>{"123", "4", "56"};
    auto stream = std::stringstream{};
    run(writebuf(stream), v);
    CHECK(stream.str() == "123456");
    // }
}
