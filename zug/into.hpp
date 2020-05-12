//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#define ZUG_STATEFUL_INTO 1

#include <zug/transduce.hpp>
#if ZUG_STATEFUL_INTO
#include <zug/reducing/output.hpp>
#else
#include <zug/reducing/emplacing_back.hpp>
#endif

namespace zug {

#if ZUG_STATEFUL_INTO
/*!
 * Transduces the input `ranges` using `xform`, storing the results in the
 * collection `col`, which is also returned.
 *
 * The results are stored in `col` using `push_back()`. If transducer has
 * multiple output arguments, they are combined in a `std::tuple`.
 */
template <typename CollectionT, typename XformT, typename... InputRangeTs>
auto into(CollectionT&& col, XformT&& xform, InputRangeTs&&... ranges)
    -> CollectionT&&
{
    transduce(std::forward<XformT>(xform),
              output,
              std::back_inserter(col),
              std::forward<InputRangeTs>(ranges)...);
    return std::forward<CollectionT>(col);
}
#else
template <typename CollectionT, typename XformT, typename... InputRangeTs>
auto into(CollectionT&& col, XformT&& xform, InputRangeTs&&... ranges)
    -> std::decay_t<CollectionT>
{
    return transduce(std::forward<XformT>(xform),
                     emplacing_back,
                     std::forward<CollectionT>(col),
                     std::forward<InputRangeTs>(ranges)...);
}
#endif

} // namespace zug
