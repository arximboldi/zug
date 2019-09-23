//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/cat.hpp>
#include <zug/transducer/map.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Similar to clojure.core/mapcat$1
 */
template <typename MappingT>
auto mapcat(MappingT&& mapping)
{
    return comp(cat, map(std::forward<MappingT>(mapping)));
}

} // namespace zug
