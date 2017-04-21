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
      template <class Final>
      struct integer_multiplicative_base_no_check : multiplicative_base_no_check<Final>, modable_base_no_check<Final>
      {
      };
      template <class Final>
      struct integer_multiplicative_base_check : multiplicative_base_check<Final>, modable_base_check<Final>
      {
      };
      template <class Final>
      struct integer_multiplicative_check : multiplicative_check<Final>, modable_check<Final>
      {
      };

      template <class Final>
      struct integer_multiplicative_no_check : multiplicative_no_check<Final>, modable_no_check<Final>
      {
      };

      template <class Final, template <class, class> class Pred=is_compatible_with>
      struct integer_multiplicative_with_if_no_check
          : multiplicative_with_if_no_check<Final, Pred>
          , modable_with_if_no_check<Final, Pred>
      {
      };

    }
  }
}
}

#endif // header
