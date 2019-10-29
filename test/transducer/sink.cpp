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
#include <zug/run.hpp>
#include <zug/transducer/enumerate.hpp>
#include <zug/transducer/sink.hpp>

#include "../spies.hpp"

using namespace zug;

TEST_CASE("sink, sink")
{
    auto v = std::vector<int>{1, 2, 3, 6};
    auto r = std::vector<int>{};
    run(sink([&](int x) { r.push_back(x); }), v);
    CHECK(v == r);
}

TEST_CASE("sink, moves values out of rvalue_container")
{
    using elem = testing::copy_spy<>;

    auto x      = elem{};
    auto v      = std::vector<elem>{x, x, x, x};
    auto copies = x.copied.count();

    auto r = into_vector(comp(sink([](elem) {}), enumerate), std::move(v));

    CHECK(x.copied.count() == copies);
    CHECK(r == (decltype(r){0, 1, 2, 3}));
}
