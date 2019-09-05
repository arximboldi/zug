//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <functional>
#include <ios>
#include <iterator>

namespace zug {

/*!
 * Generator transducer that writes buffers into the stream and passes them into
 * the sequence.  It passes them into the sequence as an range with `char*`
 * iterators.
 */
template <typename OutputStreamT>
auto writebuf(OutputStreamT& stream)
{
    return [=, stream_ref = std::ref(stream)](auto&& step) {
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
    };
}

} // namespace zug
