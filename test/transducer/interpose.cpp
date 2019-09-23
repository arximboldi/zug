//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/interpose.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("interpose, into")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto res = into_vector(interpose(42), v);
    CHECK(res == (decltype(res){1, 42, 2, 42, 3, 42, 4, 42, 5}));
}

TEST_CASE("interpose, termination")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto res = into_vector(comp(interpose(42), take(4)), v);
    CHECK(res == (decltype(res){1, 42, 2, 42}));
}

TEST_CASE("interpose, variadic")
{
    using tup = std::tuple<int, char>;
    auto v1   = std::vector<int>{1, 2, 3, 4, 5};
    auto v2   = std::vector<char>{'a', 'b', 'c'};

    auto res = into_vector(interpose(42, 'Z'), v1, v2);
    CHECK(res == (decltype(res){tup(1, 'a'),
                                tup(42, 'Z'),
                                tup(2, 'b'),
                                tup(42, 'Z'),
                                tup(3, 'c')}));
}
