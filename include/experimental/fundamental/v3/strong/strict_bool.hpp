// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRICT_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRICT_BOOL_HPP

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
  `strict_bool` is a strongly type that wraps a bool and behaves like an `bool`
  The main goal is to be able to define strong bools that don't mix between them.
  The single conversion is the explicit conversion to bool type.
  This is a safe bool, in the sense that it doesn't allows conversions from integral, floating point and pointer types.
  In addition it forbids conversions between different strong bools types.

  As any tagged type, it makes the parameter more explicit.
  Example
  <code>
  using X = strict_bool<XTag>;
  using Y = strict_bool<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
  // Do we need a Bool parameter? Which type will be a good candidate?
  // bool takes 4 bytes in some machines. Sometimes we want to use just 1 byte
  template <class Tag, class Bool = bool>
  struct strict_bool final : tagged<Tag, Bool>
  {
      using base_type = tagged<Tag, Bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strict_bool() noexcept = default;
      // If Bool is not bool, we want an explicit conversion from bool but not Bool.
      constexpr explicit strict_bool (Bool) = delete;
      constexpr explicit strict_bool (bool b) : base_type(b) {};

      // If Bool is not bool, we want an explicit conversion to bool
      constexpr explicit operator bool() const { return bool(this->value); }

      // unwanted conversions
      constexpr explicit strict_bool (int) = delete;
      constexpr explicit strict_bool (double) = delete;
      constexpr explicit strict_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strict_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strict_bool (R(C::*)(Args...)) = delete;

      //!@{
      // boolean operators
      // fixme do we need these && and || operators?
      // strict_bool is explicit convertible to bool and so && and || operator will force the conversion to bool
      // If we don't define them we need to be explicit to convert to strict_bool
      // If we define them, we need to be explicit to convert to bool.
      // todo do we need mixed boolean operators?
      // If yes, which would be the result. IMO, it should be bool.
      // If not, there will be a conversion to bool and the result is bool
      //friend constexpr strict_bool operator&&(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value && y.value); }
      //friend constexpr strict_bool operator&&(bool x, strict_bool y)  noexcept { return strict_bool(x && y.value); }
      //friend constexpr strict_bool operator&&(strict_bool x, bool y)  noexcept { return strict_bool(x.value && y); }

      //friend constexpr strict_bool operator||(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value || y.value); }
      //friend constexpr strict_bool operator||(bool x, strict_bool y)  noexcept { return strict_bool(x || y.value); }
      //friend constexpr strict_bool operator||(strict_bool x, bool y)  noexcept { return strict_bool(x.value || y); }

      //friend constexpr strict_bool operator!(strict_bool x)  noexcept { return strict_bool(! x.value); }
      //!@}


      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strict_bool x, strict_bool y)  noexcept { return x.value == y.value; }
      //friend constexpr bool operator==(bool x, strict_bool y)  noexcept { return x == y.value; }
      //friend constexpr bool operator==(strict_bool x, bool y)  noexcept { return x.value == y; }

      friend constexpr bool operator!=(strict_bool x, strict_bool y)  noexcept { return x.value != y.value; }
      // fixme do we need order
      friend constexpr bool operator<(strict_bool x, strict_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strict_bool x, strict_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strict_bool x, strict_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strict_bool x, strict_bool y)  noexcept { return x.value >= y.value; }
      //!@}
  };

  template <class Tag>
  struct strict_bool<Tag, bool> final : protected_tagged<Tag, bool>
  {
      using base_type = protected_tagged<Tag, bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      constexpr strict_bool() noexcept = default;

      // unwanted conversions
      constexpr explicit strict_bool (int) = delete;
      constexpr explicit strict_bool (double) = delete;
      constexpr explicit strict_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit strict_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit strict_bool (R(C::*)(Args...)) = delete;

      //!@{
      // boolean operators
      friend constexpr strict_bool operator&&(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value && y.value); }
      //friend constexpr strict_bool operator&&(bool x, strict_bool y)  noexcept { return strict_bool(x && y.value); }
      //friend constexpr strict_bool operator&&(strict_bool x, bool y)  noexcept { return strict_bool(x.value && y); }

      friend constexpr strict_bool operator||(strict_bool x, strict_bool y)  noexcept { return strict_bool(x.value || y.value); }
      //friend constexpr strict_bool operator||(bool x, strict_bool y)  noexcept { return strict_bool(x || y.value); }
      //friend constexpr strict_bool operator||(strict_bool x, bool y)  noexcept { return strict_bool(x.value || y); }

      friend constexpr strict_bool operator!(strict_bool x)  noexcept { return strict_bool(! x.value); }
      // todo do we need mixed boolean operators?
      //!@}


      //!@{
      //! relational operators
      //!
      //! Forwards to the underlying value
      friend constexpr bool operator==(strict_bool x, strict_bool y)  noexcept { return x.value == y.value; }
      //friend constexpr bool operator==(bool x, strict_bool y)  noexcept { return x == y.value; }
      //friend constexpr bool operator==(strict_bool x, bool y)  noexcept { return x.value == y; }

      friend constexpr bool operator!=(strict_bool x, strict_bool y)  noexcept { return x.value != y.value; }
      // fixme do we need order
      friend constexpr bool operator<(strict_bool x, strict_bool y)  noexcept { return x.value < y.value; }
      friend constexpr bool operator>(strict_bool x, strict_bool y)  noexcept { return x.value > y.value; }
      friend constexpr bool operator<=(strict_bool x, strict_bool y)  noexcept { return x.value <= y.value; }
      friend constexpr bool operator>=(strict_bool x, strict_bool y)  noexcept { return x.value >= y.value; }
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
  //! \n<b>Returns:</b><br> \c is.

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
  //! \n<b>Returns:</b><br> the result of the following expression
  //! @code
  //! os << bool(x.undelying())
  //! @endcode

  template <class CharT, class Traits, class Tag, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strict_bool<Tag, T>& x)
  {
    return os << bool(x.underlying());
  }

  namespace ordinal {
    template <class Tag, class T>
    struct traits<strict_bool<Tag, T>>
    {
      using size_type = size_t;
      using size = integral_constant<size_type, 2>;
      static strict_bool<Tag, T> val(size_type p) { return strict_bool<Tag, T>{static_cast<T>(p)}; }

      static size_type pos(strict_bool<Tag, T> u)  { return size_type{u.underlying()}; };
    };
  }

#if 0
  // fixme: Should boolean be a strong bool without tag?
  using boolean = strict_bool<default_tag, bool>;
  using boolean8_t = strict_bool<default_tag, uint8_t>;
#endif
}
}

  template <class Tag, class UT>
  struct hash<experimental::strict_bool<Tag,UT>> :
    experimental::wrapped_hash<experimental::strict_bool<Tag,UT>> {};


}

#endif // header
