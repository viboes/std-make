// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/not_null.hpp
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_CHRONO_MODULO_HPP
#define JASEL_FUNDAMENTAL_V3_CHRONO_MODULO_HPP

#include <experimental/fundamental/v2/config.hpp>

#include <ratio>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace chrono
{
  template <class SubDuration, class Duration, class Rep>
  struct modulo final
  {
      using sub_duration = SubDuration;
      using duration = Duration;
      using rep = Rep;

      using RatioPeriod = ratio_divide<typename Duration::period, typename SubDuration::period>;
      static_assert(RatioPeriod::den==1, "Duration must be a multiple of SubDuration");
      static const constexpr intmax_t period = RatioPeriod::num;

      Rep m_value;

      explicit modulo() {}
      explicit modulo(Rep v) : m_value(v) {}

      template <class Rep2>
      modulo(modulo<SubDuration, Duration, Rep2> const& v) : m_value(v.m_value) {}

      explicit modulo(sub_duration v) : m_value(v.count() % period) {}

      constexpr explicit operator sub_duration() {return sub_duration(this->m_value); }
      constexpr sub_duration get_duration() {return sub_duration(this->m_value); }

      constexpr explicit operator Rep() const noexcept {return m_value;}
      constexpr Rep value() const noexcept {return m_value;}

      bool valid() const { return m_value >= 0 && m_value < period; }

      template <class OSTREAM>
      friend OSTREAM& operator<<(OSTREAM& os, modulo const& m) {
          os << uintmax_t(m.m_value);
          return os;
      }

      constexpr modulo& operator++() noexcept {if(++m_value == period) m_value=0; return *this;}
      constexpr modulo operator++(int) noexcept {auto tmp(*this); ++(*this); return tmp;}
      constexpr modulo& operator+=(const sub_duration& d) noexcept {*this = *this + d; return *this;}


      friend constexpr bool operator==(const modulo& x, const modulo& y) noexcept { return x.m_value == y.m_value; }
      friend constexpr bool operator!=(const modulo& x, const modulo& y) noexcept { return x.m_value != y.m_value; }
      friend constexpr bool operator< (const modulo& x, const modulo& y) noexcept { return x.m_value < y.m_value; }
      friend constexpr bool operator> (const modulo& x, const modulo& y) noexcept { return x.m_value > y.m_value; }
      friend constexpr bool operator<=(const modulo& x, const modulo& y) noexcept { return x.m_value <= y.m_value; }
      friend constexpr bool operator>=(const modulo& x, const modulo& y) noexcept { return x.m_value >= y.m_value; }

      friend constexpr modulo  operator+(const modulo&  x, const sub_duration& y) noexcept { return modulo(x.m_value + y.count()); }
      friend constexpr modulo  operator+(const sub_duration& x,  const modulo& y) noexcept { return y + x; }

  };


  /**
  It is not clear yet if the following overloads have a sens

  xfn + sfn -> xsfn
  sfn + xfn -> xsfn

  bfn + sfn -> bsfn
  sfn + bfn -> bsfn

  */

  // requires D1 is a subduration of D3
  // The representation should be big enough to represent all the possible values
  // that depend on modulo::period
  template <class D1, class D2, class D3, class R1, class R2>
  constexpr
      modulo< D1, D3, common_type_t<R1, R2>>
      operator+(modulo<D1, D2, R1> x, modulo<D2, D3, R2> y)
  {
      return modulo< D1, D3, common_type_t<R1, R2>> {D1(x) + D2(y)};
  }

  template <class D1, class D2,class D3, class R1, class R2>
  constexpr
      modulo< D1, D3, common_type_t<R1, R2>>
      operator+(modulo<D2, D3, R2> x, modulo<D1, D2, R1> y)
  {
      return y + x;
  }

  template <class SD2, class D2, class R2, class SD1, class D1, class R1>
  modulo<SD2, D2, R2> modulo_cast2(modulo<SD1, D1, R1> m)
  {
      using rat = ratio_divide<typename SD1::period, typename SD2::period>;
      return modulo<SD2, D2, R2>( (m.value() * rat::num / rat::den) % modulo<SD2, D2, R2>::period );
  }

  template <class M2, class M1>
  M2 modulo_cast(M1 m)
  {
      using SD2 = typename M2::sub_duration;
      using D2 = typename M2::duration;
      using R2 = typename M2::rep;
      return modulo_cast2<SD2, D2, R2>( m );
  }
}


}
}
}

#endif // header
