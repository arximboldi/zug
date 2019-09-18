//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente (and Carl Bussey?)
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

namespace zug {
namespace detail {

template <typename T>
struct inline_variable_holder
{
    static constexpr T value {};
};

template <typename T>
constexpr T inline_variable_holder<T>::value;

template <typename T>
constexpr const T& make_inline_var()
{
    return inline_variable_holder<T>::value;
}

} // namespace detail
} // namespace zug
