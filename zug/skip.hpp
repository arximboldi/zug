//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/meta/common_type.hpp>
#include <zug/state_traits.hpp>

#include <functional>

#if !defined(ZUG_VARIANT_STD) && !defined(ZUG_VARIANT_BOOST)
#if __cplusplus >= 201703L
#define ZUG_VARIANT_STD 1
#else
#define ZUG_VARIANT_BOOST 1
#endif
#endif

#if ZUG_VARIANT_STD
#include <variant>
#define ZUG_VARIANT ::std::variant
#define ZUG_VISIT ::std::visit
#elif ZUG_VARIANT_BOOST
#include <boost/variant.hpp>
#define ZUG_VARIANT ::boost::variant
#define ZUG_VISIT ::boost::apply_visitor
#else
#error "We require an implementation of variant!"
#endif

namespace zug {

//! @defgroup skip
//! @{

/*!
 * Type-safe union type that can hold values of both `SkippedT` and `CalledT`
 * types.
 */
template <typename SkippedT, typename CalledT>
struct skip_state : ZUG_VARIANT<SkippedT, CalledT>
{
    using skipped_t = SkippedT;
    using called_t  = CalledT;

    using base_t = ZUG_VARIANT<SkippedT, CalledT>;
    using base_t::base_t;
};

template <typename T>
struct is_skip_state : std::false_type
{};

template <typename SkippedT, typename CalledT>
struct is_skip_state<skip_state<SkippedT, CalledT>> : std::true_type
{};

template <typename SkippedT, typename CalledT>
struct state_traits<skip_state<SkippedT, CalledT>>
{
    template <typename T>
    static decltype(auto) is_reduced(T&& s)
    {
        return ZUG_VISIT([](auto&& x) { return state_is_reduced(ZUG_FWD(x)); },
                         std::forward<T>(s));
    }

    template <typename T>
    static decltype(auto) complete(T&& s)
    {
        return ZUG_VISIT(
            [](auto&& x) -> SkippedT { return state_complete(ZUG_FWD(x)); },
            std::forward<T>(s));
    }

    template <typename T>
    static decltype(auto) unwrap_all(T&& s)
    {
        return ZUG_VISIT(
            [](auto&& x) -> SkippedT { return state_unwrap_all(ZUG_FWD(x)); },
            std::forward<T>(s));
    }

    template <typename T, typename U>
    static decltype(auto) rewrap(T&& s, U&& y)
    {
        return ZUG_VISIT(
            [&](auto&& x) -> std::decay_t<T> {
                return state_rewrap(ZUG_FWD(x), std::forward<U>(y));
            },
            std::forward<T>(s));
    }

    // Marks methods that don't make sense for a `skip_state` Note that
    // these methods still need to be implemented for type erasure to
    // work (via `any_state`).
    struct can_not_do_that
    {};
    template <typename T, typename D>
    static can_not_do_that data(T&&, D&&)
    {
        return {};
    }
    template <typename T>
    static can_not_do_that unwrap(T&&)
    {
        return {};
    }
};

//! @}

namespace detail {

template <typename ReducingFnT, typename StateT, typename... InputTs>
struct skip_result_impl
{
    using skipped_t = StateT;
#ifdef __cpp_lib_is_invocable
    using called_t  = std::invoke_result_t<ReducingFnT, StateT, InputTs...>;
#else
    using called_t  = std::result_of_t<ReducingFnT(StateT, InputTs...)>;
#endif
    using common_type_t = meta::common_type_t<skipped_t, called_t>;
    using error_t       = meta::could_not_find_common_type<skipped_t, called_t>;

    using type = std::conditional_t<
        !std::is_same<common_type_t, error_t>::value,
        common_type_t,
        skip_state<std::decay_t<skipped_t>, std::decay_t<called_t>>>;
};

} // namespace detail

//! @defgroup skip
//! @{

/*!
 * Metafunction that returns a type that can hold both values of type
 *   `skipped_t = StateT`
 * and
 *   `wrapped_t = declval<ReducingFnT>(declval<StateT>(),
 *                                     declval<InputTs>()...)`
 *
 * The result is:
 *
 *   - If a `std::common_type<skipped_t, wrapped_t>` exists between
 *     the two types, it returns this type.  For example, if one of
 *     these is a `any_state` because is used in a type erased
 *     transducer, `any_state` will be the result.
 *
 *   - Otherwise, it returns `skip_state<skipped_t, wrapped_t>`, which
 *     is essentially a type-safe union of these two types.
 */
template <typename ReducingFnT, typename StateT, typename... InputTs>
struct skip_result
    : std::conditional_t<
          is_skip_state<std::decay_t<StateT>>::value,
          meta::identity<std::decay_t<StateT>>,
          detail::skip_result_impl<ReducingFnT, StateT, InputTs...>>
{};

template <typename ReducingFnT, typename StateT, typename... InputTs>
using skip_result_t =
    typename skip_result<ReducingFnT, StateT, InputTs...>::type;

/*!
 * Skip calling the next reducing function in a transducer.  Returns the `state`
 * parameter, potentially wrapped in a value convertible to/from whatever `call`
 * would return for similar parameters.
 *
 * A transducer might or might not call the next reducing function in the chain.
 * One good example is `filter`, where if the predicate passes, it calls the
 * next reducing function, otherwise it just returns the current state of the
 * reduction.  However, this poses the question: what should be the return type
 * of such a transducer?  The next reducing function might wrap the state in a
 * `state_wrapper` to attach its own state to it.  However, we don't know at
 * this point what how to create such a wrapped value.  This method, and `call`,
 * take care of wrapping the state in a type that can hold values of both the
 * current state, and the state that would be returned by the next reducing
 * function. The metafunction `skip_result` defines such a type.
 */
template <typename ReducingFnT, typename StateT, typename... InputTs>
auto skip(ReducingFnT&&, StateT&& state, InputTs&&...)
    -> skip_result_t<ReducingFnT, StateT, InputTs...>
{
    return std::forward<StateT>(state);
}

template <typename StateT>
auto skip(StateT&& state) -> StateT&&
{
    return std::forward<StateT>(state);
}

/*!
 * Call the next reducing function in a transducer that could otherwise skip
 * calling the next reducing function.  Returns the result of calling
 * `step(state, ins...)` wrapped in a type that can also hold plain `state`
 * values.  If `state` is wrapped in such a type, it unwraps it before passing
 * it.
 */
template <typename ReducingFnT, typename StateT, typename... InputTs>
auto call(ReducingFnT&& step, StateT&& state, InputTs&&... ins)
    -> std::enable_if_t<is_skip_state<std::decay_t<StateT>>::value,
                        std::decay_t<StateT>>
{
    return ZUG_VISIT(
        [&](auto&& st) -> std::decay_t<StateT> {
            return std::forward<ReducingFnT>(step)(
                ZUG_FWD(st), std::forward<InputTs>(ins)...);
        },
        std::forward<StateT>(state));
}

template <typename ReducingFnT, typename StateT, typename... InputTs>
auto call(ReducingFnT&& step, StateT&& state, InputTs&&... ins)
    -> std::enable_if_t<!is_skip_state<std::decay_t<StateT>>::value,
                        skip_result_t<ReducingFnT, StateT, InputTs...>>
{
    return std::forward<ReducingFnT>(step)(std::forward<StateT>(state),
                                           std::forward<InputTs>(ins)...);
}

//! @}

} // namespace zug
