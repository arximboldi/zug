//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/inline_constexpr.hpp>

#include <tuple>

namespace zug {

//! @defgroup util
//! @{

ZUG_INLINE_CONSTEXPR struct tuplify_t
{
    constexpr std::tuple<> operator()() const { return {}; };

    template <typename InputT>
    constexpr decltype(auto) operator()(InputT&& in) const
    {
        return std::forward<InputT>(in);
    }

    template <typename InputT, typename... InputTs>
    constexpr decltype(auto) operator()(InputT&& in, InputTs&&... ins) const
    {
        return std::make_tuple(std::forward<InputT>(in),
                               std::forward<InputTs>(ins)...);
    }
}
/*!
 * Function object that forwards its argument if one single argument passed,
 * otherwise it returns a tuple with all the passed in arguments.
 */
tuplify{};

//! @}

} // namespace zug
