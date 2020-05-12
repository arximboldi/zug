//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/maybe_reduced.hpp>
#include <zug/skip.hpp>
#include <zug/util.hpp>

namespace zug {

namespace detail {

template <typename ValueT, typename InputStreamT>
auto read_(InputStreamT& stream)
{
    return [=, stream_ref = std::ref(stream)](auto&& step) {
        return [=](auto&& s, auto&&... is) mutable {
            ValueT value{};
            auto& stream = stream_ref.get();
            if (stream) {
                stream >> value;
                if (!stream.fail())
                    return not_reduced(call(step,
                                            state_unwrap(ZUG_FWD(s)),
                                            ZUG_FWD(is)...,
                                            std::move(value)));
            }
            return reduced(skip(step,
                                state_unwrap(ZUG_FWD(s)),
                                ZUG_FWD(is)...,
                                std::move(value)));
        };
    };
}

} // namespace detail

/*!
 * Generator transducer that produces a sequence of values of type `T`
 * read from the given `stream` using the `operator >>`.  If more
 * than one type is passed, it reads multiple values producing a `T0,
 * T1, ..., Tn` on each step.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/read.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 *   .. literalinclude:: ../test/transducer/read.cpp
 *      :language: c++
 *      :start-after: // example2 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename T1, typename... Ts, typename InputStreamT>
auto read(InputStreamT& stream)
{
    return comp(detail::read_<T1>(stream), detail::read_<Ts>(stream)...);
}

} // namespace zug
