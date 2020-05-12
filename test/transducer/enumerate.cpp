//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/enumerate.hpp>

using namespace zug;

TEST_CASE("enumerate, example")
{
    // example1 {
    auto s = std::string{"hello"};
    auto r = into_vector(enumerate, s);
    CHECK(r == decltype(r){{0, 'h'}, {1, 'e'}, {2, 'l'}, {3, 'l'}, {4, 'o'}});
    // }
}

TEST_CASE("enumerate_from, example")
{
    // example2 {
    auto s = std::string{"hello"};
    auto r = into_vector(enumerate_from(2), s);
    CHECK(r == decltype(r){{2, 'h'}, {3, 'e'}, {4, 'l'}, {5, 'l'}, {6, 'o'}});
    // }
}
