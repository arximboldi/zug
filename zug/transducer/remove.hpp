//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/filter.hpp>

namespace zug {

/*!
 * Similar to clojure.core/remove$1
 */
template <typename PredicateT>
auto remove(PredicateT&& pred)
{
    return filter([=](auto&& x) { return !pred(ZUG_FWD(x)); });
}

} // namespace zug
