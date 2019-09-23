//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/into_vector.hpp>
#include <zug/transducer/filter.hpp>
#include <zug/transducer/map.hpp>
#include <zug/transducer/transducer.hpp>

#include <catch2/catch.hpp>

using namespace zug;

namespace test_detail {

struct to_string_t
{
    std::string operator()(int x) const;
};
struct stoi_t
{
    int operator()(std::string x) const;
};

} // namespace test_detail

TEST_CASE("into_vector, type deduction")
{
    using namespace test_detail;

    static_assert(
        std::is_same<std::vector<int>,
                     decltype(into_vector(identity, std::vector<int>{}))>{},
        "");
    static_assert(
        std::is_same<std::vector<std::tuple<int, std::string>>,
                     decltype(into_vector(identity,
                                          std::vector<int>{},
                                          std::vector<std::string>{}))>{},
        "");
    static_assert(std::is_same<std::vector<std::string>,
                               decltype(into_vector(map(to_string_t{}),
                                                    std::vector<int>{}))>{},
                  "");
    static_assert(
        std::is_same<std::vector<int>,
                     decltype(into_vector(map(stoi_t{}),
                                          std::vector<std::string>{}))>{},
        "");
}

TEST_CASE("into_vector, zipping")
{
    auto v1   = std::vector<int>{1, 2, 3, 4};
    auto v2   = std::vector<std::string>{"a", "b"};
    using tup = std::tuple<int, std::string>;

    auto res = into_vector(identity, v1, v2);
    CHECK(res == (std::vector<tup>{tup(1, "a"), tup(2, "b")}));
}

TEST_CASE("into_vector, transduction")
{
    auto v = std::vector<int>{1, 2, 3, 4};

    auto xform = comp(filter([](int x) { return x % 2 == 0; }),
                      map([](int x) { return std::to_string(x); }));
    auto res   = into_vector(xform, v);
    CHECK(res == (std::vector<std::string>{"2", "4"}));
}

TEST_CASE("into_vector, type erasure")
{
    auto v = std::vector<int>{1, 2, 3, 4};

    auto xform = transducer<int, std::string>{
        comp(filter([](int x) { return x % 2 == 0; }),
             map([](int x) { return std::to_string(x); }))};
    auto res = into_vector(xform, v);
    CHECK(res == (std::vector<std::string>{"2", "4"}));
}
