// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_TAGGED_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_TAGGED_HPP

#include <experimental/fundamental/v3/strong/wrapper.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
    struct default_tag {};

    // fixme: should the Tag parameter be the last so that we can have a default tag?
    //! tagged wraps an underlying type providing access to the underlying value with a specific @c Tag.
    //!
    //! The main goal of the tag is to make two tagged types with different tag different types.
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type

    template <class Tag, class UT>
    struct tagged
    : wrapper<UT>
    {
      using tag_type = Tag;
      using base_type = wrapper<UT>;
      using base_type::base_type;

      constexpr tagged() noexcept = default;
      constexpr tagged(tagged const& e) = default;
      constexpr tagged(tagged&& e) = default;
      JASEL_MUTABLE_CONSTEXPR tagged& operator=(tagged const& e) = default;
      JASEL_MUTABLE_CONSTEXPR tagged& operator=(tagged&& e) = default;


      template <class U, enable_if_t<
          is_constructible<UT, const U&>::value == true &&
          is_convertible<const U&, UT>::value == true &&
          is_constructible<UT, tagged<Tag, U>&>::value == false &&
          is_constructible<UT, tagged<Tag, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&&>::value == false &&
          is_convertible<tagged<Tag, U>&, UT>::value == false &&
          is_convertible<tagged<Tag, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr explicit tagged(const tagged<Tag, U>& other)
          : base_type(other.underlying()) {}

      template <class U, enable_if_t<
          is_constructible<UT, const U&>::value == true &&
          is_convertible<const U&, UT>::value == false &&
          is_constructible<UT, tagged<Tag, U>&>::value == false &&
          is_constructible<UT, tagged<Tag, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&&>::value == false &&
          is_convertible<tagged<Tag, U>&, UT>::value == false &&
          is_convertible<tagged<Tag, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr tagged(const tagged<Tag, U>& other)
          : base_type(other.underlying()) {}

      template <class Tag2, class U, enable_if_t<
          is_same<Tag, Tag2>::value == false &&
          is_constructible<UT, const U&>::value == true &&
          is_constructible<UT, tagged<Tag2, U>&>::value == false &&
          is_constructible<UT, tagged<Tag2, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag2, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag2, U>&&>::value == false &&
          is_convertible<tagged<Tag2, U>&, UT>::value == false &&
          is_convertible<tagged<Tag2, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag2, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag2, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr explicit tagged(const tagged<Tag2, U>& other) = delete;

      template <class U, enable_if_t<
          is_constructible<UT, U&&>::value == true &&
          is_convertible<U&&, UT>::value == true &&
          is_constructible<UT, tagged<Tag, U>&>::value == false &&
          is_constructible<UT, tagged<Tag, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&&>::value == false &&
          is_convertible<tagged<Tag, U>&, UT>::value == false &&
          is_convertible<tagged<Tag, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr explicit tagged(tagged<Tag, U>&& other)
          : base_type(std::move(other).underlying()) {}

      template <class U, enable_if_t<
          is_constructible<UT, U&&>::value == true &&
          is_convertible<U&&, UT>::value == false &&
          is_constructible<UT, tagged<Tag, U>&>::value == false &&
          is_constructible<UT, tagged<Tag, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag, U>&&>::value == false &&
          is_convertible<tagged<Tag, U>&, UT>::value == false &&
          is_convertible<tagged<Tag, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr tagged(tagged<Tag, U>&& other)
          : base_type(std::move(other).underlying()) {}

      template <class Tag2, class U, enable_if_t<
          is_same<Tag, Tag2>::value == false &&
          is_constructible<UT, U&&>::value == true &&
          is_constructible<UT, tagged<Tag2, U>&>::value == false &&
          is_constructible<UT, tagged<Tag2, U>&&>::value == false &&
          is_constructible<UT, const tagged<Tag2, U>&>::value == false &&
          is_constructible<UT, const tagged<Tag2, U>&&>::value == false &&
          is_convertible<tagged<Tag2, U>&, UT>::value == false &&
          is_convertible<tagged<Tag2, U>&&, UT>::value == false &&
          is_convertible<const tagged<Tag2, U>&, UT>::value == false &&
          is_convertible<const tagged<Tag2, U>&&, UT>::value == false
          , int> = 0
      >
          constexpr tagged(tagged<Tag2, U>&& other) = delete;


    };

}
}
}

#endif // header
