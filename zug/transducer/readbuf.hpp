//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/iterator_range.hpp>
#include <zug/detail/pipeable.hpp>
#include <zug/maybe_reduced.hpp>

#include <array>
#include <functional>
#include <vector>

namespace zug {

namespace detail {

template <typename InputStreamT, typename MkBuf>
auto readbuf(InputStreamT& stream, MkBuf make_buffer)
{
    return make_pipeable([=, stream_ref = std::ref(stream)](auto&& step) {
        return [=, buffer = make_buffer()](auto&& s, auto&&... is) mutable {
            auto& stream = stream_ref.get();
            auto data    = &buffer[0];
            stream.read(data, static_cast<std::streamsize>(buffer.size()));
            return reduced_if(
                step(state_unwrap(ZUG_FWD(s)),
                     ZUG_FWD(is)...,
                     detail::make_iterator_range(data, data + stream.gcount())),
                !stream);
        };
    });
}

} // namespace detail

/*!
 * Generator transducer that reads buffers of size `N` from @a
 * `stream`, and passes them into the sequence.  It passes them into
 * the sequence as an range with `char*` iterators.
 */
template <std::size_t N, typename InputStreamT>
auto readbuf(InputStreamT& stream)
{
    return detail::readbuf(stream, [] { return std::array<char, N>{}; });
}

/*!
 * Like `readbuf(stream)` but with runtime specified size.
 */
template <typename InputStreamT>
auto readbuf(InputStreamT& stream, std::size_t n)
{
    return detail::readbuf(stream, [n] { return std::vector<char>(n); });
}

} // namespace zug
