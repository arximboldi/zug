//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/maybe_reduced.hpp>
#include <zug/skip.hpp>
#include <zug/util.hpp>

namespace zug {

/*!
 * Generator transducer that produces a sequence of values of type `T`
 * read from the given @a `stream` using the `operator >>`.  If more
 * than one type is passed, it reads multiple values producing `T0,
 * T1, ..., Tn` on each iteration.
 */
template <typename ValueT, typename InputStreamT>
auto read(InputStreamT& stream)
{
    return comp([=, stream_ref = std::ref(stream)](auto&& step) {
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
    });
}

template <typename T1, typename T2, typename... Ts, typename InputStreamT>
auto read(InputStreamT& stream)
{
    return comp(read<T1>(stream), read<T2>(stream), read<Ts>(stream)...);
}

} // namespace zug
