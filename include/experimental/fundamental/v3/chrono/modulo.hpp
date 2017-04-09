// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
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
  //! @par Requires:<br> The ratio of the Super duration period and the duration period must be integral
  //!
  //! @par Features:<br>
  //!  * modulo is a trivially copyable class type if Rep it is.
  //!  * modulo is a standard-layout class type if Rep it is.
  //!  * modulo is a literal class type if Rep it is.
  //!
  //! @par Examples:
  //! @code
  //!   using weekday = modulo<days,   weeks,    std::uint8_t>; //7
  //! @endcode

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

      //! @par Effects:<br> constructs a modulo from its representations
      constexpr explicit modulo(Rep v) : m_value(v) {}

      //! @par Requires:<br> is_convertible<Rep2, Rep>
      //! @par Effects:<br> constructs a modulo converting the representations
      template <class Rep2>
      constexpr modulo(modulo<Duration, SuperDuration, Rep2> const& v) : m_value(v.m_value) {}

      //! @par Effects:<br> constructs a modulo doing the modulo of the representation
      constexpr explicit modulo(duration_t v) : m_value(v.count() % cardinal) {}

      //! @par Returns:<br> conversion to duration
      constexpr explicit operator duration_t() { return duration_t(this->m_value); }
      constexpr duration_t to_duration() { return duration_t(this->m_value); }

      //! @par Returns:<br> conversion to the representation
      constexpr explicit operator Rep() const noexcept {return m_value;}
      constexpr Rep value() const noexcept {return m_value;}
      constexpr Rep count() const noexcept {return m_value;}

      //! @par Returns:<br> min() <= conut() && conut() <= max()
      bool valid() const { return m_value >= 0 && m_value < cardinal; }

      template <class OSTREAM>
      friend OSTREAM& operator<<(OSTREAM& os, modulo const& m) {
          os << uintmax_t(m.count());
          return os;
      }

      //! @par Returns:<br> *this.
      constexpr modulo operator+() noexcept
          { return *this; }
      //! @par Returns:<br> modulo{-Rep(*this)}.
      constexpr modulo operator-() noexcept
          { return modulo(-m_value); }

      //! @par Effects: <br>If m_value < candinal, ++m_value. Otherwise sets m_value to 0.
      //! @par Returns:<br> *this.
      constexpr modulo& operator++() noexcept
          { if (++m_value == cardinal) m_value=0; return *this; }

      //! @par Effects: <br>++(*this).
      //! @par Returns:<br> Returns: A copy of *this as it existed on entry to this member function.
      constexpr modulo operator++(int) noexcept
          { auto tmp(*this); ++(*this); return tmp; }

      //! @par Effects: <br>If m_value > 0, --m_value. Otherwise sets m_value to cardinal.
      //! @par Returns:<br> *this.
      constexpr modulo& operator--() noexcept
          { if (m_value == 0) m_value=cardinal; else --m_value; return *this;}

      //! @par Effects: <br>--(*this).
      //! @par Returns:<br> Returns: A copy of *this as it existed on entry to this member function.
      constexpr modulo operator--(int) noexcept
          { auto tmp(*this); --(*this); return tmp; }

      //! @par Effects: <br>*this = *this + d.
      //! @par Returns:<br> *this.
      constexpr modulo& operator+=(const duration_t& d) noexcept
          { *this = *this + d; return *this; }
      //! @par Effects: <br>*this = *this - d.
      //! @par Returns:<br> *this.
      constexpr modulo& operator-=(const duration_t& d) noexcept
          { *this = *this - d; return *this; }

      //! @par Returns:<br> Rep{x} == Rep{y}.
      friend constexpr bool operator==(const modulo& x, const modulo& y) noexcept
          { return x.count() == y.count(); }
      //! @par Returns:<br> Rep{x} != Rep{y}.
      friend constexpr bool operator!=(const modulo& x, const modulo& y) noexcept
          { return x.count() != y.count(); }
      //! @par Returns:<br> Rep{x} < Rep{y}.
      friend constexpr bool operator< (const modulo& x, const modulo& y) noexcept
          { return x.count() < y.count(); }
      //! @par Returns:<br> Rep{x} > Rep{y}.
      friend constexpr bool operator> (const modulo& x, const modulo& y) noexcept
          { return x.count() > y.count(); }
      //! @par Returns:<br> Rep{x} <= Rep{y}.
      friend constexpr bool operator<=(const modulo& x, const modulo& y) noexcept
          { return x.count() <= y.count(); }
      //! @par Returns:<br> Rep{x} >= Rep{y}.
      friend constexpr bool operator>=(const modulo& x, const modulo& y) noexcept
          { return x.count() >= y.count(); }

      //! @par Returns:<br> A modulo constructed by the addition of the respective representation.
      friend constexpr modulo  operator+(const modulo&  x, const duration_t& y) noexcept
          { return modulo(x.count() + y.count()); }
      //! @par Returns:<br> y + x.
      friend constexpr modulo  operator+(const duration_t& x,  const modulo& y) noexcept
          { return y + x; }
      //! @par Returns:<br> A modulo constructed by the difference of the respective representation.
      friend constexpr modulo  operator-(const modulo&  x, const duration_t& y) noexcept
          { return x + -y; }
      //! @par Returns:<br> The difference in duration_t between the respective durations.
      friend constexpr duration_t operator-(const modulo& x,  const modulo& y) noexcept
          { return x.count() - y.count(); }

      //@ @par Returns:<br> modulo{0}.
      static constexpr modulo min() noexcept
      {
        return modulo(0);
      }
      //@ @par Returns:<br> modulo{cardinal-1}.
      static constexpr modulo max() noexcept
      {
        return modulo(cardinal-1);
      }
  };
#if ! defined JASEL_DOXYGEN_INVOKED

  namespace  chrono_detail
  {
    template <class T>
    struct is_modulo : false_type {};
    template <class D, class SD, class R>
    struct is_modulo<modulo<D,SD,R>> : true_type{};
  }
#endif
  //! @par Requires:<br> modulo<D,SD,R> is more fine grained than ModuloTo
  //! @par Returns:<br> The ModuloTo of the duration_cast<typename ModuloTo::duration_t>()
  //! @par Note:<br> The duration cast is needed because the representation
  template <class ModuloTo, class D, class SD, class R>
#if defined JASEL_DOXYGEN_INVOKED
  ModuloTo
#else
  typename enable_if <
    chrono_detail::is_modulo<ModuloTo>::value, ModuloTo
  >::type
#endif
  to_modulo(modulo<D,SD,R> m)
  {
      using duration_to = typename ModuloTo::duration_t;
      return ModuloTo(
          std::chrono::duration_cast<duration_to>(m.to_duration())
          );
  }

} //chrono

namespace ordinal {
  //! chrono::moduloD,SD,R> is a -n ordinal type that can be used with the ordinal containers
  template <class D, class SD, class R>
  struct traits<chrono::modulo<D,SD,R>> : ordinal::tag
  {
    using size_type = size_t;
    using size = integral_constant<size_t, chrono::modulo<D,SD,R>::cardinal>;

    //template <class U>
    static constexpr
    chrono::modulo<D,SD,R> val(size_type pos)  {
      return chrono::modulo<D,SD,R>{static_cast<typename chrono::modulo<D,SD,R>::rep>(pos)};
    }

    //template <class U>
    static constexpr
    size_type pos(chrono::modulo<D,SD,R> val)    {
      return static_cast<size_type>(val.count());
    }
  };
} // ordinal

}
}
}

#endif // header
