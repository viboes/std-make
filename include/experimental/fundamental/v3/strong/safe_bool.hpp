// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_SAFE_BOOL_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_SAFE_BOOL_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/convertible.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/hashable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/fundamental/v3/strong/mixins/ordinal.hpp>

#include <type_traits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <class Bool = bool>
  struct safe_bool;

  namespace mixin {
    template <class B1, class B2>
    struct is_compatible_with<safe_bool<B1>, safe_bool<B2>> : true_type {};
  }
  // Q/A
  // Do we need a Bool parameter? Which type will be a good candidate?
  // bool takes 4 bytes in some machines. Sometimes we want to use just 1 byte
  //
  // Should safe_bool be implicitly convertible from bool?
  // It is better if strong types are only explicitly convertible from its underlying type, so that we avoid overload ambiguity.
  // However, requiring an explicit conversion make the code cumbersome.
  // Even if safe_bool uses strong_type, it is not tagged. strong_bool is a tagged bool that don't provide such a implicit conversion.
  //
  // Should this class convert implicitly or explicitly to bool?
  // A strong bool is a bool, so we could convert it implicitly.
  // However bool converts with other types we don't want safe_bool to work with, e.g. we don't want a safe_bool to be added to an int.
  //
  // Do we need the logical operators ?
  // safe_bool is (explicit) convertible to bool and so && and || operator and ?: operator will force the conversion to bool
  // If we don't define them the result is a bool and as safe_bool is implicitly convertible from bool, we don't need to explicit convert it to safe_bool.
  // However, that means that !boolean + int-> int :(
  // If we define them, the result should be safe_bool, as the explicit conversion to bool already result in bool, we would need to explicitly to convert to bool these boolean expressions.
  // bool b = bool(!boolean);
  // This could be considered cumbersome
  // For the time being, we pretend that logical operation on a boolean scape the safety rules.
  // If we wanted this to be safer, we would need an additional type that makes the difference.
  //
  // Do we need mixed boolean operators?
  // If yes, which would be the result. IMO, it should be bool.
  // If not, there will be a conversion to bool and the result is bool
  // So we don't need them
  //
  // fixme What should be the result of comparing two safe_bool? bool or safe_bool?
  // For the time being, these functions return bool.
  //
  // Should safe_bool be ordered?
  //  Independently of the bool int conversion, a boolean is ordered (false < true)
  //
  // Should safe_bool default construct to false?
  //    If we add a default constructor safe_bool will not be trivial anymore, and couldn't be used in physical structs.
  //
  // Should safe_bool<B1> and safe_bool<B1> be convertible?
  //    The single difference is the representation. A safe_bool has only two values and so it should be implicitly convertible

  /// A safe bool is a boolean type that stores a representation Bool
  /// convertible *implicitly* from bool
  /// convertible *explicitly* to bool
  /// Not convertible implicitly from int, double, pointers.
  /// It is comparable, streamable and hashable
  /// It provides the boolean operations indirectly via the explicit conversion.

  template <class Bool>
  struct safe_bool final
     : strong_type<safe_bool<Bool>, Bool>
     , mixin::comparable_with_if<safe_bool<Bool>>
     , mixin::explicit_convertible_to<safe_bool<Bool>, bool>
     , mixin::hashable<safe_bool<Bool>>
     , mixin::streamable<safe_bool<Bool>>
  {
      using base_type = strong_type<safe_bool<Bool>, Bool>;
      using base_type::base_type;

      static_assert(is_integral<Bool>::value, "The representation must be an integral type");
      // copy constructor/assignment default
      constexpr safe_bool() noexcept = default;
      // Implicitly convertible from bool
      // maybe we need a template here and only_when<B, is_same> so that we other implicit conversions
      constexpr safe_bool (bool b) : base_type(b) {}

      constexpr safe_bool (safe_bool const& b) noexcept = default;
      template <class B>
      constexpr safe_bool (safe_bool<B> b) noexcept
        : base_type(b.underlying()) {}

      JASEL_CXX14_CONSTEXPR safe_bool& operator=(safe_bool const& b) noexcept = default;
      template <class B>
      JASEL_CXX14_CONSTEXPR safe_bool& operator=(safe_bool<B> b) noexcept
      {
        this->value = b.underlying();
        return *this;
      }

      // unwanted conversions
      constexpr explicit safe_bool (Bool) = delete;
      constexpr explicit safe_bool (int) = delete;
      constexpr explicit safe_bool (double) = delete;
      constexpr explicit safe_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit safe_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit safe_bool (R(C::*)(Args...)) = delete;

  };
  template <>
  struct safe_bool<bool> final
  : strong_type<safe_bool<bool>, bool>
  , mixin::comparable<safe_bool<bool>>
  , mixin::explicit_convertible_to<safe_bool<bool>, bool>
  , mixin::hashable<safe_bool<bool>>
  , mixin::streamable<safe_bool<bool>>
  , mixin::ordinal<safe_bool<bool>>
  {
      using base_type = strong_type<safe_bool<bool>, bool>;
      using base_type::base_type;

      // copy constructor/assignment default
      safe_bool() noexcept = default;
      constexpr safe_bool (bool b) : base_type(b) {}

      // unwanted conversions
      constexpr explicit safe_bool (int) = delete;
      constexpr explicit safe_bool (double) = delete;
      constexpr explicit safe_bool (void*) = delete;
      template <class R, class ... Args>
      constexpr explicit safe_bool (R(*)(Args...)) = delete;
      template <class C, class R, class ... Args>
      constexpr explicit safe_bool (R(C::*)(Args...)) = delete;

      constexpr safe_bool (safe_bool const& b) noexcept = default;
      template <class B>
      constexpr safe_bool (safe_bool<B> b) noexcept
        : base_type(b.underlying()) {}

      JASEL_CXX14_CONSTEXPR safe_bool& operator=(safe_bool const& b) noexcept = default;
      template <class B>
      JASEL_CXX14_CONSTEXPR safe_bool& operator=(safe_bool<B> b) noexcept
      {
        this->value = b.underlying();
        return *this;
      }

  };

  static_assert(std::is_pod<safe_bool<bool>>::value, "");
  static_assert(std::is_trivially_default_constructible<safe_bool<bool>>::value, "");
  static_assert(std::is_trivially_copyable<safe_bool<bool>>::value, "");
  static_assert(std::is_standard_layout<safe_bool<bool>>::value, "");
  static_assert(std::is_trivial<safe_bool<bool>>::value, "");

  //! underlying_type specialization for strong_bool
  template <class Bool>
  struct underlying_type<safe_bool<Bool>> { using type = Bool; };

  //! alias of safe_bool using the builtin bool
  using boolean = safe_bool<bool>;
  //! alias of safe_bool using 1 byte
  using boolean8_t = safe_bool<uint8_t>;
}
}

  //! Hash specialization forward to underlying type
  template <class UT>
  struct hash<experimental::safe_bool<UT>> :
    experimental::wrapped_hash<experimental::safe_bool<UT>> {};


}

#endif // header
