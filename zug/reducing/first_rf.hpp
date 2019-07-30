//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

namespace zug {

constexpr auto first_rf = [](auto&& st, auto&&...) -> decltype(st)&&
{
    return std::forward<decltype(st)>(st);
};

} // namespace zug
