//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/any_state.hpp>
#include <zug/meta.hpp>

namespace zug {

/*!
 * Given a value `st` that represents the state of a reduction, this function
 * generically dispatches to the right function `UnwrappedFn` or `WrappedFn`,
 * depending of whether the value is already wrapped or not.  This is, during
 * the first iteration of the reduction, `UnwrappedFn` will be called, from then
 * on, `WrappedFnT` will be called.
 *
 * The signatures should be of the form:
 *
 *   - `UnwrappedFn : A -> B`
 *   - `WrappedFn   : B -> B`
 *
 * This function can dispatch both statically and dynamically in a transparent
 * way.  It is thus very useful for writing stateful transducers that can be
 * type erased in a `transducer<>` object.
 */
template <typename StateT, typename UnwrappedFn, typename WrappedFn>
auto with_state(StateT&& st, UnwrappedFn&&, WrappedFn&& fn)
    -> meta::lazy_enable_if_t<
        !std::is_same<std::decay_t<StateT>,
                      std::decay_t<decltype(state_complete(st))>>::value,
        std::result_of<WrappedFn(StateT)>>
{
    return std::forward<WrappedFn>(fn)(std::forward<StateT>(st));
};

template <typename StateT, typename UnwrappedFn, typename WrappedFn>
auto with_state(StateT&& st, UnwrappedFn&& fn, WrappedFn &&)
    -> meta::lazy_enable_if_t<
        !std::is_same<std::decay_t<StateT>, any_state>::value &&
            std::is_same<std::decay_t<StateT>,
                         std::decay_t<decltype(state_complete(st))>>::value,
        std::result_of<UnwrappedFn(StateT)>>
{
    return std::forward<UnwrappedFn>(fn)(std::forward<StateT>(st));
};

template <typename StateT, typename UnwrappedFn, typename WrappedFn>
auto with_state(StateT&& st, UnwrappedFn&& fn1, WrappedFn&& fn2)
    -> meta::lazy_enable_if_t<
        std::is_same<std::decay_t<StateT>, any_state>::value,
        std::result_of<UnwrappedFn(StateT)>>
{
    using wrapped_state_t = std::result_of_t<UnwrappedFn(StateT)>;

    if (!st.template has<wrapped_state_t>()) {
        return std::forward<UnwrappedFn>(fn1)(std::forward<StateT>(st));
    } else {
        return std::forward<WrappedFn>(fn2)(
            std::forward<StateT>(st).template as<wrapped_state_t>());
    }
};

} // namespace zug
