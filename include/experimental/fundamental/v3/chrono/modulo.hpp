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
#include <chrono>
#include <limits>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace chrono
{
//  modulo is a trivially copyable class type.
//  modulo is a standard-layout class type.
//  modulo is a literal class type.

  //!  modulo is a trivially copyable class type if Rep it is.
  //!  modulo is a standard-layout class type if Rep it is.
  //!  modulo is a literal class type if Rep it is.
  template <class SubDuration, class Duration, class Rep>
  struct modulo final
  {
      //using period = SubDuration;
      using sub_duration = SubDuration;
      using duration = Duration;
      using rep = Rep;

      using RatioPeriod = ratio_divide<typename Duration::period, typename SubDuration::period>;
      static_assert(RatioPeriod::den==1, "Duration must be a multiple of SubDuration");
      static const constexpr intmax_t period = RatioPeriod::num;

      Rep m_value;

      constexpr explicit modulo() {}
      constexpr explicit modulo(Rep v) : m_value(v) {}

      template <class Rep2>
      constexpr modulo(modulo<SubDuration, Duration, Rep2> const& v) : m_value(v.m_value) {}

      constexpr explicit modulo(sub_duration v) : m_value(v.count() % period) {}

      constexpr explicit operator sub_duration() { return sub_duration(this->m_value); }
      constexpr sub_duration to_duration() { return sub_duration(this->m_value); }

      constexpr explicit operator Rep() const noexcept {return m_value;}
      constexpr Rep value() const noexcept {return m_value;}
      constexpr Rep count() const noexcept {return m_value;}

      bool valid() const { return m_value >= 0 && m_value < period; }

      template <class OSTREAM>
      friend OSTREAM& operator<<(OSTREAM& os, modulo const& m) {
          os << uintmax_t(m.count());
          return os;
      }

      constexpr modulo operator+() noexcept {return *this;}
      constexpr modulo operator-() noexcept {return modulo(-m_value);}

      constexpr modulo& operator++() noexcept {if(++m_value == period) m_value=0; return *this;}
      constexpr modulo operator++(int) noexcept {auto tmp(*this); ++(*this); return tmp;}

      constexpr modulo& operator--() noexcept { if (m_value == 0) m_value=period; else --m_value; return *this;}
      constexpr modulo operator--(int) noexcept {auto tmp(*this); --(*this); return tmp;}

      constexpr modulo& operator+=(const sub_duration& d) noexcept {*this = *this + d; return *this;}
      constexpr modulo& operator-=(const sub_duration& d) noexcept {*this = *this - d; return *this;}

      friend constexpr bool operator==(const modulo& x, const modulo& y) noexcept { return x.count() == y.count(); }
      friend constexpr bool operator!=(const modulo& x, const modulo& y) noexcept { return x.count() != y.count(); }
      friend constexpr bool operator< (const modulo& x, const modulo& y) noexcept { return x.count() < y.count(); }
      friend constexpr bool operator> (const modulo& x, const modulo& y) noexcept { return x.count() > y.count(); }
      friend constexpr bool operator<=(const modulo& x, const modulo& y) noexcept { return x.count() <= y.count(); }
      friend constexpr bool operator>=(const modulo& x, const modulo& y) noexcept { return x.count() >= y.count(); }

      friend constexpr modulo  operator+(const modulo&  x, const sub_duration& y) noexcept { return modulo(x.count() + y.count()); }
      friend constexpr modulo  operator+(const sub_duration& x,  const modulo& y) noexcept { return y + x; }
      friend constexpr modulo  operator-(const modulo&  x, const sub_duration& y) noexcept { return modulo(x.count() - y.count()); }
      friend constexpr sub_duration operator-(const modulo& x,  const modulo& y) noexcept { return x.count() - y.count(); }

      //@ @par Returns: modulo{0}.
      static constexpr modulo min() noexcept
      {
        return modulo(0);
      }
      //@ @par Returns: modulo{numeric_limits<Rep>::max()}.
      static constexpr modulo max() noexcept
      {
        return modulo(period-1);
      }
  };

  template <class ModuloTo, class ModuloFrom>
  ModuloTo to_modulo(ModuloFrom m)
  {
      return ModuloTo(
          std::chrono::duration_cast<typename ModuloTo::sub_duration>(m.to_duration())
          );
  }

}


}
}
}

#endif // header
