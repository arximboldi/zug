//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/transduce.hpp>
#include <zug/transducer/zip.hpp>

using namespace zug;

TEST_CASE("zip, zip transducer")
{
    // example1 {
    auto v1  = std::vector<int>{13, 42, 5};
    auto v2  = std::vector<double>{1.1, 2.2, 3.3};
    auto res = transduce(
        zip,
        [](double x, std::tuple<int, double> t) {
            return x + std::get<0>(t) + std::get<1>(t);
        },
        1.0,
        v1,
        v2);
    CHECK_THAT(res, Catch::Matchers::WithinAbs(67.6, 0.001));
    // }
}
