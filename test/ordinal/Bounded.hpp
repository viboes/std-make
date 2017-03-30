// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)


#ifndef JASEL_EXPERIMENTAL_TEST_ORDINAL_BOUNDED_HPP
#define JASEL_EXPERIMENTAL_TEST_ORDINAL_BOUNDED_HPP

#include <experimental/ordinal.hpp>
#if __cplusplus >= 201402L

template <std::intmax_t F, std::intmax_t L, class T>
struct Bounded {
  using underlying_type = T;
  static constexpr const T First = F;
  static constexpr const T Last = F;
  static constexpr const std::size_t Size = L-F+1;
  T value;
};

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace ordinal {
    template <std::intmax_t F1, std::intmax_t L1, class T1>
    struct traits<Bounded<F1,L1, T1>>
    {
      using size_type = size_t;
      using size = integral_constant<size_t, Bounded<F1,L1,T1>::Size>;

      //template <class U>
      static constexpr
      Bounded<F1,L1, T1> val(size_type pos)  {
        return Bounded<F1,L1, T1>{static_cast<typename Bounded<F1,L1, T1>::underlying_type>(Bounded<F1,L1, T1>::First+pos)};
      }

      //template <class U>
      static constexpr
      size_t pos(Bounded<F1,L1, T1> val)  {
        return val.value-Bounded<F1,L1, T1>::First;
      }
    };
  }
}}
}

#endif
#endif
