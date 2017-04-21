// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_INTEGER_MULTIPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_INTEGER_MULTIPLICATIVE_HPP

#include <experimental/fundamental/v3/strong/mixins/multiplicative.hpp>
#include <experimental/fundamental/v3/strong/mixins/modable.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final, class Check=no_check>
      struct integer_multiplicative : multiplicative<Final, Check>, modable<Final, Check>
      {
      };

      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct integer_multiplicative_with_if
          : multiplicative_with_if<Final, Check, Pred>
          , modable_with_if<Final, Check, Pred>
      {
      };
      template <class Final, class UT=underlying_type_t<Final>>
      struct integer_multiplicative_with
          : multiplicative_with<Final, UT>
          , modable_with<Final, UT>
      {
      };

    }
  }
}
}

#endif // header
