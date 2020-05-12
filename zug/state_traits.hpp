//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <type_traits>
#include <utility>

namespace zug {

//! @defgroup state_traits
//! @{

/*!
 * Interface for a type specializing the `State` concept.
 *
 * A `State` is the first parameter of a reducing function, also known as the
 * accumulator.  Every type is a model of `State`, with the following default
 * implementation. However, one might want to specialize the state it for a
 * particular accumulator type, such that transducers can operate with it.  A
 * transducer should not make assumptions about the state it receives, instead,
 * it can only wrap it using `wrap_state` to attach additional data.
 *
 * For an example of a stateful reducing function, see `take`.
 */
template <typename StateT>
struct state_traits
{
    /*!
     * Returns whether the value is idempotent, and thus, the reduction
     * can finish.
     */
    template <typename T>
    static bool is_reduced(T&&)
    {
        return false;
    }

    /*!
     * Returns the associated from the current state.  If the state
     * contains no associated data, the `default_data` will be returned.
     */
    template <typename T, typename D>
    static decltype(auto) data(T&&, D&& d)
    {
        return std::forward<D>(d)();
    }

    /*!
     * Unwraps all the layers of state wrappers returning the deepmost
     */
    template <typename T>
    static T&& complete(T&& state)
    {
        return std::forward<T>(state);
    }

    /*!
     * Unwraps this layers of state wrappers, returning the nested
     * state for the next reducing function.
     */
    template <typename T>
    static T&& unwrap(T&& state)
    {
        return std::forward<T>(state);
    }

    /*!
     * Unwraps all layers of state wrappers, returning the most nested
     * state for the innermost reducing function.
     */
    template <typename T>
    static T&& unwrap_all(T&& state)
    {
        return std::forward<T>(state);
    }

    /*!
     * Copies all layers of state wrappers but replaces the innermost
     * state with `substate`.
     */
    template <typename T, typename U>
    static U&& rewrap(T&&, U&& x)
    {
        return std::forward<U>(x);
    }
};

template <typename T>
using state_traits_t = state_traits<std::decay_t<T>>;

/*!
 * Convenience function for calling `state_traits::complete`
 */
template <typename T>
decltype(auto) state_complete(T&& s)
{
    return state_traits_t<T>::complete(std::forward<T>(s));
}

/*!
 * Convenience function for calling `state_traits::is_reduced`
 */
template <typename T>
bool state_is_reduced(T&& s)
{
    return state_traits_t<T>::is_reduced(std::forward<T>(s));
}

/*!
 * Convenience function for calling `state_traits::data`
 */
template <typename T, typename D>
decltype(auto) state_data(T&& s, D&& d)
{
    return state_traits_t<T>::data(std::forward<T>(s), std::forward<D>(d));
}

/*!
 * Convenience function for calling `state_traits::unwrap`
 */
template <typename T>
decltype(auto) state_unwrap(T&& s)
{
    return state_traits_t<T>::unwrap(std::forward<T>(s));
}

/*!
 * Convenience function for calling `state_traits::unwrap_all`
 */
template <typename T>
decltype(auto) state_unwrap_all(T&& s)
{
    return state_traits_t<T>::unwrap_all(std::forward<T>(s));
}

/*!
 * Convenience function for calling `state_traits::unwrap_all`
 */
template <typename T, typename U>
decltype(auto) state_rewrap(T&& s, U&& x)
{
    return state_traits_t<T>::rewrap(std::forward<T>(s), std::forward<U>(x));
}

//! @}

} // namespace zug
