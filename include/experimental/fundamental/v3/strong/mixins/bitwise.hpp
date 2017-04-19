// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final>
      struct bitwise_base
      {
        friend constexpr Final operator~(Final const& x) noexcept
        {
          return Final(~x._underlying());
        }

      };

      template <class Final>
      struct bitwise_no_check : bitwise_base<Final>
      {
        friend constexpr Final operator&(Final const&x, Final const& y) noexcept
        {
          return Final(x._underlying() & y._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator&=(Final &x, Final const&y) noexcept
        {
          x._underlying() &= y._underlying();
          return x;
        }
        friend constexpr Final operator|(Final const& x, Final const& y) noexcept
        {
          return Final(x._underlying() | y._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator|=(Final &x, Final const& y) noexcept
        {
          x._underlying() |= y._underlying();
          return x;
        }
        friend constexpr Final operator^(Final const&x, Final const&y) noexcept
        {
          return Final(x._underlying() ^ y._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator^=(Final &x, Final const&y) noexcept
        {
          x._underlying() ^= y._underlying();
          return x;
        }

        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend constexpr Final operator<<(Final const&x, IntegralType y) noexcept
        {
          return Final(x._underlying() << y);
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator<<=(Final &x, IntegralType y) noexcept
        {
          x._underlying() <<= y;
          return x;
        }
        friend constexpr Final operator>>(Final const&x, int y) noexcept
        {
          return Final(x._underlying() >> y);
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator>>=(Final &x, int y) noexcept
        {
          x._underlying() >>= y;
          return x;
        }

      };
    }
  }
}
}

#endif // header
