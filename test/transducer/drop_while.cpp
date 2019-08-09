//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/drop_while.hpp>
#include <zug/transducer/take.hpp>

using namespace zug;

TEST_CASE("drop_while, into")
{
    using namespace std::placeholders;
    auto v = std::vector<int>{1, 2, 3, 4, 3};

    {
        auto lt0 = std::bind(std::less<>{}, _1, 0);
        auto res = into_vector(drop_while(lt0), v);
        CHECK(res == (decltype(res){1, 2, 3, 4, 3}));
    }

    {
        auto lt4 = std::bind(std::less<>{}, _1, 4);
        auto res = into_vector(drop_while(lt4), v);
        CHECK(res == (decltype(res){4, 3}));
    }

    {
        auto lt5 = std::bind(std::less<>{}, _1, 5);
        auto res = into_vector(drop_while(lt5), v);
        CHECK(res == (decltype(res){}));
    }
}

namespace {
bool free_lt4(int x) { return x < 4; }
} // namespace

TEST_CASE("drop_while, invoke")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 3};
    auto res = into_vector(drop_while(free_lt4), v);
    CHECK(res == (decltype(res){4, 3}));
}

TEST_CASE("drop_while, compose")
{
    using namespace std::placeholders;
    auto v   = std::vector<int>{1, 2, 3, 4, 5};
    auto lt3 = std::bind(std::less<>{}, _1, 3);
    auto res = into_vector(comp(drop_while(lt3), take(2)), v);
    CHECK(res == (decltype(res){3, 4}));
}
