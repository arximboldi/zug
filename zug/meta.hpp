//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/into.hpp>
#include <zug/reducing/last.hpp>
#include <zug/state_traits.hpp>

#include <zug/meta/pack.hpp>
#include <zug/meta/util.hpp>

namespace zug {

namespace detail {

struct output_of_rf_t
{
    template <typename StateT, typename... InputTs>
    constexpr auto operator()(StateT, InputTs&&...) const
        -> meta::pack<InputTs&&...>;
};

} // namespace detail

//! @defgroup meta
//! @{

/*!
 * Metafunction that given a transducer `XformT` and some inputs `InputTs`,
 * returns the type of the outputs of the transducer, wrapped in a `meta::pack`.
 * It preserves reference types.
 */
template <typename XformT, typename... InputTs>
struct output_of
{
    using type = decltype(
        state_complete(std::declval<XformT>()(detail::output_of_rf_t{})(
            std::declval<meta::bottom>(), std::declval<InputTs>()...)));
};

template <typename XformT, typename... InputTs>
struct output_of<XformT, meta::pack<InputTs...>>
{
    using type = typename output_of<XformT, InputTs...>::type;
};

template <typename XformT, typename... InputTs>
using output_of_t = typename output_of<XformT, InputTs...>::type;

/*!
 * Metafunction that given a transducer `XformT` and some inputs `InputTs`,
 * returns the type of the output of the transducer, combined as combined in a
 * single result with `tuplify`.
 */
template <typename XformT, typename... InputTs>
struct result_of
{
    using type = std::decay_t<decltype(state_complete(std::declval<XformT>()(
        last)(std::declval<meta::bottom>(), std::declval<InputTs>()...)))>;
};

template <typename XformT, typename... InputTs>
struct result_of<XformT, meta::pack<InputTs...>>
{
    using type = typename result_of<XformT, InputTs...>::type;
};

template <typename XformT, typename... InputTs>
using result_of_t = typename result_of<XformT, InputTs...>::type;

//! @}

} // namespace zug
