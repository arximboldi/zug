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

struct emplacing_back_t
{
    template <typename StateT, typename... InputTs>
    decltype(auto) operator()(StateT&& st, InputTs&&... ins) const
    {
        st.emplace_back(ZUG_FWD(ins)...);
        return ZUG_FWD(st);
    }
};

/*!
 * Reducing function that emplaces back at the collection that is
 * uses as state.
 */
ZUG_INLINE_CONSTEXPR emplacing_back_t emplacing_back{};

} // namespace zug
