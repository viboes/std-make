// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>
#include <type_traits>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      // Single arg
      template <class Final>
      struct additive_base
      {
        friend constexpr Final operator+(Final const&x)  noexcept
        {
          return x;
        }
        friend constexpr Final operator-(Final const&x)  noexcept
        {
          return Final(-x._underlying());
        }

        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x) noexcept
        {
          return Final(++x._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x, int) noexcept
        {
          return Final(x._underlying()++);
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x ) noexcept
        {
          return Final(--x._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x, int) noexcept
        {
          return Final(x._underlying()--);
        }

      };
      template <class Final, class Check=no_check>
      struct additive_assign : additive_base<Final>
      {
        // todo These should be moved to homogeneous when I'll add heterogeneous
        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x._underlying() += y._underlying();
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x._underlying() -= y._underlying();
          return x;
        }

      };

      //arithmetic assignment when checked relies in the assignment of the result of the arithmetic operation
      template <class Final>
      struct additive_assign<Final, check> : additive_base<Final>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x = x + y;
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x = x - y;
          return x;
        }
      };

      template <class Final, class Check=no_check>
      struct additive : additive_assign<Final, Check>
      {
        friend constexpr Final operator+(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() + y._underlying());
        }

        friend constexpr Final operator-(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() - y._underlying());
        }

      };

      // additive heterogeneous
      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct additive_with_if : additive_assign<Final, Check>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        typename common_type<Final, Other>::type
        operator+(Final const& x, Other const& y)  noexcept
        {
          using CT = typename common_type<Final, Other>::type;
          return CT(CT(x)._underlying() + CT(y)._underlying());
        }

        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        typename common_type<Final, Other>::type
        operator-(Final const& x, Other const& y)  noexcept
        {
          using CT = typename common_type<Final, Other>::type;
          return CT(CT(x)._underlying() - CT(y)._underlying());
        }

      };

    }
  }
}
}

#endif // header
