//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#include <zug/compat/apply.hpp>

namespace zug {

namespace detail {

template <
    class F,
    class Tuple,
    std::enable_if_t<std::is_same<detail::get_index_sequence_t<Tuple>,
                                  detail::could_not_get_index_sequence>::value,
                     int> = 0>
decltype(auto) apply_if_you_can_bitte(F&& f, Tuple&& t)
{
    return std::forward<F>(f)(std::forward<Tuple>(t));
}

template <
    class F,
    class Tuple,
    std::enable_if_t<!std::is_same<detail::get_index_sequence_t<Tuple>,
                                   detail::could_not_get_index_sequence>::value,
                     int> = 0>
constexpr decltype(auto) apply_if_you_can_bitte(F&& f, Tuple&& t)
{
    return apply(std::forward<F>(f), std::forward<Tuple>(t));
}

template <typename Fn>
auto apply_all_what_you_can_bitte(Fn&& fn)
{
    return fn();
}

template <typename Fn, typename Tuple, typename... Tuples>
auto apply_all_what_you_can_bitte(Fn&& fn, Tuple&& t, Tuples&&... ts)
{
    return apply_if_you_can_bitte(
        [&](auto&&... xs) {
            return apply_all_what_you_can_bitte(
                [&](auto&&... ys) {
                    return fn(ZUG_FWD(xs)..., ZUG_FWD(ys)...);
                },
                ts...);
        },
        t);
}

} // namespace detail

/*!
 * Transducer that expands all unzipable inputs into the transducer. Unzipable
 * inputs are std::tuple<>, std::pair<> and std::array<>.
 */
ZUG_INLINE_CONSTEXPR struct unzip_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&&... is) mutable {
            return detail::apply_all_what_you_can_bitte(
                [&](auto&&... xs) { return step(ZUG_FWD(s), ZUG_FWD(xs)...); },
                ZUG_FWD(is)...);
        };
    }
} unzip{};

} // namespace zug
