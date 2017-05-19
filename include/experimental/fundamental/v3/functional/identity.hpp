//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FUNCTIONAL_IDENTITY_HPP
#define JASEL_FUNDAMENTAL_V3_FUNCTIONAL_IDENTITY_HPP


#include <experimental/fundamental/v2/config.hpp>

#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  struct identity {
    template <class T>
    constexpr decay_t<T> operator()(T&& x) const { return forward<T>(x); }
  };
}}}

#endif // header
