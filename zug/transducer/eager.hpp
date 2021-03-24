//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/tuplify.hpp>
#include <zug/compose.hpp>
#include <zug/detail/iterator_range.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>

#include <vector>

namespace zug {

struct eager_tag
{};

namespace detail {

template <typename Algo, typename StepT>
struct eager_reducing_func_t
{
    Algo algo;
    StepT step;

    template <typename ContainerT>
    struct tuple_maker_t
    {
        eager_reducing_func_t* parent;

        constexpr auto operator()() const
        {
            return std::make_tuple(ContainerT{}, &parent->step, &parent->algo);
        }
    };

    template <typename StateT, typename... ItemT>
    constexpr auto operator()(StateT &&s, ItemT&&... is)
    {
        using container_t =
            std::vector<std::decay_t<decltype(tuplify(is...))>>;
        auto data = state_data(ZUG_FWD(s), tuple_maker_t<container_t>{this});
        std::get<0>(data).push_back(tuplify(ZUG_FWD(is)...));
        return wrap_state<eager_tag>(state_unwrap(ZUG_FWD(s)),
                                     std::move(data));
    }
};

template <typename Algo>
struct eager_uncomposed_transducer_t
{
    Algo algo;

    template <typename StepT>
    constexpr auto operator()(StepT&& step) const
    {
        return eager_reducing_func_t<Algo, std::decay_t<StepT>>{algo, step};
    };
};

}

/*!
 * Transducer that processes the whole sequence with the eager algorithm
 * `algo`. `algo` is a function that takes a vector as an argument a returns a
 * range.  It used to implement other transducers, like `sorted` and `reversed`.
 *
 * Note that this transducer produces no output until completion and uses @f$
 * O(n) @f$ space.
 */
template <typename Algo>
constexpr auto eager(Algo algo)
{
    return comp(detail::eager_uncomposed_transducer_t<Algo>{algo});
}

template <typename T>
decltype(auto) state_wrapper_complete(eager_tag, T&& wrapper)
{
    return identity_(state_complete(reduce_nested(
        *std::get<1>(state_wrapper_data(std::forward<T>(wrapper))),
        state_unwrap(std::forward<T>(wrapper)),
        (*std::get<2>(state_wrapper_data(std::forward<T>(wrapper))))(
            std::get<0>(state_wrapper_data(std::forward<T>(wrapper)))))));
}

struct sorted_t
{
    template <typename RangeT>
    decltype(auto) operator()(RangeT&& range) const
    {
        std::sort(range.begin(), range.end());
        return ZUG_FWD(range);
    }
};

/*!
 * Eager transducer that sorts the input sequence.
 * @rst
 *   .. literalinclude:: ../test/transducer/eager.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto sorted = eager(sorted_t{});


struct reversed_t
{
    template <typename RangeT>
    decltype(auto) operator()(RangeT&& range) const
    {
        return detail::make_iterator_range(range.rbegin(), range.rend());
    }
};

/*!
 * Eager transducer that reverses the input sequence.
 * @rst
 *   .. literalinclude:: ../test/transducer/eager.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
ZUG_INLINE_CONSTEXPR auto reversed = eager(reversed_t{});

} // namespace zug
