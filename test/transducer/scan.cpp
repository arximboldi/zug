//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/count.hpp>
#include <zug/transducer/scan.hpp>
#include <zug/transducer/take.hpp>

#include <vector>

using namespace zug;

TEST_CASE("scan, scan")
{
    // example1 {
    auto v = std::vector<int>{1, 2, 3, 4, 5, 6};
    auto r = into_vector(scan(0, std::plus<>{}), v);
    CHECK(r == (std::vector<int>{1, 3, 6, 10, 15, 21}));
    // }
}

TEST_CASE("scan, stateful")
{
    // example2 {
    auto r = into_vector(count(1) | scan(0, take(6)(std::plus<>{})));
    CHECK(r == std::vector<int>{1, 3, 6, 10, 15, 21});
    // }
}
