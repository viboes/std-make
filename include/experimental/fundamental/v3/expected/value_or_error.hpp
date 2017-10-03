//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_VALUE_OR_ERROR_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_VALUE_OR_ERROR_HPP

#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>
#include <experimental/fundamental/v3/expected/success_or_failure.hpp>

#if 0
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace value_or_error
{
template <class T, class E>
struct traits<expected<T,E>> : traits_pointer_like
{
//    template <class U>
//    static constexpr
//    bool has_value(U && e) noexcept { return e.has_value(); }
//
//    template <class U>
//    static constexpr
//    auto deref(U && e) noexcept { return *e; }

    template <class U>
    static constexpr
    auto error(U && e) noexcept { return e.error(); }

};
}
}}
}
#endif
#endif // header
