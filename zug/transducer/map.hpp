//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/util.hpp>

namespace zug {

template <typename MappingT>
struct map_t
{
    template <typename Step>
    constexpr auto operator()(Step&& step) const
    {
        return [=, mapping = mapping](auto&& s, auto&&... is) mutable {
            return step(ZUG_FWD(s), compat::invoke(mapping, ZUG_FWD(is)...));
        };
    }

    MappingT mapping;
};

/*!
 * Transforms every input using the `mapping` function.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/map.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename MappingT>
constexpr auto map(MappingT&& mapping)
{
    return comp(map_t<std::decay_t<MappingT>>{std::forward<MappingT>(mapping)});
}

} // namespace zug
