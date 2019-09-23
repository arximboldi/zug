//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/read.hpp>

#include <sstream>

using namespace zug;

TEST_CASE("read, into")
{
    auto stream = std::stringstream{"1 2 3 4 5 6"};
    auto res    = into_vector(read<int>(stream));
    CHECK(res == (decltype(res){1, 2, 3, 4, 5, 6}));
}

TEST_CASE("read, into variadic")
{
    using tup = std::tuple<int, std::string>;

    auto stream = std::stringstream{"1 hello 2 my 3 friend 4 !!"};
    auto res    = into_vector(read<int, std::string>(stream));
    CHECK(res ==
          (decltype(res){
              tup(1, "hello"), tup(2, "my"), tup(3, "friend"), tup(4, "!!")}));
}

TEST_CASE("read, into ends on bad input")
{
    auto stream = std::stringstream{"1 2 3 fuck 4 5 6"};
    auto res    = into_vector(read<int>(stream));
    CHECK(res == (decltype(res){1, 2, 3}));
}
