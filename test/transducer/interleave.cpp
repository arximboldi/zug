//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/interleave.hpp>

using namespace zug;

TEST_CASE("interleave, example")
{
    // example1 {
    auto v = std::vector<int>{1, 2, 3, 4, 5};
    auto r = into_vector(interleave, v, v);
    CHECK(r == decltype(r){1, 1, 2, 2, 3, 3, 4, 4, 5, 5});
    // }
}
