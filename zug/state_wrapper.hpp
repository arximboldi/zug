//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/state_traits.hpp>

#include <tuple>

namespace zug {

//! @defgroup state_wrapper
//! @{

/*!
 * Default tag for `state_wrapper`.
 */
struct no_tag
{};

/*!
 * A decorator for the accumulator of a reduction.
 *
 * A transducer must not make any assumptions about the accumulator of
 * the process it is helping to reduce.  However, a *stateful
 * transducer* may use a `state_wrapper` to attach extra data to the
 * accumulator such that the reducing function object itself can be state-less.
 *
 * A state wrapper has the following template arguments:
 *
 * @tparam TagT is as tag type used to identify the transducer that is
 *        attaching the state.  This can useful to specialize the
 *        `state_traits` for a concrete state wrapper.
 *
 * @tparam StateT is the accumulator that is to be wrapped and of which
 *        we shall make no assumptions.
 *
 * @tparam DataT is the additional data that the transducer wants to
 *        attach to represent its own state.
 *
 * For an example of a stateful reducing function, see take.
 */
template <typename TagT = no_tag, typename StateT = void, typename DataT = void>
struct state_wrapper : std::tuple<StateT, DataT>
{
    using tag    = TagT;
    using base_t = std::tuple<StateT, DataT>;

    state_wrapper(const state_wrapper&) = default;
    state_wrapper(state_wrapper&&)      = default;
    state_wrapper& operator=(const state_wrapper&) = default;
    state_wrapper& operator=(state_wrapper&&) = default;

    template <typename T, typename U>
    state_wrapper(T&& st, U&& data)
        : base_t(std::forward<T>(st), std::forward<U>(data))
    {}
};

/*!
 * Metafunction returning whether `StateT` is a, or reference to, a
 * `state_wrapper` instantiation.
 */
template <typename StateT, typename DecayedT = std::decay_t<StateT>>
struct is_state_wrapper : std::false_type
{};

template <typename _, typename T, typename S, typename D>
struct is_state_wrapper<_, state_wrapper<T, S, D>> : std::true_type
{};

/*!
 * Given a tag `TagT` and a state `next` and associated `data`,
 * returns a `state_wrapper` instance.
 */
template <typename TagT = no_tag, typename StateT, typename DataT = TagT>
auto wrap_state(StateT&& next, DataT&& data = DataT{})
    -> state_wrapper<TagT, std::decay_t<StateT>, std::decay_t<DataT>>
{
    return state_wrapper<TagT, std::decay_t<StateT>, std::decay_t<DataT>>{
        std::forward<StateT>(next), std::forward<DataT>(data)};
}

/*!
 * Utility function for easy overloading of `state_traits::unwrap`
 * for state wrappers with a specific tag.
 */
template <typename TagT, typename T>
decltype(auto) state_wrapper_unwrap(TagT, T&& s)
{
    return std::get<0>(std::forward<T>(s));
}

/*!
 * Utility function for easy overloading of `state_traits::complete`
 * for state wrappers with a specific tag.
 */
template <typename TagT, typename T>
decltype(auto) state_wrapper_complete(TagT, T&& s)
{
    return state_complete(state_unwrap(std::forward<T>(s)));
}

/*!
 * Utility function for easy overloading of `state_traits::unwrap_all`
 * for state wrappers with a specific tag.
 */
template <typename TagT, typename T>
decltype(auto) state_wrapper_unwrap_all(TagT, T&& s)
{
    return state_unwrap_all(state_unwrap(std::forward<T>(s)));
}

/*!
 * Utility function for easy overloading of `state_traits::rewrap`
 * for state wrappers with a specific tag.
 */
template <typename TagT, typename T, typename U>
decltype(auto) state_wrapper_rewrap(TagT, T&& s, U&& x)
{
    return wrap_state<TagT>(state_rewrap(state_unwrap(std::forward<T>(s)), x),
                            state_wrapper_data(std::forward<T>(s)));
}

/*!
 * Utility function for easy overloading of `state_traits::data`
 * for state wrappers with a specific tag.
 */
template <typename TagT, typename T, typename D>
decltype(auto) state_wrapper_data(TagT tag, T&& s, D&&)
{
    return state_wrapper_data(tag, std::forward<T>(s));
}

template <typename T>
decltype(auto) state_wrapper_data(T&& s)
{
    return state_wrapper_data(typename std::decay_t<T>::tag{},
                              std::forward<T>(s));
}

template <typename TagT, typename T>
decltype(auto) state_wrapper_data(TagT, T&& s)
{
    return std::get<1>(std::forward<T>(s));
}

/*!
 * Utility function that returns whether the `DataT` associated with a
 * state wrapper with tag `TagT` is reduced -- i.e. idempotent.
 * Can be overloaded custom tags.
 */
template <typename TagT, typename DataT>
bool state_wrapper_data_is_reduced(TagT, DataT&&)
{
    return false;
}

/*!
 * Utility function for easy overloading of `state_traits::is_reduced`
 * for state wrappers with a specific tag.  Most of the time you may
 * want to overload `state_wrapper_is_reduced` instead.
 */
template <typename TagT, typename T>
bool state_wrapper_is_reduced(TagT tag, T&& s)
{
    return state_wrapper_data_is_reduced(
               tag, state_wrapper_data(tag, std::forward<T>(s))) ||
           state_is_reduced(state_unwrap(std::forward<T>(s)));
}

/*!
 * State traits specialization for `state_wrapper`.  Just forwards to the
 * `state_wrapper_*` methods, that are easier to specialize for a given tag.
 */
template <typename TagT, typename StateT, typename DataT>
struct state_traits<state_wrapper<TagT, StateT, DataT>>
{
    template <typename T>
    static decltype(auto) complete(T&& s)
    {
        return state_wrapper_complete(TagT{}, std::forward<T>(s));
    }

    template <typename T>
    static bool is_reduced(T&& s)
    {
        return state_wrapper_is_reduced(TagT{}, std::forward<T>(s));
    }

    template <typename T, typename D>
    static decltype(auto) data(T&& s, D&& d)
    {
        return state_wrapper_data(
            TagT{}, std::forward<T>(s), std::forward<D>(d));
    }

    template <typename T>
    static decltype(auto) unwrap(T&& s)
    {
        return state_wrapper_unwrap(TagT{}, std::forward<T>(s));
    }

    template <typename T>
    static decltype(auto) unwrap_all(T&& s)
    {
        return state_wrapper_unwrap_all(TagT{}, std::forward<T>(s));
    }

    template <typename T, typename U>
    static decltype(auto) rewrap(T&& s, U&& x)
    {
        return state_wrapper_rewrap(
            TagT{}, std::forward<T>(s), std::forward<U>(x));
    }
};

//! @}

} // namespace zug
