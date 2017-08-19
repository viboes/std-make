// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_ID_HPP

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/hashable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/fundamental/v3/strong/mixins/ordinal.hpp>
#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>

#include <type_traits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  /**
  `strong_id` is a strongly type that wraps a regular type and behaves like a
  `Regular` type, comparable, streamable and `Ordinal` type, able to enumerate.
  The main goal is to be able to define strong identifiers that don't mix between them.
  No conversion to the underlying type is provided as a strong_id is not an underlying type,
  however you can always have access to it using the underlying fuction.

  Example
  <code>
  using X = strong_id<XTag>;
  using Y = strong_id<YTag>;
  void f(X, Y);

  f(X(true), Y(false));

  </code>
  */
#if 0
  template <class Tag, class UT = int>
  struct strong_id final
    : strong_type<strong_id<Tag, UT>, UT>
    , mixin::comparable<strong_id<Tag, UT>>
    , mixin::hashable<strong_id<Tag, UT>>
    , mixin::streamable<strong_id<Tag, UT>>
    , mixin::ordinal<strong_id<Tag, UT>>
  {
      using base_type = strong_type<strong_id<Tag, UT>, UT>;
      using tag_type = Tag;
      using underlying_t = UT;

      using base_type::base_type;
  };
#else
    template <class Tag, class UT = int>
    struct strong_id
      : new_class<strong_id<Tag, UT>, UT
      , meta_mixin::comparable
      , meta_mixin::hashable
      , meta_mixin::streamable
      , meta_mixin::ordinal
      >
    {
        using base_type = new_class<strong_id<Tag, UT>, UT
            , meta_mixin::comparable
            , meta_mixin::hashable
            , meta_mixin::streamable
            , meta_mixin::ordinal
            >;
        using base_type::base_type;
    };
#endif
  static_assert(std::is_pod<strong_id<int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_id<int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_id<int>>::value, "");
  static_assert(std::is_standard_layout<strong_id<int>>::value, "");
  static_assert(std::is_trivial<strong_id<int>>::value, "");

  //! underlying_type specialization for strong_id
  template <class Tag, class UT>
  struct underlying_type<strong_id<Tag, UT>> { using type = UT; };

}
}

  /// Hash specialization forwarding to the hash of underlying type
  template <class Tag, class UT>
  struct hash<experimental::strong_id<Tag, UT>
        //, experimental::meta::when<is_hashable<UT>::value>
      > : experimental::wrapped_hash<experimental::strong_id<Tag, UT>> {};

}

#endif // header
