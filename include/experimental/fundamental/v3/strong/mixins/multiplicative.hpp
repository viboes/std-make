// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MULTIPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MULTIPLICATIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
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
      struct multiplicative_base_no_check
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
      struct multiplicative_base_check
      {
        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator*=(Final& x, Final const& y) noexcept
        {
          x = Final(x._underlying() * y._underlying());
          return x;
        }
        friend JASEL_MUTABLE_CONSTEXPR Final& operator/=(Final& x, Final const& y) noexcept
        {
          x = Final(x._underlying() / y._underlying());
          return x;
        }
      };
      template <class Final>
      struct multiplicative_no_check : multiplicative_base_no_check<Final>
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
      template <class Final>
      struct multiplicative_check : multiplicative_base_check<Final>
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

#if 0
      template <class Final, template <class, class> class Pred=is_compatible_with>
      struct multiplicative_with_if_base_no_check
      {
        //! Forwards to the underlying value
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator*=(Final& x, Other const& y) noexcept
        {
          x._underlying() *= y._underlying();
          return x;
        }

        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend JASEL_MUTABLE_CONSTEXPR Final& operator/=(Final& x, Other const& y) noexcept
        {
          x._underlying() /= y._underlying();
          return x;
        }
      };
#endif
      template <class Final, template <class, class> class Pred=is_compatible_with>
      //struct multiplicative_with_if_no_check : multiplicative_with_if_base_no_check<Final, Pred>
      struct multiplicative_with_if_no_check : multiplicative_base_no_check<Final>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr typename common_type<Final, Other>::type operator*(Final const& x, Other const& y)  noexcept
        {
          using CT = typename common_type<Final, Other>::type;
          return CT(CT(x)._underlying() * CT(y)._underlying());
        }

        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr typename common_type<Final, Other>::type operator/(Final const& x, Other const& y)  noexcept
        {
          using CT = typename common_type<Final, Other>::type;
          return CT(CT(x)._underlying() / CT(y)._underlying());
        }
      };

    }
  }
}
}

#endif // header
