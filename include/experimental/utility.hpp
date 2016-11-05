// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_UTILITY_HPP
#define JASEL_EXPERIMENTAL_UTILITY_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  struct unit{};
  // this should be moved away (meta?)
  struct swallow {
    template <class ... Ts>
    swallow(Ts&&...) {  }
  };

}}}

#include <experimental/fundamental/v3/std/std_pair.hpp>

#endif // header
