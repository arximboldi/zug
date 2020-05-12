//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/skip.hpp>
#include <zug/state_traits.hpp>

namespace zug {

/*!
 * Common reductor interface.  This is an abstract type, use `reductor` and
 * `empty_reductor` concrete types.
 */
template <typename ReducingFnT, typename StateT, typename... InputTs>
struct reductor_base
{
    using reducing_fn_type = ReducingFnT;
    using state_type       = StateT;
    using complete_type =
        std::decay_t<decltype(state_complete(std::declval<state_type>()))>;

    reductor_base() = delete;

    /*!
     * Returns whether the reductor will produce more values.
     */
    explicit operator bool() const { return !state_is_reduced(state_); }

    /*!
     * Completes and returns the state of the reduction.  The operation
     * os move aware.
     */
    complete_type complete() const& { return state_complete(state_); }
    complete_type complete() && { return state_complete(std::move(state_)); }

    /*!
     * Peeks at the current visible state of the reduction.  The operation is
     * move aware.
     */
    complete_type current() const& { return state_unwrap_all(state_); }
    complete_type current() && { return state_unwrap_all(std::move(state_)); }

    /*!
     * Changes the current visible state of the reduction to be the value `x`;
     */
    template <typename T>
    void current(T&& x)
    {
        state_ = state_rewrap(std::move(state_), std::forward<T>(x));
    }

    /*!
     * Evaluates the next step of the reduction, passing the inputs `ins` to the
     * reducing function.
     *
     * @note When operated on a `const` object, the it returns a new
     *       reductor object.  Otherwise, the operation is performed
     *       in-place and the object itself is returned.  The operation
     *       is move-aware.
     */
    template <typename... InputTs2>
    reductor_base& operator()(InputTs2&&... ins) &
    {
        auto next = step_(std::move(state_), std::forward<InputTs2>(ins)...);
        state_    = std::move(next);
        return *this;
    }

    template <typename... InputTs2>
    reductor_base operator()(InputTs2&&... ins) const&
    {
        auto copied = *this;
        copied(std::forward<InputTs2>(ins)...);
        return copied;
    }

    template <typename... InputTs2>
    reductor_base&& operator()(InputTs2&&... ins) &&
    {
        return std::move((*this)(std::forward<InputTs2>(ins)...));
    }

protected:
    template <typename ReducingFnT2, typename StateT2>
    reductor_base(ReducingFnT2&& step, StateT2&& state)
        : step_{std::forward<ReducingFnT2>(step)}
        , state_{std::forward<StateT2>(state)}
    {}

private:
    reducing_fn_type step_;
    state_type state_;
};

//! @defgroup reductor
//! @{

/*!
 * Function object that performs a reduction using a reducing function of type
 * `ReducingFnT`, an initial state of type of type `InitialStateT` and inputs of
 * types `InputTs...`.
 *
 * The function object can be called with arguments converibles to `InputTs...`
 * to step the reduction.  If the reductor object is `const`, it will return a
 * new value representing the advanced state.  Otherwise it performs the
 * reduction in place and returns a reference to itself. See `reductor_base` for
 * details on the interface.
 *
 * This object requires an initial input to be pased to the constructor.  If no
 * input is available at construction time, use `empty_reductor` instead.
 */
template <typename ReducingFnT, typename InitialStateT, typename... InputTs>
struct reductor
    : reductor_base<ReducingFnT,
                    std::result_of_t<ReducingFnT(InitialStateT, InputTs...)>,
                    InputTs...>
{
    using base_t =
        reductor_base<ReducingFnT,
                      std::result_of_t<ReducingFnT(InitialStateT, InputTs...)>,
                      InputTs...>;

    template <typename ReducingFnT2,
              typename InitialStateT2,
              typename... InputTs2>
    reductor(ReducingFnT2&& step, InitialStateT2&& state, InputTs2&&... ins)
        : base_t(std::forward<ReducingFnT2>(step),
                 step(std::forward<InitialStateT2>(state),
                      std::forward<InputTs2>(ins)...))
    {}
};

/*!
 * Constructs a `reductor` object with deduced argument types.
 */
template <typename ReducingFnT, typename InitialStateT, typename... InputTs>
auto make_reductor(ReducingFnT&& step, InitialStateT&& state, InputTs&&... ins)
    -> reductor<std::decay_t<ReducingFnT>,
                std::decay_t<InitialStateT>,
                std::decay_t<InputTs>...>
{
    return {std::forward<ReducingFnT>(step),
            std::forward<InitialStateT>(state),
            std::forward<InputTs>(ins)...};
}

//! @}

namespace detail {

template <typename ReducingFnT>
struct caller
{
    ReducingFnT step;

    template <typename... Args>
    decltype(auto) operator()(Args&&... args)
    {
        return call(step, std::forward<Args>(args)...);
    }
};

} // namespace detail

//! @defgroup reductor
//! @{

/*!
 * Reductor object that does not require inputs to be fed at construction time.
 * In some cases though, `reductor` can perform better.
 */
template <typename ReducingFnT, typename InitialStateT, typename... InputTs>
struct empty_reductor
    : reductor_base<
          detail::caller<std::decay_t<ReducingFnT>>,
          std::decay_t<skip_result_t<ReducingFnT, InitialStateT, InputTs...>>,
          InputTs...>
{
    using base_t = reductor_base<
        detail::caller<std::decay_t<ReducingFnT>>,
        std::decay_t<skip_result_t<ReducingFnT, InitialStateT, InputTs...>>,
        InputTs...>;

    template <typename ReducingFnT2, typename InitialStateT2>
    empty_reductor(ReducingFnT2&& step, InitialStateT2&& state)
        : base_t{
              std::forward<ReducingFnT2>(step),
              typename base_t::state_type{std::forward<InitialStateT2>(state)}}
    {}
};

/*!
 * Constructs an `empty_reductor` object with deduced argument types.  The
 * `InputTs` over which the reductor functions have to be passed explicitly
 * though.
 */
template <typename... InputTs, typename ReducingFnT, typename InitialStateT>
auto make_empty_reductor(ReducingFnT&& step, InitialStateT&& state)
    -> empty_reductor<std::decay_t<ReducingFnT>,
                      std::decay_t<InitialStateT>,
                      std::decay_t<InputTs>...>
{
    return {std::forward<ReducingFnT>(step),
            std::forward<InitialStateT>(state)};
}

//! @}

template <typename ReducingFnT, typename InitialStateT, typename... InputTs>
struct empty_reductor<ReducingFnT, InitialStateT, meta::pack<InputTs...>>
    : empty_reductor<ReducingFnT, InitialStateT, InputTs...>
{
    using empty_reductor<ReducingFnT, InitialStateT, InputTs...>::
        empty_reductor;
};

} // namespace zug
