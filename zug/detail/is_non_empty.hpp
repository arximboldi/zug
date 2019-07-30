//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <iterator>

namespace zug {
namespace detail {

inline bool is_non_empty() { return true; }

template <typename RangeT, typename... RangeTs>
bool is_non_empty(const RangeT& r, const RangeTs&... rs)
{
    using std::begin;
    using std::end;
    return begin(r) != end(r) && is_non_empty(rs...);
}

} // namespace detail
} // namespace zug
