//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/map.hpp>
#include <zug/tuplify.hpp>

namespace zug {

/*!
 * Transducer that, if more than one argument is passed, forwards all
 * of them as a single tuple.
 *
 * @todo This should a `constexpr` in C++14
 */
auto zip = map(tuplify);

} // namespace zug
