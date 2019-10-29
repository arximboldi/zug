//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/compose.hpp>
#include <zug/into_vector.hpp>
#include <zug/transducer/cycle.hpp>
#include <zug/transducer/distinct.hpp>

using namespace zug;

TEST_CASE("distinct, into")
{
    auto v   = std::vector<int>{1, 2, 1, 3, 2, 1};
    auto res = into_vector(distinct, v);
    CHECK(res == (decltype(res){1, 2, 3}));
}

TEST_CASE("distinct, variadic")
{
    using tup = std::tuple<int, char>;
    auto v    = std::vector<int>{1, 2, 1, 3, 2, 1, 2};
    auto res  = into_vector(comp(cycle(std::string("ab")), distinct), v);

    CHECK(
        res ==
        (decltype(res){
            tup(1, 'a'), tup(2, 'b'), tup(3, 'b'), tup(2, 'a'), tup(1, 'b')}));
}
