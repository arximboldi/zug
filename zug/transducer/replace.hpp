//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/map.hpp>

namespace zug {

namespace detail {

template <typename TableT>
struct lookup_or_key
{
    TableT table;

    template <typename KeyT>
    auto operator()(const KeyT& k) -> const KeyT&
    {
        auto elem = table.find(k);
        return elem == table.end() ? k : elem->second;
    }

    template <typename K1T, typename K2T, typename... KTs>
    auto operator()(K1T&& k1, K2T&& k2, KTs&&... ks)
    {
        auto key  = std::make_tuple(std::forward<K1T>(k1),
                                   std::forward<K2T>(k2),
                                   std::forward<KTs>(ks)...);
        auto elem = table.find(key);
        return elem == table.end() ? key : elem->second;
    }
};

} // namespace detail

/*!
 * Similar to clojure.core/replace$1
 */
template <typename TableT>
auto replace(TableT&& table)
{
    return map(detail::lookup_or_key<std::decay_t<TableT>>{table});
}

/*!
 * Transducer that replaces all elements by `table[tuplify(inputs)]`
 */
template <typename TableT>
auto replace_all(TableT&& table)
{
    return map(
        [=](auto&&... ks) mutable { return table[tuplify(ZUG_FWD(ks)...)]; });
}

/*!
 * Transducer that replaces all elements by `table.at(tuplify(inputs))`
 */
template <typename TableT>
auto replace_all_safe(TableT&& table)
{
    return map([=](auto&&... ks) { return table.at(tuplify(ZUG_FWD(ks)...)); });
}

} // namespace zug
