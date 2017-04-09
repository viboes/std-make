// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/optional.hpp>
#include <exception>
#include <functional>
#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // By default it should checks if x is in the range of the underlying type
  // This function must be specialized for C++98 enums which have a more specific range
  template <class E, class Int>
  constexpr bool is_valid_enum(Int x) { return true; }

  // This function must be specialized for each enum
  template <class E>
  constexpr bool is_enumerator(E x) { return false; }

  template <class E, class Int>
  constexpr bool is_valid_enumerator(Int x) { return is_valid_enum<E>(x) && is_enumerator(E(x)); }


  template <class E, class Int>
  constexpr E enum_cast(Int x) { return static_cast<E>(x); }

  struct bad_enum_cast : std::logic_error {
      bad_enum_cast() : std::logic_error("Bad enum cast") {}
  };

  template <class E, class Int>
  constexpr E to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast(); }

  template <class E, class Int>
  constexpr E to_valid_enum_or(Int x, E other) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return other; }

  template <class E, class Int>
  constexpr E to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast(); }

  template <class E, class Int>
  constexpr E to_enumerator_or(Int x, E other) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return other; }

  template <class E, class Int>
  constexpr optional<E> try_to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return nullopt; }

  template <class E, class Int>
  constexpr optional<E> try_to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return nullopt; }

  // This template must be specialized for enum wrappers

  template <class E>
  typename enable_if<is_enum<E>::value, E>::type to_enum(E v)
  { return v; }

  template <class T>
  auto to_enum(T v) -> decltype(v.to_enum())
  { return v.to_enum(); }

  template <class E>
  struct native_enum_type
  {
    using type = decltype(to_enum(declval<E>()));
  };

  template <class E>
  using native_enum_type_t = typename native_enum_type<E>::type;

  template <class E, class UT=typename underlying_type<E>::type>
  struct enum_wrapper : private_wrapper<UT>
  {
      using base_type = private_wrapper<UT>;
      using base_type::base_type;

      typedef E enum_type;

      constexpr enum_wrapper() noexcept = default;
      constexpr enum_wrapper(enum_type v) noexcept : base_type(UT(v)) {}

      enum_type to_enum() const noexcept { return enum_type(this->value); }
      explicit operator enum_type() const noexcept { return enum_type(this->value); }

  };

  //! strong_enum ensures that static_cast is not allowed between two strong_enums
  //! Note that static_cast of two C++11 enum class is allowed as they are integral types.
  //! strong_enum behaves almost as a C++11 enum class E: UT
  //! However there are some syntactical issues.
  //! In C++98, if you want scoped enums you should wrap the enum in a struct
  //! struct SE {
  //!     enum type { E0, E1, E2 };
  //! };
  //! typedef strong_enum<SE::type, uint8_t> SSE

  template <class E, class UT=typename underlying_type<E>::type>
  struct strong_enum final : enum_wrapper<E, UT>
  {
      using base_type = enum_wrapper<E, UT>;
      using base_type::enum_wrapper;

      friend constexpr bool operator==(strong_enum x, strong_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_enum x, strong_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_enum x, strong_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(strong_enum x, strong_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(strong_enum x, strong_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(strong_enum x, strong_enum y) noexcept { return x.value >= y.value; }

  };

  // stream operators

  //! strong_enum input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c strong_enum
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class E, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, strong_enum<E, T>& x)
  {
    T v;
    is >> v;
    x = strong_enum<E, T>(v);
    return is;
  }

  //! strong_enum output function.
  //! @param os the output stream
  //! @param x the \c strong_enum
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << bool(x.undelying())
  //! @endcode

  template <class CharT, class Traits, class E, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const strong_enum<E, T>& x)
  {
    return os << x.underlying();
  }

  template <class E, class UT>
  struct underlying_type<strong_enum<E,UT>> { typedef UT type; };

  // safe_enum is a strong_enum that checks the validity of the values of the enum using is_valid_enum
  template <class E, class UT=typename underlying_type<E>::type>
  struct safe_enum final: enum_wrapper<E,UT>
  {
      using base_type = enum_wrapper<E,UT>;
      using base_type::base_type;
      using underlying_t = typename base_type::underlying_t;

      // safe_enum is not safe if the default value 0 is not one of the enumerators
      // the question is, should the class invariant be valid for uninitialized instances.
      // This has relation of applicability of invariant of moved objects.
      constexpr safe_enum() noexcept = default;
      explicit safe_enum(underlying_t v): base_type(UT(to_valid_enum<E>(v))) {}

      friend constexpr bool operator==(safe_enum x, safe_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(safe_enum x, safe_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(safe_enum x, safe_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(safe_enum x, safe_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(safe_enum x, safe_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(safe_enum x, safe_enum y) noexcept { return x.value >= y.value; }

  };

  // stream operators

  //! safe_enum input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c safe_enum
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class E, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, safe_enum<E, T>& x)
  {
    T v;
    is >> v;
    x = safe_enum<E, T>(v);
    return is;
  }

  //! safe_enum output function.
  //! @param os the output stream
  //! @param x the \c safe_enum
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << bool(x.undelying())
  //! @endcode

  template <class CharT, class Traits, class E, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const safe_enum<E, T>& x)
  {
    return os << int(x.to_enum());
  }

  template <class E, class UT>
  struct underlying_type<safe_enum<E,UT>> { typedef UT type; };

  // ordinal_enum is a strong enum that checks the validity of the values of the enum using is_enumerator
  // is_enumerator is specialized for ordinal enums
  template <class E, class UT=typename underlying_type<E>::type>
  struct ordinal_enum final: enum_wrapper<E,UT>
  {
      using base_type = enum_wrapper<E,UT>;
      using base_type::base_type;
      using underlying_t = typename  base_type::underlying_t;

      // ordinal_enum is not ordinal if the default value 0 is not one of the enumerators
      // the question is, should the class invariant be valid for uninitialized instances.
      // This has relation of applicability of invariant of moved objects.
      constexpr ordinal_enum() noexcept = default;
      explicit ordinal_enum(underlying_t v): base_type(UT(to_enumerator<E>(v))) {}

      friend constexpr bool operator==(ordinal_enum x, ordinal_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(ordinal_enum x, ordinal_enum y) noexcept { return x.value != y.value; }

      // Note that ordinal_enum order is based on the underlying type not the position
      friend constexpr bool operator<(ordinal_enum x, ordinal_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(ordinal_enum x, ordinal_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(ordinal_enum x, ordinal_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(ordinal_enum x, ordinal_enum y) noexcept { return x.value >= y.value; }

  };

  // stream operators
  // fixme enums don't define the stream operators
  //! ordinal_enum input function.
  //! @par Effects:<br> Extracts a T from is and stores it in the passes x.
  //! @param is the input stream
  //! @param x the \c safe_enum
  //! @par Returns:<br> \c is.

  template <class CharT, class Traits, class E, class T >
  std::basic_istream<CharT, Traits>&
  operator>>(std::basic_istream<CharT, Traits>& is, ordinal_enum<E, T>& x)
  {
    T v;
    is >> v;
    x = ordinal_enum<E, T>(v);
    return is;
  }

  //! ordinal_enum output function.
  //! @param os the output stream
  //! @param x the \c ordinal_enum
  //!
  //! @par Returns:<br> the result of the following expression
  //! @code
  //! os << +x.undelying()
  //! @endcode

  template <class CharT, class Traits, class E, class T >
  std::basic_ostream<CharT, Traits>&
  operator<<(std::basic_ostream<CharT, Traits>& os, const ordinal_enum<E, T>& x)
  {
    return os << +x.to_undelying();
  }

  template <class E, class UT>
  struct underlying_type<ordinal_enum<E,UT>> { typedef UT type; };


}
}

  template <class E, class UT>
  struct hash<experimental::strong_enum<E,UT>>
    : experimental::wrapped_hash<experimental::strong_enum<E,UT>> {};
  template <class E, class UT>
  struct hash<experimental::safe_enum<E,UT>>
    : experimental::wrapped_hash<experimental::safe_enum<E,UT>> {};
  template <class E, class UT>
  struct hash<experimental::ordinal_enum<E,UT>>
    : experimental::wrapped_hash<experimental::ordinal_enum<E,UT>> {};
}

#endif
#endif // header
