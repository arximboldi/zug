//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/reducing/last.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/range.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("range, stop")
{
    auto res = into_vector(range(3));
    CHECK(res == (std::vector<std::size_t>{{0, 1, 2}}));
}

TEST_CASE("range, start stop")
{
    auto res = into_vector(range(2, 6));
    CHECK(res == (std::vector<int>{{2, 3, 4, 5}}));
}

TEST_CASE("range, start stop step")
{
    auto res = into_vector(range(2, 6, 2));
    CHECK(res == (std::vector<int>{{2, 4}}));
}

TEST_CASE("range, start stop step floating_point")
{
    auto res      = into_vector(range(2.0, 6.0, 1.2));
    auto expected = std::vector<double>{{2.0, 3.2, 4.4, 5.6}};
    CHECK(transduce(map([](double x, double y) {
                        CHECK_THAT(x, Catch::Matchers::WithinAbs(y, 0.1));
                        return true;
                    }),
                    last,
                    true,
                    res,
                    expected));
}
