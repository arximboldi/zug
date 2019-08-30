//
// zug: transducers for C++
// Copyright (C) 2019 Juan Pedro Bolivar Puente
//
// This software is distributed under the Boost Software License, Version 1.0.
// See accompanying file LICENSE or copy at http://boost.org/LICENSE_1_0.txt
//

#pragma once

#include <zug/transducer/filter.hpp>

#include <random>

#ifndef ABL_XFORM_DEFAULT_RANDOM_ENGINE
#define ABL_XFORM_DEFAULT_RANDOM_ENGINE ::std::default_random_engine
#endif

namespace zug {

namespace detail {

struct default_generator
{
    using engine_t       = ABL_XFORM_DEFAULT_RANDOM_ENGINE;
    using distribution_t = std::uniform_real_distribution<>;

    engine_t engine             = engine_t{};
    distribution_t distribution = distribution_t{};

    auto operator()() { return distribution(engine); }
};

template <typename ProbabilityT, typename GeneratorT>
struct random_sampler
{
    ProbabilityT probability;
    GeneratorT generator;

    template <typename Ts>
    bool operator()(Ts&&...)
    {
        return generator() <= probability;
    }
};

} // namespace detail

/*!
 * Similar to clojure.core/random-sample$1
 */
template <typename ProbabilityT,
          typename GeneratorT = detail::default_generator>
auto random_sample(ProbabilityT prob, GeneratorT gen = GeneratorT{})
{
    using sampler_t = detail::random_sampler<ProbabilityT, GeneratorT>;
    return filter(sampler_t{std::move(prob), std::move(gen)});
}

} // namespace zug
