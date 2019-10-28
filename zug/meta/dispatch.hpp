#pragma once

#include <cstddef>

namespace zug {
namespace meta {

/*!
 * Tags used for function dispatching.
 */
template <std::size_t Index>
struct dispatch_t : dispatch_t<Index - 1>
{};

template <>
struct dispatch_t<0>
{};

using try_t   = dispatch_t<1>;
using catch_t = dispatch_t<0>;

} // namespace meta
} // namespace zug
