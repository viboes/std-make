//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_MAYBE_UNUSED_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_MAYBE_UNUSED_HPP

#include <utility>
#include <type_traits>

// states that this lvalue is maybe not used
#define JASEL_MAYBE_UNUSED(x) \
    static_assert(std::is_lvalue_reference<decltype(*&x)>::value, "WTS_MAYBE_UNUSED used with an expression not lvalue"); \
    static_cast<void>( sizeof x )

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
//! states that this lvalue is maybe not used
//! <code>
//! void fx(int p) {
//!      maybe_unused(p);
//! }
//! </code>

template <class U>
    // requires U& is an lvalue
void maybe_unused(U& ) {
    //return static_cast<void>(sizeof(u));
}

}
}
}

#endif // header
