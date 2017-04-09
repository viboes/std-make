// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_TAGGED_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_TAGGED_HPP

#include <experimental/fundamental/v3/strong/underlying_type.hpp>
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
    };

    //! public_tagged is a tagged wrapper that provides implicit conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Tag, class UT>
    struct public_tagged
    : public_wrapper<UT>
    {
      using tag_type = Tag;
      using base_type = public_wrapper<UT>;
      using base_type::base_type;
    };

    //! protected_tagged is a tagged wrapper that provides explicit conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Tag, class UT>
    struct protected_tagged
    : protected_wrapper<UT>
    {
      using tag_type = Tag;
      using base_type = protected_wrapper<UT>;
      using base_type::base_type;
    };

    //! private_tagged is a tagged wrapper that provides no conversion to the underlying type
    //!
    //! @tparam Tag the tag type
    //! @tparam UT the underlying type
    template <class Tag, class UT>
    struct private_tagged
    : private_wrapper<UT>
    {
      using tag_type = Tag;
      using base_type = private_wrapper<UT>;
      using base_type::base_type;
    };

}
}
}

#endif // header
