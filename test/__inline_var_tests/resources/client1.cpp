//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente (and Carl Bussey, maybe?)
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include "client1.hpp"

#include <memory>

namespace zug {
namespace detail {
namespace client1 {

const to_be_inlined* address_of_inline_var()
{
    return std::addressof(inline_var);
}

} // namespace client1
} // namespace detail
} // namespace zug
