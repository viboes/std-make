// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/hashable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/optional.hpp>
#include <experimental/type_traits.hpp>
#include <exception>
#include <functional>

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
  JASEL_CXX14_CONSTEXPR E to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast(); }

  template <class E, class Int>
  JASEL_CXX14_CONSTEXPR E to_valid_enum_or(Int x, E other) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return other; }

  template <class E, class Int>
  JASEL_CXX14_CONSTEXPR E to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast(); }

  template <class E, class Int>
  JASEL_CXX14_CONSTEXPR E to_enumerator_or(Int x, E other) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return other; }

  template <class E, class Int>
  JASEL_CXX14_CONSTEXPR optional<E> try_to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return nullopt; }

  template <class E, class Int>
  JASEL_CXX14_CONSTEXPR optional<E> try_to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return nullopt; }

  // This template must be specialized for enum wrappers

  template <class E>
  enable_if_t<is_enum<E>::value, E> to_enum(E v)
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

  template <class Final, class E, class UT=underlying_type_t<E>>
  struct enum_wrapper
      : strong_type<Final, UT>
      , mixin::comparable<Final>
      , mixin::hashable<Final>
      , mixin::streamable<Final>
  {
      using base_type = strong_type<Final, UT>;
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

  template <class E, class UT=underlying_type_t<E>>
  struct strong_enum final : enum_wrapper<strong_enum<E, UT>, E, UT>
  {
      using base_type = enum_wrapper<strong_enum<E, UT>, E, UT>;
      using base_type::base_type;

  };

  // safe_enum is a strong_enum that checks the validity of the values of the enum using is_valid_enum
  template <class E, class UT=underlying_type_t<E>>
  struct safe_enum final: enum_wrapper<safe_enum<E, UT>, E, UT>
  {
      using base_type = enum_wrapper<safe_enum<E, UT>, E, UT>;
      using base_type::base_type;
      using underlying_t = typename base_type::underlying_t;

      // safe_enum is not safe if the default value 0 is not one of the enumerators
      // the question is, should the class invariant be valid for uninitialized instances.
      // This has relation of applicability of invariant of moved objects.
      constexpr safe_enum() noexcept = default;
      explicit safe_enum(underlying_t v): base_type(UT(to_valid_enum<E>(v))) {}

  };

//  template <class E, class UT>
//  struct underlying_type<safe_enum<E,UT>> { typedef UT type; };

  // ordinal_enum is a strong enum that checks the validity of the values of the enum using is_enumerator
  // is_enumerator is specialized for ordinal enums
  template <class E, class UT=underlying_type<E>>
  struct ordinal_enum final: enum_wrapper<ordinal_enum<E, UT>, E, UT>
  {
      using base_type = enum_wrapper<ordinal_enum<E, UT>, E, UT>;
      using base_type::base_type;
      using underlying_t = typename  base_type::underlying_t;

      // ordinal_enum is not ordinal if the default value 0 is not one of the enumerators
      // the question is, should the class invariant be valid for uninitialized instances.
      // This has relation of applicability of invariant of moved objects.
      constexpr ordinal_enum() noexcept = default;
      explicit ordinal_enum(underlying_t v): base_type(UT(to_enumerator<E>(v))) {}

      // Note that ordinal_enum order is based on the underlying type not the position

  };

//  template <class E, class UT>
//  struct underlying_type<ordinal_enum<E,UT>> { typedef UT type; };

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

#endif // header
