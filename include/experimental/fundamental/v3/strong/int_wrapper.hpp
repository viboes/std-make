// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_INT_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_INT_WRAPPER_HPP

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class Tag, class UT, class Default = uninitialized_t>
  struct int_wrapper final : protected_wrapper<UT, Default>
  {
      using base_type = protected_wrapper<UT, Default>;
      using base_type::base_type;
      using base_type::underlying;

      using tag_type = Tag;
      using underlying_t = UT;

      // relational operators
      friend constexpr bool operator<(int_wrapper const& x, int_wrapper const& y)  noexcept
      { return x.value < y.value; }
      friend constexpr bool operator<=(int_wrapper const& x, int_wrapper const& y)  noexcept
      { return x.value <= y.value; }
  };


}
}
}

#endif // header
