//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/compose.hpp>
#include <zug/detail/tuple_utils.hpp>
#include <zug/meta.hpp>
#include <zug/meta/value_type.hpp>
#include <zug/reductor.hpp>
#include <zug/transducer/iter.hpp>

#include <zug/detail/iterator_facade.hpp>
#include <zug/detail/iterator_range.hpp>

#include <iostream>
#include <vector>

namespace zug {

namespace detail {

template <typename ValueT, typename XformT>
struct sequence_data
{
    using value_t = ValueT;
    using cache_t = std::vector<value_t>;
    using state_t = sequence_data*;
    struct end_t
    {};

    struct step_t
    {
        template <typename... Ts>
        state_t operator()(state_t s, Ts&&... xs) const
        {
            s->impl_.cache.emplace_back(std::forward<Ts>(xs)...);
            return s;
        }
    };

    using reductor_t =
        empty_reductor<std::decay_t<std::result_of_t<XformT(step_t)>>, state_t>;

    sequence_data(const XformT& xform)
        : impl_{std::size_t{}, cache_t{}, reductor_t{xform(step_t{}), this}}
    {
        pull();
    }

    sequence_data(const XformT& xform, end_t)
        : impl_{std::size_t{}, cache_t{}, reductor_t{xform(step_t{}), this}}
    {}

    sequence_data(sequence_data&& other)
        : impl_(std::move(other.impl_))
    {
        impl_.reductor.current(this);
    }

    sequence_data(const sequence_data& other)
        : impl_(other.impl_)
    {
        impl_.reductor.current(this);
    }

    sequence_data& operator=(sequence_data&& other)
    {
        impl_ = std::move(other.impl);
        impl_.reductor.current(this);
    }

    sequence_data& operator=(const sequence_data& other)
    {
        using std::swap;
        sequence_data copied{other};
        swap(*this, copied);
        return *this;
    }

    bool empty() const
    {
        return impl_.pos == impl_.cache.size() && !impl_.reductor;
    }

    const value_t& front() const { return impl_.cache[impl_.pos]; }

    void pop()
    {
        ++impl_.pos;
        pull();
    }

    bool operator==(const sequence_data& other) const
    {
        return impl_.pos == other.impl_.pos &&
               impl_.cache.size() == other.impl_.cache.size();
    }

    bool operator!=(const sequence_data& other) const
    {
        return !(*this == other);
    }

private:
    void pull()
    {
        if (impl_.pos == impl_.cache.size()) {
            impl_.cache.clear();
            impl_.pos = 0;
            while (impl_.cache.empty() && impl_.reductor) {
                impl_.reductor();
            }
        }
    }

    struct
    {
        std::size_t pos;
        cache_t cache;
        reductor_t reductor;
    } impl_;
};

} // namespace detail

//! @defgroup sequence
//! @{

/*!
 * Range adaptor that transduces the ranges in `RangeTs` with the
 * transducer `XformT`, producing values of `ValueT`.  It also works
 * with no range, as a generator.
 *
 * @rst
 *
 * .. tip:: The transducer is processed lazily.  It is ok to adapt
 *    infinite ranges or infinite generators!
 *
 * .. note:: There is no shared data between the range iterators excepting the
 *    adapted ranges, for which a const reference is kept.  This means that it
 *    is safe to copy the iterators around and use them from different threads.
 *    It is also safe to use the iterators after the parent `sequence_range` is
 *    destroyed, but the adapted ranges should still be kept alive.
 *
 * .. warning:: The transducer is processed every time for each iterator.  Thus,
 *    any side effects the transducer might cause will be produced whenever we
 *    run on each iterator of the range.
 *
 * @endrst
 */
template <typename ValueT, typename XformT, typename... RangeTs>
struct sequence_range
{
    using xform_t = decltype(comp(
        iter(detail::make_iterator_range(std::declval<const RangeTs&>())...),
        std::declval<XformT>()));

    using data_t = detail::sequence_data<ValueT, xform_t>;

    using value_type = const ValueT;

    struct iterator
        : detail::iterator_facade<iterator, std::input_iterator_tag, value_type>
    {
    private:
        friend sequence_range;
        friend struct detail::iterator_core_access;

        iterator(data_t data)
            : data_{std::move(data)}
        {}

        void increment() { data_.pop(); }
        bool equal(const iterator& other) const { return data_ == other.data_; }

        const value_type& dereference() const { return data_.front(); }

        data_t data_;
    };

    sequence_range(XformT xform, const RangeTs&... ranges)
        : xform_{comp(iter(detail::make_iterator_range(ranges)...),
                      std::move(xform))}
    {}

    using const_iterator = iterator;

    iterator begin() const { return {xform_}; }
    iterator end() const { return {{xform_, typename data_t::end_t{}}}; }

private:
    xform_t xform_;
};

//! @}

namespace detail {
struct deduce_value_type
{};
} // namespace detail

//! @defgroup sequence
//! @{

/*!
 * Returns a `sequence_range` values producing an iterable range out of a
 * transducer, in the spirit of
 * [clojure.core/sequence](https://clojuredocs.org/clojure.core/sequence)
 */
template <typename ValueT = detail::deduce_value_type,
          typename XformT,
          typename... RangeTs>
auto sequence(XformT&& xform, const RangeTs&... ranges)
    -> sequence_range<typename boost::mpl::eval_if<
                          std::is_same<ValueT, detail::deduce_value_type>,
                          result_of<XformT, meta::value_t<RangeTs>...>,
                          meta::identity<ValueT>>::type,
                      std::decay_t<XformT>,
                      std::decay_t<RangeTs>...>
{
    return {std::forward<XformT>(xform), ranges...};
}

//! @}

} // namespace zug
