//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta.hpp>

#include <vector>

namespace zug {

/*!
 * Transduces the input `ranges` using `xform`, storing the results in a vector
 * that is returned.
 *
 * The value type of the vector is automatically deduced from the application of
 * the transducer to the input ranges.  If there are multiple arguments in the
 * outputs from the transducer, they are combined in an `std::tuple`.
 */
template <typename XformT, typename... InputRangeTs>
auto into_vector(XformT&& xform, InputRangeTs&&... ranges) -> std::vector<
    result_of_t<XformT, typename std::decay_t<InputRangeTs>::value_type...>>
{
    using result_t = std::vector<
        result_of_t<XformT,
                    typename std::decay_t<InputRangeTs>::value_type...>>;

    return into(result_t{},
                std::forward<XformT>(xform),
                std::forward<InputRangeTs>(ranges)...);
}

} // namespace zug
