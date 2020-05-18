//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/detail/empty_transducer_error.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/state_wrapper.hpp>
#include <zug/util.hpp>
#include <zug/with_state.hpp>

namespace zug {

struct chainr_tag
{};

template <typename InputRange>
auto chainr(InputRange range)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            auto data = state_data(
                ZUG_FWD(s), [&] { return std::make_pair(&step, &range); });
            return wrap_state<chainr_tag>(
                (*data.first)(state_unwrap(ZUG_FWD(s)), ZUG_FWD(is)...),
                std::move(data));
        };
    });
}

/*!
 * Transducer produces the sequence passed as parameter _after_ all other input
 * has finished.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example22 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename... InputRangeTs>
auto chainr(InputRangeTs... rs)
{
    return comp(chainr(std::forward<InputRangeTs>(rs))...);
}

template <typename T>
decltype(auto) state_wrapper_complete(chainr_tag, T&& wrapper)
{
    return identity_(state_complete(
        reduce_nested(*state_wrapper_data(std::forward<T>(wrapper)).first,
                      state_unwrap(std::forward<T>(wrapper)),
                      *state_wrapper_data(std::forward<T>(wrapper)).second)));
}

template <typename InputRange>
auto chainl(InputRange range)
{
    return comp([=](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            return with_state(
                ZUG_FWD(s),
                [&](auto&& st) {
                    using std::begin;
                    using std::end;
                    return wrap_state(
                        begin(range) != end(range)
                            ? step(detail::reduce_nested_non_empty(
                                       step, ZUG_FWD(st), range),
                                   ZUG_FWD(is)...)
                            : step(ZUG_FWD(st), ZUG_FWD(is)...));
                },
                [&](auto&& st) {
                    return wrap_state(
                        step(state_unwrap(ZUG_FWD(st)), ZUG_FWD(is)...));
                });
        };
    });
}

/*!
 * Transducer produces the sequence passed as parameter _before_ processing the
 * first input.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example3 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example4 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/chain.cpp
 *      :language: c++
 *      :start-after: // example5 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename... InputRangeTs>
auto chainl(InputRangeTs... rs)
{
    return comp(chainl(std::forward<InputRangeTs>(rs))...);
}

/*!
 * Alias for `chainr`.
 */
template <typename... InputRangeTs>
auto chain(InputRangeTs&&... rs)
{
    return chainr(std::forward<InputRangeTs>(rs)...);
}

} // namespace zug
