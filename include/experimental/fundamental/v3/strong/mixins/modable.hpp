// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MODABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_MODABLE_HPP

#include <experimental/fundamental/v2/config.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final>
      struct modable_base_no_check
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator%=(Final& x, Final const& y) noexcept
        {
          x._underlying() %= y._underlying();
          return x;
        }
      };
      template <class Final>
      struct modable_base_check
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator%=(Final& x, Final const& y) noexcept
        {
          x = Final(x._underlying() % y._underlying());
          return x;
        }
      };
      template <class Final>
      struct modable_check : modable_base_check<Final>
      {
        friend constexpr Final operator%(Final const& x, Final const& y)  noexcept
        {
          return Final(x._underlying() % y._underlying());
        }
      };
    }
  }
}
}

#endif // header
