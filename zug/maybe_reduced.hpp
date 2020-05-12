//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/state_wrapper.hpp>

namespace zug {

//! @defgroup maybe_reduced
//! @{

/*!
 * Tag for `maybe_reduced` state wrapper.
 */
struct maybe_reduced_tag
{};

/*!
 * State wrapper for transducers that may want to signal that the reduction is
 * finished.
 */
template <typename T>
using maybe_reduced = state_wrapper<maybe_reduced_tag, T, bool>;

inline bool state_wrapper_data_is_reduced(maybe_reduced_tag, bool is_reduced)
{
    return is_reduced;
}

/*!
 * Wraps `x` in a `maybe_reduced`, where `is_reduced` contains whether the
 * reduction should actually finish.
 */
template <typename T>
auto reduced_if(T&& x, bool is_reduced) -> maybe_reduced<std::decay_t<T>>
{
    return maybe_reduced<std::decay_t<T>>{std::forward<T>(x), is_reduced};
}

/*!
 * Wraps `x` such that the reduction should finish.
 */
template <typename T>
decltype(auto) reduced(T&& x)
{
    return reduced_if(std::forward<T>(x), true);
}

/*!
 * Wraps `x` such that the reduction should continue.
 */
template <typename T>
decltype(auto) not_reduced(T&& x)
{
    return reduced_if(std::forward<T>(x), false);
}

//! @}

} // namespace zug
