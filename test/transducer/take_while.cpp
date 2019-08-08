//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/take_while.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("take_while, into")
{
    using namespace std::placeholders;
    auto v    = std::vector<int>{1, 2, 3, 4, 5};
    auto pred = std::bind(std::less<>{}, _1, 4);

    auto res = into_vector(take_while(pred), v);
    CHECK(res == (decltype(res){1, 2, 3}));
}

namespace {
bool free_lt4(int x) { return x < 4; }
} // namespace

TEST_CASE("take_while, invoke")
{
    auto v   = std::vector<int>{1, 2, 3, 4, 3};
    auto res = into_vector(take_while(free_lt4), v);
    CHECK(res == (decltype(res){1, 2, 3}));
}

TEST_CASE("take_while, generate with map")
{
    auto count   = int{};
    auto counter = [&] { return count++; };
    auto lt10    = [](int x) { return x < 10; };
    auto res =
        transduce(comp(map(counter), take_while(lt10)), std::plus<int>{}, 1);
    CHECK(res == 46);
}
