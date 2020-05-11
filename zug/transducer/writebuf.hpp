//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/compose.hpp>
#include <zug/util.hpp>

#include <functional>
#include <ios>
#include <iterator>

namespace zug {

/*!
 * Generator transducer that writes buffers into the stream and passes them into
 * the sequence.  A buffer shall be a range with `char*` iterators.
 * Lets `n` elements through and finishes.
 *
 * @rst
 *   .. literalinclude:: ../test/transducer/writebuf.cpp
 *      :language: c++
 *      :start-after: // example1 {
 *      :end-before:  // }
 *      :dedent: 4
 * @endrst
 */
template <typename OutputStreamT>
auto writebuf(OutputStreamT& stream)
{
    return comp([=, stream_ref = std::ref(stream)](auto&& step) {
        return [=](auto&& s, auto&& buf, auto&&... is) mutable {
            using std::begin;
            using std::end;

            auto& stream = stream_ref.get();
            auto first   = begin(buf);
            auto last    = end(buf);
            stream.write(
                &*first,
                static_cast<std::streamsize>(std::distance(first, last)));
            return step(ZUG_FWD(s), ZUG_FWD(buf), ZUG_FWD(is)...);
        };
    });
}

} // namespace zug
