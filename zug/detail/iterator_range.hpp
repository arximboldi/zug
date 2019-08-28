//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <iterator>
#include <type_traits>

namespace zug {
namespace detail {

template <typename I, typename S>
struct iterator_range
{
    using value_type     = typename std::iterator_traits<I>::value_type;
    using iterator       = I;
    using const_iterator = I;
    using sentinel       = S;

    iterator_range(I first, S last)
        : begin_{first}
        , end_{last}
    {}

    constexpr I& begin() & { return begin_; }
    constexpr I const& begin() const& { return begin_; }

    constexpr S& end() & { return end_; }
    constexpr S const& end() const& { return end_; }

private:
    I begin_;
    S end_;
};

template <typename I, typename S>
auto make_iterator_range(I&& fst, S&& lst)
{
    return iterator_range<std::decay_t<I>, std::decay_t<S>>{fst, lst};
}

template <typename Range>
auto make_iterator_range(Range&& range)
{
    using std::begin;
    using std::end;
    return make_iterator_range(begin(range), end(range));
}

} // namespace detail
} // namespace zug
