// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP

#include <experimental/fundamental/v3/strong/underlying_type.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{

    //! wrapper wraps an underlying type providing access to the underlying value via the underlying function
    //!
    //! @tparam UT the underlying type

    template <class UT>
    struct wrapper
    {
      //! the underlying type reflected type
      using underlying_t = UT;

      constexpr wrapper() noexcept = default;

      //! explicit conversion from the underlying type
      explicit constexpr wrapper(underlying_t v): value(v) {}

      //! underlying value access
      constexpr underlying_t underlying() const noexcept
      { return value; }

    protected:
      //! the wrapped value
      underlying_t value;
    };

    //! underlying_type specialization for wrapper
    template <class UT>
    struct underlying_type<wrapper<UT>>
    { typedef UT type; };

    //! public_wrapper is a wrapper that provides implicit conversion to the underlying type
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct public_wrapper
    : wrapper<UT>
    {
      using base_type = wrapper<UT>;
      using typename base_type::underlying_t;
      using base_type::base_type;
      using base_type::underlying;

      constexpr operator UT () const noexcept
      { return this->value;}
    };

    //! protected_wrapper is a wrapper that provides explicit conversion to the underlying type
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct protected_wrapper
    : wrapper<UT>
    {
      using base_type = wrapper<UT>;
      using typename base_type::underlying_t;
      using base_type::base_type;
      using base_type::underlying;

      explicit constexpr operator UT () const noexcept
      { return this->value;}
    };

    //! private_wrapper is a wrapper that provides no conversion to the underlying type
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct private_wrapper
    : wrapper<UT>
    {
      using base_type = wrapper<UT>;
      using base_type::wrapper;
      using base_type::underlying;

      operator UT () const = delete;
    };

}
}
}

#endif // header
