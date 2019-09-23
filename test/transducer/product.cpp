//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/sequence.hpp>
#include <zug/transducer/product.hpp>
#include <zug/transducer/range.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("product, product")
{
    using tup = std::tuple<int, int>;
    auto v1   = std::vector<int>{1, 2};
    auto v2   = std::vector<int>{4, 5};
    auto res  = into_vector(product(v2), v1);
    CHECK(res == (decltype(res){tup(1, 4), tup(1, 5), tup(2, 4), tup(2, 5)}));
}

TEST_CASE("product, variadic")
{
    using tup = std::tuple<int, int, char>;
    auto v1   = std::vector<int>{1, 2};
    auto v2   = std::vector<int>{4, 5};
    auto v3   = std::vector<char>{'a', 'b'};

    auto res = into_vector(product(v2, v3), v1);
    CHECK(res == (decltype(res){tup(1, 4, 'a'),
                                tup(1, 4, 'b'),
                                tup(1, 5, 'a'),
                                tup(1, 5, 'b'),
                                tup(2, 4, 'a'),
                                tup(2, 4, 'b'),
                                tup(2, 5, 'a'),
                                tup(2, 5, 'b')}));
}

TEST_CASE("product, generator")
{
    using tup = std::tuple<int, int>;
    auto v1   = std::vector<int>{1, 2};
    auto v2   = std::vector<int>{4, 5};

    auto res = into_vector(comp(take(1), product(v1, v2)));
    CHECK(res == (decltype(res){tup(1, 4), tup(1, 5), tup(2, 4), tup(2, 5)}));
}

TEST_CASE("product, tranducer product example")
{
    using tup = std::tuple<int, int>;

    auto idx = sequence(range(2));
    auto res = into_vector(product(idx), idx);

    for (auto&& x : res) {
        std::cout << std::get<0>(x) << ", " << std::get<1>(x) << std::endl;
    }
    CHECK(res == (decltype(res){tup(0, 0), tup(0, 1), tup(1, 0), tup(1, 1)}));
}
