//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_EXPECTED_SUCCESS_OR_FAILURE_HPP
#define JASEL_FUNDAMENTAL_V3_EXPECTED_SUCCESS_OR_FAILURE_HPP

#include <experimental/fundamental/v3/expected/expected.hpp>
#include <experimental/fundamental/v3/success_or_failure/success_or_failure.hpp>
#include <utility>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace success_or_failure
{
template <class T, class E>
struct traits<expected<T,E>>
{
    template <class U>
    static constexpr
    bool succeeded(U && e) noexcept { return bool(e); }

    template <class U>
    static constexpr
    auto success_value(U && e)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
            *e
    )

    // This is the major reason for having expected::get_unexpected. Error propagation.
    // We could do it unexpected and expected::error, but then we cannot return a reference.
    // return unexpected(e.error())
    template <class U>
    static constexpr
    auto failure_value(U && e)
    JASEL_DECLTYPE_RETURN_NOEXCEPT (
            e.get_unexpected()
    )

};
}
}}
}
#endif // header
