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

constexpr auto map = [](auto mapping) {
    return [=](auto step) {
        return [=](auto&& s, auto&&... is) {
            return step(ZUG_FWD(s), invoke(mapping, ZUG_FWD(is)...));
        };
    };
};

} // namespace zug
