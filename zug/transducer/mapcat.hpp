//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/transducer/cat.hpp>
#include <zug/transducer/map.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Equivalent to `cat | map(mapping)`.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/mapcat.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename MappingT>
auto mapcat(MappingT&& mapping)
{
    return cat | map(std::forward<MappingT>(mapping));
}

} // namespace zug
