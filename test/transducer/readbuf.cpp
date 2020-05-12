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
#include <zug/transducer/cat.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/readbuf.hpp>

#include <sstream>

using namespace zug;

TEST_CASE("readbuf, constant sized")
{
    // example1 {
    auto as_str = map([](auto buf) {
        return std::string{buf.begin(), buf.end()};
    });
    auto stream = std::stringstream{"12345"};
    auto res    = into_vector(readbuf<3>(stream) | as_str);
    CHECK(res == std::vector<std::string>{"123", "45"});
    // }
}

TEST_CASE("readbuf, dynamic sized")
{
    // example2 {
    auto as_str = map([](auto buf) {
        return std::string{buf.begin(), buf.end()};
    });
    auto stream = std::stringstream{"12345"};
    auto res    = into_vector(readbuf(stream, 3) | as_str);
    CHECK(res == std::vector<std::string>{"123", "45"});
    // }
}
