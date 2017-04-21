// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MULTIPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MULTIPLICATIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/meta/v1/rebind.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final, class Check=no_check>
      struct multiplicative_assign
      {
        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator*=(Final& x, Final const& y) noexcept
        {
          x._underlying() *= y._underlying();
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator/=(Final& x, Final const& y) noexcept
        {
          x._underlying() /= y._underlying();
          return x;
        }
      };
      template <class Final>
      struct multiplicative_assign<Final, check>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator*=(Final& x, Final const& y) noexcept
        {
          x = x * y;
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator/=(Final& x, Final const& y) noexcept
        {
          x = x / y;
          return x;
        }
      };
      //homegeneous
      template <class Final, class Check=no_check>
      struct multiplicative : multiplicative_assign<Final, Check>
      {
        friend constexpr Final operator*(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() * y._underlying());
        }
        friend constexpr Final operator/(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() / y._underlying());
        }
      };

      // heterogeneous

      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct multiplicative_with_if : multiplicative_assign<Final, Check>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr common_type_t<Final, Other> operator*(Final const& x, Other const& y)  noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._underlying() * CT(y)._underlying());
        }

        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr common_type_t<Final, Other> operator/(Final const& x, Other const& y)  noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._underlying() / CT(y)._underlying());
        }
      };

      template <class Final, class UT=underlying_type_t<Final>>
      struct multiplicative_with
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator*=(Final& x, UT const& y) noexcept
        {
          x._underlying() *= y;
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator/=(Final& x, UT const& y) noexcept
        {
          x._underlying() /= y;
          return x;
        }

        template <class UT2>
        friend constexpr
        meta::rebind_t<Final, common_type_t<UT, UT2>> operator*(Final const& x, UT2 const& y)  noexcept
        {
          using CR = common_type_t<UT, UT2>;
          using CT = meta::rebind_t<Final, CR>;
          return CT(CT(x)._underlying() * CR(y));
        }

        template <class UT2>
        friend constexpr
        meta::rebind_t<Final, common_type_t<UT, UT2>> operator*(UT2 const& x, Final const& y)  noexcept
        {
          return y * x;
        }

        template <class UT2
          , typename = enable_if_t<is_convertible<UT2,UT>::value>
        >
        friend constexpr meta::rebind_t<Final, common_type_t<UT, UT2>> operator/(Final const& x, UT2 const& y)  noexcept
        {
          using CR = common_type_t<UT, UT2>;
          using CT = meta::rebind_t<Final, CR>;
          return CT(CT(x)._underlying() / CR(y));
        }
      };
    }
  }
}
}

#endif // header
