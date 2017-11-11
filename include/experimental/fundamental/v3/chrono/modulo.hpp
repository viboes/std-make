// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_CHRONO_MODULO_HPP
#define JASEL_FUNDAMENTAL_V3_CHRONO_MODULO_HPP

#include <experimental/fundamental/v2/config.hpp>

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/additive.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/integer_multiplicative.hpp>
#include <experimental/fundamental/v3/strong/mixins/hashable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/fundamental/v3/strong/mixins/ordinal.hpp>

#include <experimental/meta/v1/rebind.hpp>
#include <experimental/type_traits.hpp>

#include <experimental/ordinal.hpp>

#include <ratio>
#include <chrono>
#include <limits>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
// todo: Generalize to any counter that has associated a period and a representation.
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

  // todo: Couldn't the representation be deduced from the cardinality?
  // todo: define it using strong_type and mixins: This will need to define modulo arithmetic mixins.
  template <class Duration, class SuperDuration, class Rep>
  struct modulo final
      : strong_type<modulo<Duration, SuperDuration, Rep>, Rep>
      , mixin::comparable<modulo<Duration, SuperDuration, Rep>>
      , mixin::increase_base<modulo<Duration, SuperDuration, Rep>>
      , mixin::decrease_base<modulo<Duration, SuperDuration, Rep>>

  {
  public:
      static_assert(std::is_unsigned<Rep>::value, "Representation must be unsigned");
      static_assert(std::is_integral<typename Duration::rep>::value, "Duration representation must be integral");
      static_assert(std::is_integral<typename SuperDuration::rep>::value, "SuperDuration representation must be integral");
      using period = typename ratio_divide<typename SuperDuration::period, typename Duration::period>::type;
      static_assert(period::den==1, "SuperDuration must be a multiple of Duration");
      static const constexpr intmax_t cardinal = period::num;
      static_assert(period::num <= numeric_limits<Rep>::max(), "The number of elements must be smaller than the max of the representation");

      using duration_t = Duration;
      using super_duration_t = SuperDuration;
      using rep = Rep;

      using base_type = strong_type<modulo<Duration, SuperDuration, Rep>, Rep>;
      using base_type::base_type;

      template <class UT2>
      using rebind = modulo<Duration, SuperDuration, UT2>;


      explicit constexpr modulo() = default;
      modulo(modulo const&) = default;
      modulo(modulo &&) = default;

      //! @par Pre-condition:
      //!   v is in the range [0,cardinal)
      //! @par Effects:<br> constructs a modulo from its representations
      constexpr explicit modulo(Rep v)
          : base_type(v)
      {}

      //! @par Requires:<br> is_convertible<Rep2, Rep>
      //! @par Effects:<br> constructs a modulo converting the representations
      template <class Rep2
      //, typename = enable_if
      >
      constexpr modulo(modulo<Duration, SuperDuration, Rep2> const& v)
          : base_type(static_cast<rep>(v.count()))
      {}

      //! @par Effects:<br> constructs a modulo doing the modulo of the representation
      constexpr explicit modulo(duration_t v)
          : base_type(static_cast<rep>(v.count()) % cardinal)
      {}

      // assignment
      modulo& operator=(modulo const&) = default;
      modulo& operator=(modulo &&) = default;

      //! @par Returns:<br> conversion to duration
      constexpr explicit operator duration_t() const
      {
          return duration_t(count());
      }
      constexpr duration_t to_duration() const
      {
          return duration_t(count());
      }

      //! @par Returns:<br> conversion to the representation
      constexpr explicit operator Rep() const noexcept
      {
          return this->underlying();
      }
      constexpr Rep value() const noexcept
      {
          return this->underlying();
      }
      constexpr Rep count() const noexcept
      {
          return this->underlying();
      }

      //! @par Returns:
      //!   0 <= count() && count() < cardinal
      constexpr bool valid() const
      {
          return count() >= 0 && count() < cardinal;
      }

      template <class OSTREAM>
      friend OSTREAM& operator<<(OSTREAM& os, modulo const& m)
      {
          os << uintmax_t(m.count());
          return os;
      }

      //! @par Returns:
      //!   *this.
      constexpr modulo operator+() const noexcept
      {
          return *this;
      }
      //! @par Returns:<br> modulo{-Rep(*this)}.
      // modulo arithmetic
      constexpr modulo operator-() const noexcept
      {
          return modulo(cardinal - count());
      }

      //! @par Effects: <br>If count() < cardinal, ++_backdoor()._underlying(). Otherwise sets _backdoor()._underlying() to 0.
      //! @par Returns:<br> *this.
      // modulo arithmetic
      JASEL_MUTABLE_CONSTEXPR modulo& operator++() noexcept
      {
          if (this->_backdoor()._underlying() == cardinal-1)
              this->_backdoor()._underlying() = 0;
          else
              ++this->_backdoor()._underlying();
          return *this;
      }

      //! @par Effects: <br>++(*this).
      //! @par Returns:<br> Returns: A copy of *this as it existed on entry to this member function.
      // JASEL_MUTABLE_CONSTEXPR modulo operator++(int) noexcept;

      //! @par Effects: <br>If _backdoor()._underlying() > 0, --_backdoor()._underlying(). Otherwise sets _backdoor()._underlying() to cardinal.
      //! @par Returns:<br> *this.
      // modulo arithmetic
      JASEL_MUTABLE_CONSTEXPR modulo& operator--() noexcept
      {
          if (this->_backdoor()._underlying() == 0)
              this->_backdoor()._underlying() = cardinal;
          else
              --this->_backdoor()._underlying();
          return *this;
      }

      //! @par Effects: <br>--(*this).
      //! @par Returns:<br> Returns: A copy of *this as it existed on entry to this member function.
      // JASEL_MUTABLE_CONSTEXPR modulo operator--(int) noexcept;

      //! @par Effects: <br>*this = *this + d.
      //! @par Returns:<br> *this.
      // modulo arithmetic
      JASEL_MUTABLE_CONSTEXPR modulo& operator+=(const duration_t& d) noexcept
      {
          *this = *this + d;
          return *this;
      }
      //! @par Effects: <br>*this = *this - d.
      //! @par Returns:<br> *this.
      // modulo arithmetic
      JASEL_MUTABLE_CONSTEXPR modulo& operator-=(const duration_t& d) noexcept
      {
          *this = *this - d;
          return *this;
      }

      //! @par Returns:<br> A modulo constructed by the addition of the respective representation.
      //! e.g. hour + hours -> hour
      // modulo arithmetic
      friend constexpr modulo  operator+(const modulo&  x, const duration_t& y) noexcept
      {
          return modulo((x.count() + y.count()) % cardinal);
      }
      //! @par Returns:<br> y + x.
      //! e.g. hours + hour -> hour
      // modulo arithmetic
      friend constexpr modulo  operator+(const duration_t& x,  const modulo& y) noexcept
      {
          return y + x;
      }
      //! @par Returns:<br> A modulo constructed by the difference of the respective representation.
      //! e.g. hour - hours -> hour
      // modulo arithmetic
      friend constexpr modulo  operator-(const modulo&  x, const duration_t& y) noexcept
      {
          //return modulo((cardinal + x.count() - y.count()) % cardinal);
          return x + -y;
      }
      //! @par Returns:<br> The difference in duration_t between the respective durations.
      //! e.g. hour + hour -> hours
      friend constexpr duration_t operator-(const modulo& x,  const modulo& y) noexcept
      {
          return x.count() - y.count();
      }

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
  // fixme: do we want to be able to add two modulo, e.g.
  // hour + minute?
  // frame_number + subframe_number?
  // What will be the result? x_subframe_number. Can it be deduced?
  // frame_number -> frames/x_frames
  // subframe_number -> subframes/frames
  // so the result must be modulo<SUB, SUPER, RRR>
  // SUB must be the common_type of sub duration => subframes
  // SUPER must be the the other type of the common_type of the super durations => x_frames
  // RRR must be the representation able to store the value

  // HH's Date library uses hour/minute/second to concatenate relative units


  // bi_frame_number + subframe_number?
  // What will be the result? bi_subframe_number. Can it be deduced?
  // bi_frame_number -> frames/bi_frames
  // subframe_number -> subframes/frames
  // so the result must be modulo<SUB, SUPER, RRR>
  // SUB must be the common_type of sub duration => subframes
  // SUPER must be the the other type of the common_type of the super durations => bi_frames
  // RRR must be the representation able to store the value

  // frame_number + bi_subframe_number?
  // What will be the result? bi_subframe_number. Can it be deduced?
  // frame_number -> frames
  // bi_subframe_number -> bi_subframes
  // common_type bi_subframes
  // so the result must be modulo<subframes, XXX, RRR>
  // XXX must be the super duration => frames
  // RRR must be the

#if ! defined JASEL_DOXYGEN_INVOKED

  namespace  chrono_detail
  {
    template <class T>
    struct is_modulo : false_type {};
    template <class D, class SD, class R>
    struct is_modulo<modulo<D,SD,R>> : true_type{};

  //! @par Requires:<br> modulo<D,SD,R> is more fine grained than ModuloTo
  //! @par Returns:<br> The ModuloTo of the duration_cast<typename ModuloTo::duration_t>()
  //! @par Note:<br> The duration cast is needed because the representation
  template <class ModuloTo, class D, class SD, class R>
#if defined JASEL_DOXYGEN_INVOKED
  ModuloTo
#else
  enable_if_t <
    chrono_detail::is_modulo<ModuloTo>::value
    && (is_same<SD, typename ModuloTo::super_duration_t>::value || is_same<D, typename ModuloTo::duration_t>::value)
    , ModuloTo
  >
#endif
  to_modulo(modulo<D,SD,R> m)
  {
      using duration_to = typename ModuloTo::duration_t;
      return ModuloTo(
          std::chrono::duration_cast<duration_to>(m.to_duration())
          );
  }
}
#endif

  template <class ModuloTo, class SuperDuration, class SubDuration, class Duration, class Rep>
  constexpr
#if defined JASEL_DOXYGEN_INVOKED
  ModuloTo
#else
  enable_if_t <
    chrono_detail::is_modulo<ModuloTo>::value
  , ModuloTo
  >
#endif
  modulo_cast(modulo<SubDuration, Duration, Rep> m)
  {
    using SubDurationSuperDuration = modulo<SubDuration,   SuperDuration,    std::uint64_t>;
    using SubModSuperDuration = modulo<typename ModuloTo::duration_t,   SuperDuration,    std::uint64_t>;
    using ModuloToX = modulo<typename ModuloTo::duration_t,   typename ModuloTo::super_duration_t,    std::uint64_t>;

    return chrono_detail::to_modulo<ModuloToX>(
              chrono_detail::to_modulo<SubModSuperDuration>(
                 chrono_detail::to_modulo<SubDurationSuperDuration>(m)
              )
           );
  }

} //chrono

namespace ordinal {
  //! chrono::moduloD,SD,R> is an ordinal type that can be used with the ordinal containers
  template <class D, class SD, class R>
  struct traits<chrono::modulo<D,SD,R>>
  {
    using size_type = ordinal::index_t;
    using size = integral_constant<ordinal::index_t, chrono::modulo<D,SD,R>::cardinal>;

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
