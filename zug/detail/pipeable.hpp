#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/util.hpp>

#include <type_traits>
#include <utility>

namespace zug {
namespace detail {

template <typename Transducer>
struct pipeable
{
    using transducer_type = Transducer;
    transducer_type f;

    template <typename Step>
    auto operator()(Step&& step) & -> decltype(auto)
    {
        return compat::invoke(f, std::forward<Step>(step));
    }

    template <typename Step>
    auto operator()(Step&& step) const& -> decltype(auto)
    {
        return compat::invoke(f, std::forward<Step>(step));
    }

    template <typename Step>
    auto operator()(Step&& step) && -> decltype(auto)
    {
        return compat::invoke(std::move(f), std::forward<Step>(step));
    }
};

template <typename T1, typename T2>
constexpr auto operator|(pipeable<T1> lhs, T2 rhs)
{
    return comp(std::move(lhs), std::move(rhs));
}

template <typename T1, typename T2>
constexpr auto operator|(T1 lhs, pipeable<T2> rhs)
{
    return comp(std::move(lhs), std::move(rhs));
}

template <typename T1, typename T2>
constexpr auto operator|(pipeable<T1> lhs, pipeable<T2> rhs)
{
    return comp(std::move(lhs), std::move(rhs));
}

template <typename Transducer>
constexpr auto make_pipeable(Transducer&& f)
{
    return transducer_holder<std::decay_t<Transducer>>{
        std::forward<Transducer>(f)};
}

} // namespace detail
} // namespace zug