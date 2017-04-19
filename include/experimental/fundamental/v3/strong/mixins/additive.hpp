// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP

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
      struct additive
      {
        friend constexpr Final operator+(Final const&x)  noexcept
        {
          return x;
        }

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
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x) noexcept
        {
          return Final(++x._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x, int) noexcept
        {
          return Final(x._underlying()++);
        }

        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x._underlying() -= y._underlying();
          return x;
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
    }
  }
}
}

#endif // header
