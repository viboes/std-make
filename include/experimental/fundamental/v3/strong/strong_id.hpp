// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <type_traits>
#include <functional>
#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_id` is a strongly type that wraps a regular type and behaves like a `Regular` type
  The main goal is to be able to define strong identifiers that don't mix between them.
  No conversion to the underlying integer type is provided.

  Example
  <code>
  using X = strong_id<XTag>;
  using Y = strong_id<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  template <class Tag, class UT = int>
  struct strong_id final : private_tagged<Tag, UT>
  {
      using base_type = private_tagged<Tag, UT>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_id() noexcept = default;


      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strong_id x, strong_id y)  noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_id x, strong_id y)  noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_id x, strong_id y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_id x, strong_id y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_id x, strong_id y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_id x, strong_id y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  static_assert(std::is_pod<strong_id<int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_id<int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_id<int>>::value, "");
  static_assert(std::is_standard_layout<strong_id<int>>::value, "");
  static_assert(std::is_trivial<strong_id<int>>::value, "");

  //! underlying_type specialization for strong_id
  template <class Tag, class UT>
  struct underlying_type<strong_id<Tag, UT>> { using type = UT; };

  // stream operators

  //! input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c strong_id
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class Tag, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, strong_id<Tag, T>& x)
  {
    T v;
    is >> v;
    x = strong_id<Tag, T>(v);
    return is;
  }

  //! output function.
  //! @param os the output stream
  //! @param x the \c strong_id
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << x.undelying()
  //! @endcode

  template <class CharT, class Traits, class Tag, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strong_id<Tag, T>& x)
  {
    return os << x.underlying();
  }

}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_id<Tag, UT>>
    : experimental::wrapped_hash<experimental::strong_id<Tag, UT>> {};

}

#endif // header
