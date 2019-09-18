//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente (and Carl Bussey, maybe?)
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include "resources/client1.hpp"
#include "resources/client2.hpp"

#include <zug/detail/inline_var.hpp>

#include <catch2/catch.hpp>

using namespace zug::detail;

// static tests
static_assert( 
    std::is_same<decltype(make_inline_var<int>()), const int&>::value,
    "Return type of make_inline_var is a const reference of template type"
);

TEST_CASE("inline_var: variable included in two files has same address")
{
    CHECK( client1::address_of_inline_var() == client2::address_of_inline_var() );
}
