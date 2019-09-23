//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/cat.hpp>
#include <zug/transducer/readbuf.hpp>

#include <sstream>

using namespace zug;

TEST_CASE("readbuf, constant sized")
{
    auto stream = std::stringstream{"123456"};
    auto res    = into_vector(comp(readbuf<3>(stream), cat));
    CHECK(res == (decltype(res){'1', '2', '3', '4', '5', '6'}));
}

TEST_CASE("readbuf, dynamic sized")
{
    auto stream = std::stringstream{"123456"};
    auto res    = into_vector(comp(readbuf(stream, 3), cat));
    CHECK(res == (decltype(res){'1', '2', '3', '4', '5', '6'}));
}
