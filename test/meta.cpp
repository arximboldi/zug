//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/meta.hpp>
#include <zug/util.hpp>

#include <catch2/catch.hpp>

using namespace zug;

TEST_CASE("output_of, identity")
{
    using meta::pack;
    using identity_t = decltype(identity);

    static_assert(output_of_t<identity_t>{} == pack<>{}, "");
    static_assert(output_of_t<identity_t, int>{} == pack<int&&>{}, "");
    static_assert(
        output_of_t<identity_t, int, float>{} == pack<int&&, float&&>{}, "");

    static_assert(output_of_t<identity_t, meta::pack<int, float>>{} ==
                      pack<int&&, float&&>{},
                  "");

    static_assert(output_of_t<identity_t, int&>{} == pack<int&>{}, "");
    static_assert(output_of_t<identity_t, int&, const float&>{} ==
                      pack<int&, const float&>{},
                  "");

    static_assert(output_of_t<identity_t, int&&>{} == pack<int&&>{}, "");
    static_assert(output_of_t<identity_t, const int&&, float&&>{} ==
                      pack<const int&&, float&&>{},
                  "");
}

TEST_CASE("result_of, identity")
{
    using meta::pack;
    using identity_t = decltype(identity);

    static_assert(pack<result_of_t<identity_t>>{} == pack<std::tuple<>>{}, "");
    static_assert(pack<result_of_t<identity_t, int>>{} == pack<int>{}, "");
    static_assert(pack<result_of_t<identity_t, int, float>>{} ==
                      pack<std::tuple<int, float>>{},
                  "");

    static_assert(pack<result_of_t<identity_t, meta::pack<int, float>>>{} ==
                      pack<std::tuple<int, float>>{},
                  "");

    static_assert(pack<result_of_t<identity_t, int&>>{} == pack<int>{}, "");
    static_assert(pack<result_of_t<identity_t, int&, float&>>{} ==
                      pack<std::tuple<int, float>>{},
                  "");
}
