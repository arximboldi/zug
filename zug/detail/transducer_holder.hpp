#pragma once

#include <zug/compat/invoke.hpp>
#include <zug/util.hpp>

#include <type_traits>
#include <utility>

namespace zug {
namespace detail {

template <typename Transducer>
struct transducer_holder
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
constexpr auto operator|(transducer_holder<T1>&& lhs, T2&& rhs)
{
    return comp(std::forward<transducer_holder<T1>>(lhs),
                std::forward<T2>(rhs));
}

template <typename T1, typename T2>
constexpr auto operator|(T1&& lhs, transducer_holder<T2>&& rhs)
{
    return comp(std::forward<T1>(lhs),
                std::forward<transducer_holder<T2>>(rhs));
}

template <typename T1, typename T2>
constexpr auto operator|(transducer_holder<T1>&& lhs,
                         transducer_holder<T2>&& rhs)
{
    return comp(std::forward<transducer_holder<T1>>(lhs),
                std::forward<transducer_holder<T2>>(rhs));
}

template <typename Transducer>
constexpr auto make_transducer_holder(Transducer&& f)
{
    return transducer_holder<std::decay_t<Transducer>>{
        std::forward<Transducer>(f)};
}

} // namespace detail
} // namespace zug