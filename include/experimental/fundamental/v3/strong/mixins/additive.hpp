// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

// Should the mixin operations be defined using SFINAE on the underlying operations?
// The advantage of not using SFINAE is compile time
// The liability of not using SFINAE is that we define an invalid operation

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
  namespace experimental
  {
    inline  namespace fundamental_v3
  {
    namespace mixin
    {

      template <class Final, class Other>
      struct addable_with
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Other const& y) noexcept
        {
          x._backdoor()._underlying() += y._backdoor()._underlying();
          return x;
        }
        friend constexpr
        Final operator+(Final const& x, Other const& y)  noexcept
        {
          return Final(x._backdoor()._underlying() + y._backdoor()._underlying());
        }

        friend constexpr
        Final operator+(Other const& x, Final const& y)  noexcept
        {
          return y + x;
        }

      };

      template <class Final, class Other>
      struct substractable_with
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Other const& y) noexcept
        {
          x._backdoor()._underlying() -= y._backdoor()._underlying();
          return x;
        }

        friend constexpr Final operator-(Final const& x, Other const& y)  noexcept
        {
          return Final(x._backdoor()._underlying() - y._backdoor()._underlying());
        }

        friend constexpr Other operator-(Final const& x, Final const& y)  noexcept
        {
          return Other(x._backdoor()._underlying() - y._backdoor()._underlying());
        }
      };
      // Single arg
      template <class Final>
      struct increase_base
      {
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x, int) noexcept
        {
          Final tmp(x);
          ++x;
          return tmp;
        }

      };
      template <class Final, class Check=no_check>
      struct addable_base : increase_base<Final>
      {
        friend constexpr Final operator+(Final const&x)  noexcept
        {
          return x;
        }

        friend JASEL_MUTABLE_CONSTEXPR Final& operator++(Final& x) noexcept
        {
          ++x._backdoor()._underlying();
          return x;
        }
      };
      template <class Final>
      struct addable_base<Final, void> : increase_base<Final>
      {
      };

      template <class Final>
      struct decrease_base
      {
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x, int) noexcept
        {
          Final tmp(x);
          --x;
          return tmp;
        }

      };
      template <class Final, class Check=no_check>
      struct substractable_base : decrease_base<Final>
      {
        friend constexpr Final operator-(Final const&x)  noexcept
        {
          return Final(-x._backdoor()._underlying());
        }
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x ) noexcept
        {
          --x._backdoor()._underlying();
          return x;
        }
      };
      template <class Final>
      struct substractable_base<Final, void> : decrease_base<Final>
      {
      };
      template <class Final, class Check=no_check>
      struct additive_base : addable_base<Final, Check>, substractable_base<Final, Check>      {      };

      template <class Final, class Check=no_check>
      struct addable_assign : addable_base<Final, Check>
      {
        // todo These should be moved to homogeneous when I'll add heterogeneous
        //! Forwards to the underlying value
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x._backdoor()._underlying() += y._backdoor()._underlying();
          return x;
        }
      };

      //arithmetic assignment when checked relies in the assignment of the result of the arithmetic operation
      template <class Final>
      struct addable_assign<Final, check> : addable_base<Final, check>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x = x + y;
          return x;
        }
      };
      template <class Final>
      struct addable_assign<Final, void> : addable_base<Final, void>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Final const& y) noexcept
        {
          x = x + y;
          return x;
        }
      };

      template <class Final, class Check=no_check>
      struct substractable_assign : substractable_base<Final, Check>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x._backdoor()._underlying() -= y._backdoor()._underlying();
          return x;
        }

      };

      //arithmetic assignment when checked relies in the assignment of the result of the arithmetic operation
      template <class Final>
      struct substractable_assign<Final, check> : substractable_base<Final, check>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x = x - y;
          return x;
        }
      };
      template <class Final>
      struct substractable_assign<Final, void> : substractable_base<Final, void>
      {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Final const& y) noexcept
        {
          x = x - y;
          return x;
        }
      };


      template <class Final, class Check=no_check>
      struct additive_assign : addable_assign<Final, Check>, substractable_assign<Final, Check>      {      };


      template <class Final, class Check=no_check>
      struct addable : addable_assign<Final, Check>
      {
        friend constexpr Final operator+(Final const& x, Final const& y)  noexcept
        {
          return Final(x._backdoor()._underlying() + y._backdoor()._underlying());
        }
      };
      template <class Final, class Check=no_check>
      struct substractable : substractable_assign<Final, Check>
      {
        friend constexpr Final operator-(Final const& x, Final const& y)  noexcept
        {
          return Final(x._backdoor()._underlying() - y._backdoor()._underlying());
        }
      };
      template <class Final, class Check=no_check>
      struct additive : addable<Final, Check>, substractable<Final, Check>      {      };

      // additive heterogeneous
      template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
      struct additive_with_if : additive_assign<Final, Check>
      {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        common_type_t<Final, Other> operator+(Final const& x, Other const& y)  noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._backdoor()._underlying() + CT(y)._backdoor()._underlying());
        }

        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        common_type_t<Final, Other> operator-(Final const& x, Other const& y)  noexcept
        {
          using CT = common_type_t<Final, Other>;
          return CT(CT(x)._backdoor()._underlying() - CT(y)._backdoor()._underlying());
        }

      };

    }
  }
}
}

#endif // header
