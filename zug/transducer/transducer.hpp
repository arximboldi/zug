//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/any_state.hpp>
#include <zug/compose.hpp>
#include <zug/meta.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>
#include <zug/with_state.hpp>

#include <cassert>
#include <functional>

namespace zug {

namespace detail {

template <typename CompleteT, typename ReduceT>
struct transducer_tag
{
    using complete_t = CompleteT;
    using reduce_t   = ReduceT;
};

template <typename C, typename R, typename T>
auto state_wrapper_complete(transducer_tag<C, R>, T&& wrapper) -> C
{
    return state_complete(state_unwrap(std::forward<T>(wrapper)))
        .template as<C>();
}

template <typename C, typename R, typename T>
auto state_wrapper_unwrap_all(transducer_tag<C, R>, T&& wrapper) -> C
{
    return state_unwrap_all(state_unwrap(std::forward<T>(wrapper)))
        .template as<C>();
}

template <typename C, typename R, typename T, typename U>
auto state_wrapper_rewrap(transducer_tag<C, R>, T&& s, U&& x) -> std::decay_t<T>
{
    static_assert(std::is_same<std::decay_t<U>, C>{} ||
                      std::is_same<std::decay_t<U>, any_state>{},
                  "Yo! you are rewrapping with the wrong thing!");
    return wrap_state<transducer_tag<C, R>>(
        state_rewrap(state_unwrap(std::forward<T>(s)), std::forward<U>(x)),
        state_wrapper_data(std::forward<T>(s)));
}

// this is the actual transducer that is used at the end of the chain
template <typename TagT, typename StepT>
auto from_any_state(StepT&& step)
{
    return [=](any_state st, auto&&... ins) mutable {
        using reduce_t   = typename TagT::reduce_t;
        using complete_t = typename TagT::complete_t;
        if (st.has<reduce_t>()) {
            auto next = step(std::move(st).as<reduce_t>(), ZUG_FWD(ins)...);
            st        = std::move(next);
        } else if (st.has<complete_t>()) {
            auto next = step(std::move(st).as<complete_t>(), ZUG_FWD(ins)...);
            st        = std::move(next);
        } else {
            assert(!"oops!");
        }
        return st;
    };
}

template <typename... ArgTs>
struct get_reducing_fn
{
    using type = std::function<any_state(any_state, ArgTs...)>;
};

} // namespace detail

/*!
 * Type erased transducer.
 *
 * This allows to store transducers in places where the full type can not be
 * known at compile time.  The `InputT` template argument is the type of the
 * input over which you may apply the transducer.  For example:
 *
 * @code{.cpp}
 * transducer<int> filter_odd = filter([] (int x) {
 *     return x % 2;
 * });
 * @endcode
 *
 * A second template argument can be passed to indicate the type of data after
 * running through the transducer.  By default, as in the previous example, it's
 * the same as the input, but it does not have to be:
 *
 * @code{.cpp}
 * transducer<int, std::string> serialize = map([] (int x) {
 *     return std::to_string(x);
 * });
 * @endcode
 *
 * Note that, both the first or second template arguments can take a
 * `meta::pack` when it can take or pass more than one input type.
 *
 * @code{.cpp}
 * transducer<pack<int, int>, std::tuple<int, int>> xf1 = zip;
 * transducer<std::tuple<int, int>, pack<int, int>> xf2 = unzip;
 * transducer<pack<int, int>, float> xf3 = map([] (int a, int b) {
 *     return float(a) / b;
 * });
 * @endcode
 *
 * Like any other transducer, a type erased one can be composed with other
 * transducers that have compatible signatures:
 *
 * @code{.cpp}
 * auto x1 = transducer<int>{...};
 * auto x2 = transducer<int, pack<float, std::string>>{...};
 * auto x3 = x1 | x2 | map([] (float x, std::string s) {
 *     return std::to_string(x) + s;
 * });
 * // Note that the result of composing type erased transducers is not a type
 * // erased one.  We can however still type erase the result:
 * auto x4 = transducer<int, std::string>{x3};
 * @endcode
 *
 * @rst
 *
 * .. warning:: Type erased transducers do have a performance cost.  Not only is
 *    it slower to pass them around, they are significantly slower during
 *    processing of the sequence.  For such, use them only when really needed,
 *    and otherwise use `auto` and templates to avoid erasing the types of the
 *    transducers.
 *
 * .. note:: A type erased transducer actually defers applying the held
 *    transducer until it first runs through a sequence, as ilustrated by this
 *    example:
 *
 *    .. code-block:: c++
 *
 *       transducer<int> filter_odd = [](auto step) {
 *           std::cout << "Building step" << std::endl;
 *           return [](auto st, int x) {
 *               return x % 2 ? step(st, x) : st;
 *           };
 *       };
 *       // Doesn't print anything
 *       auto step = filter_odd(std::plus<>{});
 *       // Now it prints
 *       auto sum = reduce(step, 0, {1, 2, 3})
 *
 *    This should normally have no implications. All transducers in the
 *    library perform no side effects when applying a *reducing function*.
 *
 * @endrst
 */
template <typename InputT = meta::pack<>, typename OutputT = InputT>
class transducer : detail::pipeable
{
public:
    using in_step_t  = meta::unpack_t<detail::get_reducing_fn, InputT>;
    using out_step_t = meta::unpack_t<detail::get_reducing_fn, OutputT>;
    using xform_t    = std::function<in_step_t(out_step_t)>;

    template <typename... OutputTs>
    struct get_transducer_state
    {
        template <typename StateT, typename ReducingFnT, typename XformT>
        struct apply
        {
            template <typename WrappedT>
            struct get_state_wrapper
            {
                using xformed_t = typename XformT::result_type;
                using reduce_t  = std::decay_t<
                    std::result_of_t<ReducingFnT(StateT, OutputTs...)>>;
                using complete_t = std::decay_t<decltype(
                    state_complete(std::declval<reduce_t>()))>;
                using tag_t      = detail::transducer_tag<complete_t, reduce_t>;
                using type       = state_wrapper<tag_t, WrappedT, xformed_t>;
            };

            using type = typename std::conditional_t<
                is_state_wrapper<StateT>::value,
                meta::identity<std::decay_t<StateT>>,
                get_state_wrapper<any_state>>::type;
        };
    };

    template <typename StateT, typename ReducingFnT, typename XformT>
    using transducer_state =
        typename meta::unpack<get_transducer_state, OutputT>::
            template apply<StateT, ReducingFnT, XformT>::type;

public:
    transducer(xform_t xform = {})
        : xform_{std::move(xform)}
    {}

    template <typename ReducingFn>
    auto operator()(ReducingFn&& step)
    {
        using step_t = std::decay_t<ReducingFn>;

        return [step, xform = xform_](auto st, auto&&... ins) mutable {
            using state_t   = decltype(st);
            using wrapped_t = transducer_state<state_t, step_t, xform_t>;
            using tag_t     = typename wrapped_t::tag;

            return with_state(
                std::move(st),
                [&](auto&& sst) {
                    auto xformed = xform(detail::from_any_state<tag_t>(step));
                    auto next    = xformed(ZUG_FWD(sst), ZUG_FWD(ins)...);
                    return wrap_state<tag_t>(std::move(next),
                                             std::move(xformed));
                },
                [&](auto&& sst) {
                    auto next = state_wrapper_data(ZUG_FWD(sst))(
                        std::move(state_unwrap(ZUG_FWD(sst))), ZUG_FWD(ins)...);
                    state_unwrap(sst) = std::move(next);
                    return std::move(sst);
                });
        };
    }

private:
    xform_t xform_;
};

} // namespace zug
