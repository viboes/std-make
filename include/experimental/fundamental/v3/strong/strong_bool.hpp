// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_BOOL_HPP

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/ordinal.hpp>

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
  `strong_bool` is a strongly type that wraps a bool and behaves like an `bool`
  The main goal is to be able to define strong bools that don't mix between them.
  The single conversion is the explicit conversion to bool type.
  This is a safe bool, in the sense that it doesn't allows conversions from integral, floating point and pointer types.
  In addition it forbids conversions between different strong bools types.

  As any tagged type, it makes the parameter more explicit.
  Example
  <code>
  using X = strong_bool<XTag>;
  using Y = strong_bool<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  // Do we need a Bool parameter? Which type will be a good candidate?
  // bool takes 4 bytes in some machines. Sometimes we want to use just 1 byte
  template <class Tag, class Bool = bool>
  struct strong_bool final : tagged<Tag, Bool>
  {
      using base_type = tagged<Tag, Bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_bool() noexcept = default;
      // If Bool is not bool, we want an explicit conversion from bool but not Bool.
      constexpr explicit strong_bool (Bool) = delete;
      constexpr explicit strong_bool (bool b) : base_type(b) {};

      // If Bool is not bool, we want an explicit conversion to bool
      constexpr explicit operator bool() const { return bool(this->value); }

      // unwanted conversions
      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strong_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strong_bool (R(C::*)(Args...)) = delete;

      //!@{
      // boolean operators
      // fixme do we need these && and || operators?
      // strong_bool is explicit convertible to bool and so && and || operator will force the conversion to bool
      // If we don't define them we need to be explicit to convert to strong_bool
      // If we define them, we need to be explicit to convert to bool.
      // todo do we need mixed boolean operators?
      // If yes, which would be the result. IMO, it should be bool.
      // If not, there will be a conversion to bool and the result is bool
      friend constexpr strong_bool operator&&(strong_bool x, strong_bool y)  noexcept { return strong_bool(x.value && y.value); }
      //friend constexpr strong_bool operator&&(bool x, strong_bool y)  noexcept { return strong_bool(x && y.value); }
      //friend constexpr strong_bool operator&&(strong_bool x, bool y)  noexcept { return strong_bool(x.value && y); }

      friend constexpr strong_bool operator||(strong_bool x, strong_bool y)  noexcept { return strong_bool(x.value || y.value); }
      //friend constexpr strong_bool operator||(bool x, strong_bool y)  noexcept { return strong_bool(x || y.value); }
      //friend constexpr strong_bool operator||(strong_bool x, bool y)  noexcept { return strong_bool(x.value || y); }

      friend constexpr strong_bool operator!(strong_bool x)  noexcept { return strong_bool(! x.value); }
      //!@}


      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strong_bool x, strong_bool y)  noexcept { return x.value == y.value; }
      //friend constexpr bool operator==(bool x, strong_bool y)  noexcept { return x == y.value; }
      //friend constexpr bool operator==(strong_bool x, bool y)  noexcept { return x.value == y; }

      friend constexpr bool operator!=(strong_bool x, strong_bool y)  noexcept { return x.value != y.value; }
      // fixme do we need order
      friend constexpr bool operator<(strong_bool x, strong_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bool x, strong_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bool x, strong_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bool x, strong_bool y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  template <class Tag>
  struct strong_bool<Tag, bool> final : protected_tagged<Tag, bool>
  {
      using base_type = protected_tagged<Tag, bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strong_bool() noexcept = default;

      // unwanted conversions
      constexpr explicit strong_bool (int) = delete;
      constexpr explicit strong_bool (double) = delete;
      constexpr explicit strong_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strong_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strong_bool (R(C::*)(Args...)) = delete;

      //!@{
      // boolean operators
      friend constexpr strong_bool operator&&(strong_bool x, strong_bool y)  noexcept { return strong_bool(x.value && y.value); }
      //friend constexpr strong_bool operator&&(bool x, strong_bool y)  noexcept { return strong_bool(x && y.value); }
      //friend constexpr strong_bool operator&&(strong_bool x, bool y)  noexcept { return strong_bool(x.value && y); }

      friend constexpr strong_bool operator||(strong_bool x, strong_bool y)  noexcept { return strong_bool(x.value || y.value); }
      //friend constexpr strong_bool operator||(bool x, strong_bool y)  noexcept { return strong_bool(x || y.value); }
      //friend constexpr strong_bool operator||(strong_bool x, bool y)  noexcept { return strong_bool(x.value || y); }

      friend constexpr strong_bool operator!(strong_bool x)  noexcept { return strong_bool(! x.value); }
      // todo do we need mixed boolean operators?
      //!@}


      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strong_bool x, strong_bool y)  noexcept { return x.value == y.value; }
      //friend constexpr bool operator==(bool x, strong_bool y)  noexcept { return x == y.value; }
      //friend constexpr bool operator==(strong_bool x, bool y)  noexcept { return x.value == y; }

      friend constexpr bool operator!=(strong_bool x, strong_bool y)  noexcept { return x.value != y.value; }
      // fixme do we need order
      friend constexpr bool operator<(strong_bool x, strong_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strong_bool x, strong_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strong_bool x, strong_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strong_bool x, strong_bool y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  static_assert(std::is_pod<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_bool<bool>>::value, "");
  static_assert(std::is_trivially_copyable<strong_bool<bool>>::value, "");
  static_assert(std::is_standard_layout<strong_bool<bool>>::value, "");
  static_assert(std::is_trivial<strong_bool<bool>>::value, "");

  //! underlying_type specialization for strong_bool
  template <class Tag, class Bool>
  struct underlying_type<strong_bool<Tag, Bool>> { using type = Bool; };

  // stream operators

  //! input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c strong_bool
  //! \n<b>Returns:</b><br> \c is.

  template <class CharT, class Traits, class Tag, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, strong_bool<Tag, T>& x)
  {
    T v;
    is >> v;
    x = strong_bool<Tag, T>(v);
    return is;
  }

  //! output function.
  //! @param os the output stream
  //! @param x the \c strong_bool
  //!
  //! \n<b>Returns:</b><br> the result of the following expression
  //! @code
  //! os << bool(x.undelying())
  //! @endcode

  template <class CharT, class Traits, class Tag, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strong_bool<Tag, T>& x)
  {
    return os << bool(x.underlying());
  }

#if __cplusplus >= 201402L
  namespace ordinal {
    template <class Tag, class T>
    struct traits<strong_bool<Tag, T>>
    {
      using size_type = size_t;
      using size = integral_constant<size_type, 2>;
      static strong_bool<Tag, T> val(size_type p) { return strong_bool<Tag, T>{static_cast<T>(p)}; }

      static size_type pos(strong_bool<Tag, T> u)  { return size_type{u.underlying()}; };
    };
  }
#endif

  // fixme: Should boolean be a strong bool without tag?
  using boolean = strong_bool<default_tag, bool>;
  using boolean8_t = strong_bool<default_tag, uint8_t>;
}
}

  template <class Tag, class UT>
  struct hash<experimental::strong_bool<Tag,UT>> :
    experimental::wrapped_hash<experimental::strong_bool<Tag,UT>> {};


}

#endif // header
