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
#include <experimental/ordinal.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

namespace chrono
{

  //! The modulo class breaks a duration, into a "broken" down sub duration.
  //! The Duration template parameter dictates the precision to which the super Duration is broken down.
  //! modulo is primarily a splitting tool.
  //!
  //! @par Requires: The ratio of the Super duration period and the duration period must be integral
  //!
  //! @par Features:
  //!  modulo is a trivially copyable class type if Rep it is.
  //!  modulo is a standard-layout class type if Rep it is.
  //!  modulo is a literal class type if Rep it is.
  //!
  //! @par Examples:
  //!   using weekday = modulo<days,   weeks,    std::uint8_t>; //7

  template <class Duration, class SuperDuration, class Rep>
  class modulo final
  {
      Rep m_value;
  public:
      using period = typename ratio_divide<typename SuperDuration::period, typename Duration::period>::type;
      static_assert(period::den==1, "SuperDuration must be a multiple of Duration");
      static const constexpr intmax_t cardinal = period::num;

      using duration_t = Duration;
      using super_duration_t = SuperDuration;
      using rep = Rep;

      explicit modulo() = default;

      //! @par Effects: constructs a modulo from its representations
      constexpr explicit modulo(Rep v) : m_value(v) {}

      //! @par Requires: is_convertible<Rep2, Rep>
      //! @par Effects: constructs a modulo converting the representations
      template <class Rep2>
      constexpr modulo(modulo<Duration, SuperDuration, Rep2> const& v) : m_value(v.m_value) {}

      //! @par Effects: constructs a modulo doing the modulo of the representation
      constexpr explicit modulo(duration_t v) : m_value(v.count() % cardinal) {}

      //! @par Returns: conversion to duration
      constexpr explicit operator duration_t() { return duration_t(this->m_value); }
      constexpr duration_t to_duration() { return duration_t(this->m_value); }

      //! @par Returns: conversion to the representation
      constexpr explicit operator Rep() const noexcept {return m_value;}
      constexpr Rep value() const noexcept {return m_value;}
      constexpr Rep count() const noexcept {return m_value;}

      //! @par Returns: min() <= conut() && conut() <= max()
      bool valid() const { return m_value >= 0 && m_value < cardinal; }

      template <class OSTREAM>
      friend OSTREAM& operator<<(OSTREAM& os, modulo const& m) {
          os << uintmax_t(m.count());
          return os;
      }

      //! @par Returns: *this.
      constexpr modulo operator+() noexcept
          { return *this; }
      //! @par Returns: modulo{-Rep(*this)}.
      constexpr modulo operator-() noexcept
          { return modulo(-m_value); }

      //! @par Effects: If m_value < candinal, ++m_value. Otherwise sets m_value to 0.
      //! @par Returns: *this.
      constexpr modulo& operator++() noexcept
          { if (++m_value == cardinal) m_value=0; return *this; }

      //! @par Effects: ++(*this).
      //! @par Returns: Returns: A copy of *this as it existed on entry to this member function.
      constexpr modulo operator++(int) noexcept
          { auto tmp(*this); ++(*this); return tmp; }

      //! @par Effects: If m_value > 0, --m_value. Otherwise sets m_value to cardinal.
      //! @par Returns: *this.
      constexpr modulo& operator--() noexcept
          { if (m_value == 0) m_value=cardinal; else --m_value; return *this;}

      //! @par Effects: --(*this).
      //! @par Returns: Returns: A copy of *this as it existed on entry to this member function.
      constexpr modulo operator--(int) noexcept
          { auto tmp(*this); --(*this); return tmp; }

      //! @par Effects: *this = *this + d.
      //! @par Returns: *this.
      constexpr modulo& operator+=(const duration_t& d) noexcept
          { *this = *this + d; return *this; }
      //! @par Effects: *this = *this - d.
      //! @par Returns: *this.
      constexpr modulo& operator-=(const duration_t& d) noexcept
          { *this = *this - d; return *this; }

      //! @par Returns: Rep{x} == Rep{y}.
      friend constexpr bool operator==(const modulo& x, const modulo& y) noexcept
          { return x.count() == y.count(); }
      //! @par Returns: Rep{x} != Rep{y}.
      friend constexpr bool operator!=(const modulo& x, const modulo& y) noexcept
          { return x.count() != y.count(); }
      //! @par Returns: Rep{x} < Rep{y}.
      friend constexpr bool operator< (const modulo& x, const modulo& y) noexcept
          { return x.count() < y.count(); }
      //! @par Returns: Rep{x} > Rep{y}.
      friend constexpr bool operator> (const modulo& x, const modulo& y) noexcept
          { return x.count() > y.count(); }
      //! @par Returns: Rep{x} <= Rep{y}.
      friend constexpr bool operator<=(const modulo& x, const modulo& y) noexcept
          { return x.count() <= y.count(); }
      //! @par Returns: Rep{x} >= Rep{y}.
      friend constexpr bool operator>=(const modulo& x, const modulo& y) noexcept
          { return x.count() >= y.count(); }

      //! @par Returns: A modulo constructed by the addition of the respective representation.
      friend constexpr modulo  operator+(const modulo&  x, const duration_t& y) noexcept
          { return modulo(x.count() + y.count()); }
      //! @par Returns: y + x.
      friend constexpr modulo  operator+(const duration_t& x,  const modulo& y) noexcept
          { return y + x; }
      //! @par Returns: A modulo constructed by the difference of the respective representation.
      friend constexpr modulo  operator-(const modulo&  x, const duration_t& y) noexcept
          { return x + -y; }
      //! @par Returns: The difference in duration_t between the respective durations.
      friend constexpr duration_t operator-(const modulo& x,  const modulo& y) noexcept
          { return x.count() - y.count(); }

      //@ @par Returns: modulo{0}.
      static constexpr modulo min() noexcept
      {
        return modulo(0);
      }
      //@ @par Returns: modulo{cardinal-1}.
      static constexpr modulo max() noexcept
      {
        return modulo(cardinal-1);
      }
  };
  namespace  chrono_detail
  {
    template <class T>
    struct is_modulo : false_type {};
    template <class D, class SD, class R>
    struct is_modulo<modulo<D,SD,R>> : true_type{};
  }
  //! @par Requires modulo<D,SD,R> is more fine grained than ModuloTo
  //! @par Returns The ModuloTo of the duration_cast<typename ModuloTo::duration_t>()
  //! @par Note: The duration cast is needed because the representation
  template <class ModuloTo, class D, class SD, class R>
  typename enable_if <
    chrono_detail::is_modulo<ModuloTo>::value, ModuloTo
  >::type
  to_modulo(modulo<D,SD,R> m)
  {
      using duration_to = typename ModuloTo::duration_t;
      return ModuloTo(
          std::chrono::duration_cast<duration_to>(m.to_duration())
          );
  }

} //chrono

//! chrono::moduloD,SD,R> is a -n ordinal type that can be used with the ordinal containers
namespace ordinal {
  namespace meta {
    template <class D, class SD, class R>
    struct size<chrono::modulo<D,SD,R>> : integral_constant<size_t, chrono::modulo<D,SD,R>::cardinal> {};
  }
  template <class D, class SD, class R>
  struct traits<chrono::modulo<D,SD,R>> : ordinal::tag
  {
    using size_type = size_t;
    template <class U>
    static constexpr
    U val(size_type pos)  {
      return U{static_cast<typename U::rep>(pos)};
    }

    template <class U>
    static constexpr
    size_type pos(U val)    {
      return static_cast<size_type>(val.count());
    }
  };
} // ordinal

}
}
}

#endif // header
