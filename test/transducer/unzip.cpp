//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/reducing/last.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/unzip.hpp>

#include <array>
#include <iostream>

using namespace zug;

TEST_CASE("unzip, minimal")
{
    auto r = unzip(map([](auto a, auto b) { return a + b; })(last))(
        0, std::make_tuple(13, 42));
    CHECK(r == 55);
}

TEST_CASE("unzip, unzip transducer")
{
    using tup = std::tuple<int, double>;

    auto v1 = std::vector<tup>{{tup(13, 1.1), tup(42, 2.2), tup(5, 3.3)}};

    auto res = transduce(
        unzip, [](double x, int y, double z) { return x + y + z; }, 1.0, v1);

    CHECK_THAT(res, Catch::Matchers::WithinAbs(67.6, 0.001));
}

TEST_CASE("unzip, unzip transducer variadic multitype")
{
    using tup1 = std::tuple<int, double>;
    using tup2 = std::tuple<char>;
    using arr  = std::array<int, 2>;

    auto v1 = std::vector<tup1>{{tup1(13, 1.1), tup1(42, 2.2), tup1(5, 3.3)}};
    auto v2 = std::vector<short>{{1, 2, 3}};
    auto v3 = std::vector<tup2>{{tup2('a'), tup2('b'), tup2('c')}};
    auto v4 = std::vector<arr>{{arr{{1, 2}}, arr{{3, 4}}, arr{{5, 6}}}};

    auto res =
        transduce(unzip,
                  [](double x, int y, double z, short v, char w, int a, int b) {
                      return x + y + z + v + w + a + b;
                  },
                  1.0,
                  v1,
                  v2,
                  v3,
                  v4);

    CHECK_THAT(res, Catch::Matchers::WithinAbs(388.6, 0.001));
}
