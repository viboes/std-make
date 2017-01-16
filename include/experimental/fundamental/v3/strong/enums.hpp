// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ENUMS_HPP

#include <experimental/fundamental/v3/strong/wrapper.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/optional.hpp>
#include <exception>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // By default it should checks if x is in the range of the underlying type
  // This function must be specialized for C++98 enums wich have a more specific range
  template <class E, class Int>
  constexpr bool is_valid_enum(Int x) {return true;}

  // This function must be specialized for each enum
  template <class E>
  constexpr bool is_enumerator(E x) {return false;}

  template <class E, class Int>
  constexpr bool is_valid_enumerator(Int x) {return is_valid_enum<E>(x) && is_enumerator(E(x));}


  template <class E, class Int>
  constexpr E enum_cast(Int x) {return static_cast<E>(x);}

  struct bad_enum_cast : std::logic_error {
      bad_enum_cast() : std::logic_error("Bad enum cast") {}
  };

  template <class E, class Int>
  constexpr E to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast();}

  template <class E, class Int>
  constexpr E to_valid_enum_or(Int x, E other) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return other;}

  template <class E, class Int>
  constexpr E to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else throw bad_enum_cast();}

  template <class E, class Int>
  constexpr E to_enumerator_or(Int x, E other) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return other;}

  template <class E, class Int>
  constexpr optional<E> try_to_enumerator(Int x) { if (is_valid_enumerator<E>(x)) return enum_cast<E>(x); else return nullopt;}

  template <class E, class Int>
  constexpr optional<E> try_to_valid_enum(Int x) { if (is_valid_enum<E>(x)) return enum_cast<E>(x); else return nullopt;}

  // strong_enum ensures that there is no allowed cast between two strong_enums
  // It behaves almost as an C++11 enum class: ut. However there are some syntactical issues.
  // In C++98, if you want scoped enums you should wrap the enum in a struct
  // struct SE {
  //     enum type { E0, E1, E2 };
  // };
  // Note that two C++11 enum class can be casted as they are integral types.



  template <class E, class UT=typename underlying_type<E>::type, class Default = default_initialized_t>
  struct strong_enum : protected_wrapper<UT, Default>
  {
      using base_type = protected_wrapper<UT, Default>;
      using base_type::base_type;

      typedef E enum_type;

      // fixme: Why do we need this default constructor
      constexpr strong_enum() noexcept : base_type() {}
      constexpr strong_enum(enum_type v) noexcept : strong_enum(UT(v)) {}

      enum_type enum_value() const { return enum_type(this->value); }
      enum_type get() const { return enum_type(this->value); }
      explicit operator enum_type() const { return enum_type(this->value); }

      friend constexpr bool operator==(strong_enum x, strong_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(strong_enum x, strong_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(strong_enum x, strong_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(strong_enum x, strong_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(strong_enum x, strong_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(strong_enum x, strong_enum y) noexcept { return x.value >= y.value; }

  };

  template <class E, class UT, class D>
  struct underlying_type<strong_enum<E,UT,D>> { typedef UT type; };

  // safe_enum is a strong_enum that checks the validity of the values of the enum using is_valid_enum
  template <class E, class UT=typename underlying_type<E>::type, class Default = default_initialized_t>
  struct safe_enum final: strong_enum<E,UT, Default>
  {
      using base_type = strong_enum<E,UT, Default>;
      using base_type::base_type;
      using underlying_t = typename base_type::underlying_t;

      // fixme: Why do we need this default constructor
      constexpr safe_enum() noexcept : base_type() {}
      explicit safe_enum(underlying_t v): base_type(to_valid_enum<E>(v)) {}

      friend constexpr bool operator==(safe_enum x, safe_enum y) noexcept { return x.value == y.value; }
      friend constexpr bool operator!=(safe_enum x, safe_enum y) noexcept { return x.value != y.value; }
      friend constexpr bool operator<(safe_enum x, safe_enum y) noexcept { return x.value < y.value; }
      friend constexpr bool operator<=(safe_enum x, safe_enum y) noexcept { return x.value <= y.value; }
      friend constexpr bool operator>(safe_enum x, safe_enum y) noexcept { return x.value > y.value; }
      friend constexpr bool operator>=(safe_enum x, safe_enum y) noexcept { return x.value >= y.value; }

  };

  template <class E, class UT, class D>
  struct underlying_type<safe_enum<E,UT,D>> { typedef UT type; };

  // ordinal_enum is a strong_enum that checks the validity of the values of the enum using is_enumerator
  // is_enumerator is specialized for ordinal enums
  template <class E, class UT=typename underlying_type<E>::type, class Default = default_initialized_t>
  struct ordinal_enum final: strong_enum<E,UT, Default>
  {
      using base_type = strong_enum<E,UT, Default>;
      using base_type::base_type;
      using underlying_t = typename  base_type::underlying_t;

    explicit ordinal_enum(underlying_t v): base_type(to_enumerator<E>(v)) {}

    friend constexpr bool operator==(ordinal_enum x, ordinal_enum y) noexcept { return x.value == y.value; }
    friend constexpr bool operator!=(ordinal_enum x, ordinal_enum y) noexcept { return x.value != y.value; }
    friend constexpr bool operator<(ordinal_enum x, ordinal_enum y) noexcept { return x.value < y.value; }
    friend constexpr bool operator<=(ordinal_enum x, ordinal_enum y) noexcept { return x.value <= y.value; }
    friend constexpr bool operator>(ordinal_enum x, ordinal_enum y) noexcept { return x.value > y.value; }
    friend constexpr bool operator>=(ordinal_enum x, ordinal_enum y) noexcept { return x.value >= y.value; }

  };
  template <class E, class UT, class D>
  struct underlying_type<ordinal_enum<E,UT,D>> { typedef UT type; };


}
}
}

#endif // header
