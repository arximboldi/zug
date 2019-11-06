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
#include <zug/transducer/drop.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("drop, into")
{
    using namespace std::placeholders;
    auto v = std::vector<int>{1, 2, 3, 4, 5};

    {
        auto res = into_vector(drop(0), v);
        CHECK(res == (decltype(res){1, 2, 3, 4, 5}));
    }

    {
        auto res = into_vector(drop(2), v);
        CHECK(res == (decltype(res){3, 4, 5}));
    }

    {
        auto res = into_vector(drop(6), v);
        CHECK(res == (decltype(res){}));
    }
}

TEST_CASE("drop, compose")
{
    using namespace std::placeholders;
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto res = into_vector(comp(drop(2), take(2)), v);
    CHECK(res == (decltype(res){3, 4}));
}
