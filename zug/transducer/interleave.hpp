//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/detail/iterator_range.hpp>
#include <zug/detail/transducer_holder.hpp>
#include <zug/reduce_nested.hpp>
#include <zug/state_wrapper.hpp>

#include <vector>

namespace zug {

namespace detail {

template <typename StepT, typename StateT>
decltype(auto) interleave_step(StepT&&, StateT&& s)
{
    return std::forward<StateT>(s);
}

template <typename StepT, typename StateT, typename InputT, typename... InputTs>
auto interleave_step(StepT&& step, StateT&& s, InputT&& i, InputTs&&... is)
    -> decltype(step(std::forward<StateT>(s), std::forward<InputT>(i)))
{
    return !state_is_reduced(s) ? interleave_step(std::forward<StepT>(step),
                                                  step(std::forward<StateT>(s),
                                                       std::forward<InputT>(i)),
                                                  std::forward<InputTs>(is)...)
                                : std::forward<StateT>(s);
}

} // namespace detail

struct interleave_t
{
    template <typename StepT>
    auto operator()(StepT&& step) const
    {
        return [=](auto&& s, auto&& i, auto&&... is) mutable {
            return detail::interleave_step(
                step, step(ZUG_FWD(s), ZUG_FWD(i)), ZUG_FWD(is)...);
        };
    }
};

ZUG_INLINE_CONSTEXPR auto interleave = detail::make_transducer_holder(interleave_t{});

} // namespace zug
