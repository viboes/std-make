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
      // homegeneous
      template <class Final>
      struct additive_base_no_check : additive_base<Final>
      {
        // todo These should be moved to homogeneous when I'll add heterogeneous
        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x._underlying() += y._underlying();
          return x;
        }
//        JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final y) noexcept
//        {
//          Final::_final(this)._underlying() += y._underlying();
//          return Final::_final(this);
//        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x._underlying() -= y._underlying();
          return x;
        }

      };
      template <class Final>
      struct additive_base_check : additive_base<Final>
      {
        // todo These should be moved to homogeneous when I'll add heterogeneous

        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x = Final(x._underlying() + y._underlying());
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x = Final(x._underlying() - y._underlying());
          return x;
        }

      };
      template <class Final>
      struct additive_check : additive_base_check<Final>
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

      template <class Final, template <class, class> class Pred=is_compatible_with>
      struct additive_with_if_base_no_check : additive_base<Final>
      {
        // todo These should be moved to homogeneous when I'll add heterogeneous
        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Other const& y) noexcept
        {
          x._underlying() += y._underlying();
          return x;
        }
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Other const& y) noexcept
        {
          x._underlying() -= y._underlying();
          return x;
        }
      };

      template <class Final, template <class, class> class Pred=is_compatible_with>
      //struct additive_with_if_no_check : additive_with_if_base_no_check<Final,Pred>
      struct additive_with_if_no_check : additive_base_no_check<Final>
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
