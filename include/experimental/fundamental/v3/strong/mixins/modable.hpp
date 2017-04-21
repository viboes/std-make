// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MODABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MODABLE_HPP

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
      template <class Final, class Check=no_check>
      struct modable_assign
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator%=(Final& x, Final const& y) noexcept
        {
          x._underlying() %= y._underlying();
          return x;
        }
      };
      template <class Final>
      struct modable_assign<Final, check>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator%=(Final& x, Final const& y) noexcept
        {
          x = x % y;
          return x;
        }
      };
      // homogeneous
      template <class Final, class Check=no_check>
      struct modable : modable_assign<Final, Check>
      {
        friend constexpr Final operator%(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() % y._underlying());
        }
      };

      // heterogeneous
      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct modable_with_if : modable_assign<Final, Check>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr typename common_type<Final, Other>::type operator%(Final const& x, Other const& y)  noexcept
        {
          using CT = typename common_type<Final, Other>::type;

          return CT(CT(x)._underlying() % CT(y)._underlying());
        }
      };

    }
  }
}
}

#endif // header
