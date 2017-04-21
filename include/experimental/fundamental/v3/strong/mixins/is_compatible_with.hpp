// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_IS_COMPATIBLE_WITH_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_IS_COMPATIBLE_WITH_HPP

#include <experimental/fundamental/v2/config.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      struct check {};
      struct no_check {};

      template <class T, class U>
      struct is_compatible_with : false_type {};

      //template <class T>
      //struct is_compatible_with<T,T> : true_type {};

    }
  }
}
}

#endif // header
