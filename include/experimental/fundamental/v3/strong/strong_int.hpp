// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_INT_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/ordinal.hpp>

#include <cstdint>
#include <limits>
#include <functional>
#include <iosfwd>


namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_int` is a strongly type tagged by the tag @c Tag that wraps an integral type @c UT and behaves like an @c UT
  The main goal is to be able to define strong integers that don't mix between them.
  No implicit conversion to the underlying integer type is provided.

  Note that this is not a safe int, it just forbids the conversions between
  different strong integers types with non compatible tags.

  Example
  <code>
  using X = strong_int<XTag, int>;
  using Y = strong_int<YTag, int>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */
  template <class Tag, class UT>
  struct strong_int;

  namespace mixin {
    template <class Tag, class UT1, class UT2>
    struct is_comparable_with<strong_int<Tag, UT1>, strong_int<Tag, UT2>> : true_type {};
  }

  //fixme Why private_tagged?
  template <class Tag, class UT>
  struct strong_int final
    : private_strong_type<strong_int<Tag, UT>, UT>
    , mixin::comparable_with_if<strong_int<Tag, UT>>
    , mixin::streamable<strong_int<Tag, UT>>
  {
      static_assert(is_integral<UT>::value, "UT must be integral");

      using base_type = private_strong_type<strong_int<Tag, UT>, UT>;
      using base_type::base_type;

      /// @par Effects Constructs an uninitialized strong_int
      strong_int() = default;
      strong_int(strong_int const&) = default;

      /// @par Effects Implicitly constructs a strong_int from another strong_int with implicit convertible underlying types
      /// @par Throws: Whatever the underlying copy can throw.
      /// @par Remarks: This overload participate in overload resolution only if
      ///   is_constructible_v<UT, UT2 const&> && is_convertible_v<UT2 const&, UT>
      template <class UT2
#if ! defined JASEL_DOXYGEN_INVOKED
      //fixme  error: constructor cannot be redeclared when using , typename = typename enable_if<>::type
        , enable_if_t<
            is_constructible<UT, UT2 const&>::value &&
            is_convertible<UT2 const&, UT>::value
            , bool> = false
#endif
      >
      strong_int(strong_int<Tag, UT2> const& other)
        : base_type(other.underlying())
      {}

      /// @par Effects Explicitly constructs a strong_int from another strong_int with explicit convertible underlying types
      /// @par Throws: Whatever the underlying copy can throw.
      /// @par Remarks: This overload participate in overload resolution only if
      ///   is_constructible_v<UT, UT2 const&> && ! is_convertible_v<UT2 const&, UT>
      template <class UT2
#if ! defined JASEL_DOXYGEN_INVOKED
        ,  typename = enable_if_t<
            is_constructible<UT, UT2 const&>::value &&
            ! is_convertible<UT2 const&, UT>::value
          >
#endif
      >
      explicit strong_int(strong_int<Tag, UT2> const& other)
        : base_type(other.underlying())
      {}

      // todo add assignment?

      // additive operators
      JASEL_MUTABLE_CONSTEXPR strong_int& operator+=(strong_int y)  noexcept
      { this->value += y.underlying(); return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_int operator++()  noexcept
      { return strong_int(++this->value); }
      JASEL_MUTABLE_CONSTEXPR strong_int operator++(int)  noexcept
      { return strong_int(this->value++); }

      JASEL_MUTABLE_CONSTEXPR strong_int& operator-=(strong_int y)  noexcept
      { this->value -= y.underlying(); return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_int operator--()  noexcept
      { return strong_int(--this->value); }
      JASEL_MUTABLE_CONSTEXPR strong_int operator--(int)  noexcept
      { return strong_int(this->value--); }

      //  Multiplicative operators
      JASEL_MUTABLE_CONSTEXPR strong_int& operator*=(strong_int y)  noexcept
      { this->value *= y.underlying(); return *this; }

      JASEL_MUTABLE_CONSTEXPR strong_int& operator/=(strong_int y)  noexcept
      { this->value /= y.underlying(); return *this; }

      JASEL_MUTABLE_CONSTEXPR strong_int& operator%=(strong_int y)  noexcept
      { this->value %= y.underlying(); return *this; }

      JASEL_MUTABLE_CONSTEXPR strong_int& operator&=(strong_int y)  noexcept
      { this->value &= y.underlying(); return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_int& operator|=(strong_int y)  noexcept
      { this->value |= y.underlying(); return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_int& operator^=(strong_int y)  noexcept
      { this->value ^= y.underlying(); return *this; }

      // fixme: do we want the Bitwise logic operators for an integer? if strong_int should be a replacement of any int, yes.
      // This doesn't mean that we cannot have more specialized types

      // Bitwise logic operators
      // fixme: Should the << parameter be int?
      JASEL_MUTABLE_CONSTEXPR strong_int& operator<<=(strong_int y)  noexcept
      { this->value <<= y.value; return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_int& operator>>=(strong_int y)  noexcept
      { this->value >>= y.value; return *this; }

  };

  /// @par Returns A strong int with the tag `Tag` wrapping the value `x`
  template <class Tag, class R>
  constexpr strong_int<Tag, R> make_strong_int(R x)
  {
    return strong_int<Tag, R>(x);
  }

  // additive operators
  template <class Tag, class R>
  constexpr strong_int<Tag,R> operator+(strong_int<Tag,R> x)  noexcept
  {
    return x;
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator+(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() + y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() + y.underlying());
  }

  template <class Tag, class R>
  constexpr auto operator-(strong_int<Tag,R> x)  noexcept -> decltype(make_strong_int<Tag>(-x.underlying()))
  {
    return make_strong_int<Tag>(-x.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator-(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() - y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() - y.underlying());
  }

  //  Multiplicative operators
  template <class Tag, class R1, class R2>
  constexpr auto operator*(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() * y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() * y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator/(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() / y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() / y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator%(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() * y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() % y.underlying());
  }


  // Bitwise logic operators

  template <class Tag, class R>
  constexpr strong_int<Tag,R> operator~(strong_int<Tag,R> x)  noexcept
  {
    return strong_int<Tag,R>(~x.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator&(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() & y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() & y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator|(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() | y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() | y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator^(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() ^ y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() ^ y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator<<(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() << y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() << y.underlying());
  }

  template <class Tag, class R1, class R2>
  constexpr auto operator>>(strong_int<Tag,R1> x, strong_int<Tag,R2> y)  noexcept -> decltype(make_strong_int<Tag>(x.underlying() >> y.underlying()))
  {
    return make_strong_int<Tag>(x.underlying() >> y);
  }

  // fixme do we need swap?

  //! underlying_type specialization for strong_int
  template <class Tag, class UT>
  struct underlying_type<strong_int<Tag,UT>> { using type = UT; };

  static_assert(std::is_pod<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_int<bool,int>>::value, "");
  static_assert(std::is_standard_layout<strong_int<bool,int>>::value, "");
  static_assert(std::is_trivial<strong_int<bool,int>>::value, "");

  // fixme: Should integer be a strong int without tag?
  using integer = strong_int<default_tag, int>;
  using integer8_t = strong_int<default_tag, int8_t>;
  using uinteger8_t = strong_int<default_tag, uint8_t>;
  using integer16_t = strong_int<default_tag, int16_t>;
  using uinteger16_t = strong_int<default_tag, uint16_t>;
  using integer32_t = strong_int<default_tag, int32_t>;
  using uinteger32_t = strong_int<default_tag, uint32_t>;
  using integer64_t = strong_int<default_tag, int64_t>;
  using uinteger64_t = strong_int<default_tag, uint64_t>;

  namespace ordinal {
    /// A strong_int is an ordinal type if the underlying type is an ordinal type
    template <class Tag, class T>
    struct traits<strong_int<Tag, T>> : wrapped_ordinal_traits<strong_int<Tag, T>>    { };
  }
}
}

  /// Hash specialization forwarding to the hash of underlying type
  template <class Tag, class UT>
  struct hash<experimental::strong_int<Tag,UT>>
    : experimental::wrapped_hash<experimental::strong_int<Tag, UT>> {};

  /// numeric_limits specialization forwarding to the numeric_limits of underlying type
  template <class Tag, class UT>
  struct numeric_limits<experimental::strong_int<Tag,UT>> : numeric_limits<UT> {  };


}
#endif // header
