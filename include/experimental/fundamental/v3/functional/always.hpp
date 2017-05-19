//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FUNCTIONAL_ALWAYS_HPP
#define JASEL_FUNDAMENTAL_V3_FUNCTIONAL_ALWAYS_HPP


#include <experimental/fundamental/v2/config.hpp>

#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class U>
  struct always {
    constexpr  always(U val) : value(move(val)) {}
    template <class T>
    constexpr U operator()(T&& ) const { return value; }
  private:
    U value;

  };
}}}

#endif // header
