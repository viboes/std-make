// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_CONVERTIBLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_CONVERTIBLE_HPP

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
      template <class Final, class T>
      struct explicit_convertible_to
      {
        constexpr explicit operator T () const noexcept
        {
          return static_cast<T>(Final::_final(this).underlying());
        }
      };
      template <class Final, class T>
      struct implicit_convertible_to
      {
        constexpr operator T () const noexcept
        {
          return Final::_final(this).underlying();
        }
      };

    }
  }
}
}

#endif // header
