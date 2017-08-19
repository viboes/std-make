// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_INCREMENTABLE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_INCREMENTABLE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/mixins/additive.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {
      template <class Final>
      struct forward_incrementable
      {
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x) noexcept
        {
          return Final(++x._backdoor()._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x, int) noexcept
        {
          return Final(x._backdoor()._underlying()++);
        }
      };

      template <class Final>
      struct bidirectional_incrementable : forward_incrementable<Final>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x ) noexcept
        {
          return Final(--x._backdoor()._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x, int) noexcept
        {
          return Final(x._backdoor()._underlying()--);
        }
      };

      template <class Final, class DiffType>
      struct random_incrementable
          : bidirectional_incrementable<Final>
          , addable_with<Final, DiffType>
          , substractable_with<Final, DiffType>
      {
            using difference_type = DiffType;
      };

    }
  }
}
}

#endif // header
