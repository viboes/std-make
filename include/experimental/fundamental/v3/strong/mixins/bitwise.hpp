// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_BITWISE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_BITWISE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>

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
          return Final(~x._backdoor()._underlying());
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend constexpr Final operator<<(Final const&x, IntegralType y) noexcept
        {
          return Final(typename Final::underlying_t(x._backdoor()._underlying() << y));
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend constexpr Final operator>>(Final const&x, IntegralType y) noexcept
        {
          return Final(x._backdoor()._underlying() >> y);
        }

      };

      template <class Final, class Check=no_check>
      struct bitwise_assign : bitwise_base<Final>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator&=(Final &x, Final const&y) noexcept
        {
          x._backdoor()._underlying() &= y._backdoor()._underlying();
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator|=(Final &x, Final const& y) noexcept
        {
          x._backdoor()._underlying() |= y._backdoor()._underlying();
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator^=(Final &x, Final const&y) noexcept
        {
          x._backdoor()._underlying() ^= y._backdoor()._underlying();
          return x;
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator<<=(Final &x, IntegralType y) noexcept
        {
          x._backdoor()._underlying() <<= y;
          return x;
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator>>=(Final &x, IntegralType y) noexcept
        {
          x._backdoor()._underlying() >>= y;
          return x;
        }
      };

      template <class Final>
      struct bitwise_assign<Final, check> : bitwise_base<Final>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator&=(Final &x, Final const&y) noexcept
        {
          x = x & y;
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator|=(Final &x, Final const& y) noexcept
        {
          x = x | y;
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator^=(Final &x, Final const&y) noexcept
        {
          x = x ^ y;
          return x;
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator<<=(Final &x, IntegralType y) noexcept
        {
          x = x << y;
          return x;
        }
        template <class IntegralType, typename = enable_if<is_integral<IntegralType>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator>>=(Final &x, IntegralType y) noexcept
        {
          x = x >> y;
          return x;
        }

      };
      template <class Final, class Check=no_check>
      struct bitwise : bitwise_assign<Final, Check>
      {
        friend constexpr Final operator&(Final const&x, Final const& y) noexcept
        {
          return Final(x._backdoor()._underlying() & y._backdoor()._underlying());
        }
        friend constexpr Final operator|(Final const& x, Final const& y) noexcept
        {
          return Final(x._backdoor()._underlying() | y._backdoor()._underlying());
        }
        friend constexpr Final operator^(Final const&x, Final const&y) noexcept
        {
          return Final(x._backdoor()._underlying() ^ y._backdoor()._underlying());
        }
      };

      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct bitwise_with_if : bitwise_assign<Final, Check>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr common_type_t<Final, Other> operator&(Final const&x, Other const& y) noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._backdoor()._underlying() & CT(y)._backdoor()._underlying());
        }
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr common_type_t<Final, Other> operator|(Final const& x, Other const& y) noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._backdoor()._underlying() | CT(y)._backdoor()._underlying());
        }
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr common_type<Final, Other> operator^(Final const&x, Other const&y) noexcept
        {
          using CT = common_type<Final, Other>;
          return CT(CT(x)._backdoor()._underlying() ^ CT(y)._backdoor()._underlying());
        }

      };
    }
  }
}
}

#endif // header
