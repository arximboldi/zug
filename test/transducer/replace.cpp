//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <catch2/catch.hpp>

#include <zug/into_vector.hpp>
#include <zug/transduce.hpp>
#include <zug/transducer/replace.hpp>

#include <map>

using namespace zug;

TEST_CASE("replace, replace")
{
    // example1 {
    auto table = std::map<std::string, std::string>{{"hola", "adios"}};
    auto v     = std::vector<std::string>{"hola", " ", "amigo"};
    CHECK(transduce(replace(table), std::plus<>{}, std::string{}, v) ==
          "adios amigo");
    // }
}

TEST_CASE("replace, replace variadic")
{
    using tup = std::tuple<int, std::string>;

    auto table = std::map<tup, tup>{{tup(1, "hola"), tup(42, "adios")}};

    auto v1 = std::vector<int>{1, 2, 3};
    auto v2 = std::vector<std::string>{"hola", " ", "amigo"};

    auto res = into_vector(replace(table), v1, v2);
    CHECK(res ==
          (decltype(res){tup(42, "adios"), tup(2, " "), tup(3, "amigo")}));
}

TEST_CASE("replace, replace all")
{
    // example2 {
    auto table = std::map<std::string, int>{{"hola", 1}, {"amigo", 2}};
    auto v     = std::vector<std::string>{"hola", " ", "amigo"};
    CHECK(transduce(replace_all(table), std::plus<>{}, int{}, v) == 3);
    // }
}

TEST_CASE("replace, replace safe pass")
{
    // example3 {
    auto table = std::map<std::string, int>{{"hola", 1}, {"amigo", 2}};
    auto v     = std::vector<std::string>{"hola", "amigo"};
    CHECK(transduce(replace_all_safe(table), std::plus<>{}, int{}, v) == 3);
    // }
}

TEST_CASE("replace, replace safe exception")
{
    // example4 {
    auto table = std::map<std::string, int>{{"hola", 12}, {"amigo", 42}};
    auto v     = std::vector<std::string>{"hola", "oops", "amigo"};
    CHECK_THROWS_AS(transduce(replace_all_safe(table), std::plus<>{}, int{}, v),
                    std::out_of_range);
    // }
}
