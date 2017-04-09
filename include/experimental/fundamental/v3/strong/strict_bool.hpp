// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRICT_STRONG_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRICT_STRONG_BOOL_HPP

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
  `strict_bool` is a strongly type that wraps a bool and behaves like an `bool`
  The main goal is to be able to define strong bools that don't mix between them.
  The single conversion is the explicit conversion to the underlying bool type.

  Note that this is not a safe bool, it just forbids the conversions between
  different strong bools types.

  Example
  <code>
  using X = strict_bool<XTag>;
  using Y = strict_bool<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  template <class Tag, class Bool = bool>
  struct strict_bool final : tagged<Tag, Bool>
  {
      using base_type = tagged<Tag, Bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strict_bool() noexcept = default;
      // do we want an explicit conversion from Bool?
      //constexpr explicit strict_bool (Bool) = delete;

      // If Bool is not bool, do we want an explicit conversion from bool?
      //constexpr strict_bool(bool b) noexcept : base_type(b) {}

      // If Bool is not bool, do we want an explicit conversion to bool?
      //constexpr explicit operator bool() const { return value; }

      constexpr explicit strict_bool (int) = delete;
      constexpr explicit strict_bool (double) = delete;
      constexpr explicit strict_bool (void*) = delete;

      //!@{
      // boolean operators
      // todo do We need mixed boolean operators
      friend constexpr strict_bool operator&&(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value && y.value); }
      //friend constexpr strict_bool operator&&(bool x, strict_bool y)  noexcept { return strict_bool(x && y.value); }
      //friend constexpr strict_bool operator&&(strict_bool x, bool y)  noexcept { return strict_bool(x.value && y); }

      friend constexpr strict_bool operator||(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value || y.value); }
      //friend constexpr strict_bool operator||(bool x, strict_bool y)  noexcept { return strict_bool(x || y.value); }
      //friend constexpr strict_bool operator||(strict_bool x, bool y)  noexcept { return strict_bool(x.value || y); }

      friend constexpr strict_bool operator!(strict_bool x)  noexcept { return strict_bool(! x.value); }
      //!@}

      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strict_bool x, strict_bool y)  noexcept { return x.value == y.value; }
      //friend constexpr bool operator==(bool x, strict_bool y)  noexcept { return x == y.value; }
      //friend constexpr bool operator==(strict_bool x, bool y)  noexcept { return x.value == y; }

      friend constexpr bool operator!=(strict_bool x, strict_bool y)  noexcept { return x.value != y.value; }
      //friend constexpr bool operator!=(bool x, strict_bool y)  noexcept { return x != y.value; }
      //friend constexpr bool operator!=(strict_bool x, bool y)  noexcept { return x.value != y; }

      friend constexpr bool operator<(strict_bool x, strict_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strict_bool x, strict_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strict_bool x, strict_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strict_bool x, strict_bool y)  noexcept { return x.value >= y.value; }
      // todo do We need mixed relational operators
      //!@}
  };

  static_assert(std::is_pod<strict_bool<bool>>::value, "");
  static_assert(std::is_trivially_default_constructible<strict_bool<bool>>::value, "");
  static_assert(std::is_trivially_copyable<strict_bool<bool>>::value, "");
  static_assert(std::is_standard_layout<strict_bool<bool>>::value, "");
  static_assert(std::is_trivial<strict_bool<bool>>::value, "");

  //! underlying_type specialization for strict_bool
  template <class Tag, class Bool>
  struct underlying_type<strict_bool<Tag, Bool>> { using type = Bool; };

  // stream operators

  //! input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c strict_bool
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class Tag, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, strict_bool<Tag, T>& x)
  {
    T v;
    is >> v;
    x = strict_bool<Tag, T>(v);
    return is;
  }

  //! output function.
  //! @param os the output stream
  //! @param x the \c strict_bool
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << bool(x.undelying())
  //! @endcode

  template <class CharT, class Traits, class Tag, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strict_bool<Tag, T>& x)
  {
    return os << bool(x.underlying());
  }


}
}

  template <class Tag, class UT>
  struct hash<experimental::strict_bool<Tag,UT>> :
    experimental::wrapped_hash<experimental::strict_bool<Tag,UT>> {};

}

#endif // header
