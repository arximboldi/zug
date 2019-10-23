//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/pipeable.hpp>
#include <zug/util.hpp>

#include <utility>

namespace zug {

namespace detail {

template <typename MappingT>
struct map
{
    MappingT mapping;

    template <typename Step>
    auto operator()(Step&& step) const
    {
        return [=, mapping = mapping](auto&& s, auto&&... is) mutable {
            return step(ZUG_FWD(s), compat::invoke(mapping, ZUG_FWD(is)...));
        };
    };
};

} // namespace detail

/*!
 * Similar to clojure.core/map$1
 */
template <typename MappingT>
constexpr auto map(MappingT&& mapping)
{
    return make_pipeable(
        detail::map<std::decay_t<MappingT>>{std::forward<MappingT>(mapping)});
}

} // namespace zug
