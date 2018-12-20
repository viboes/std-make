//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_DISCARD_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_DISCARD_HPP

#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// discards the value of an expression after evaluating it
// Note that the intent is not the same as maybe_unused, which takes a lvalue. This can take an rvalue and it will be evaluated
// use:
//      // auto r = discard(expr);
//      // maybe_unused(r);
//      discard(expr);

template <class U>
void discard(U&& u) {
    return static_cast<void>(std::forward<U>(u));
}

}
}
}

#endif // header
