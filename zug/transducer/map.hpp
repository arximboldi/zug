//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/util.hpp>

namespace zug {

/*!
 * Similar to clojure.core/map$1
 */
template <typename MappingT>
constexpr auto map(MappingT&& mapping)
{
    return [=](auto step) {
        return [=, mapping = mapping](auto&& s, auto&&... is) mutable {
            return step(ZUG_FWD(s), compat::invoke(mapping, ZUG_FWD(is)...));
        };
    };
}

} // namespace zug
