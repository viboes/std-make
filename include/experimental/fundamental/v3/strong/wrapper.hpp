// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP

#include <experimental/ordinal.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/wrapped.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
    struct wrapper_base {};
    //! wrapper wraps an underlying type providing access to the underlying value via the underlying function
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct wrapper : wrapper_base
    {
      //! the underlying type reflected type
      using underlying_type = UT;

      constexpr wrapper() noexcept = default;

      //! explicit conversion from the underlying type
      //! @par Effects Constructs a wrapper from its underlying type
      explicit constexpr wrapper(underlying_type v): _value(v) {}

      //! underlying value access
      //! @par Returns the underlying value
      constexpr underlying_type underlying() const noexcept
      { return _value; }


    protected:
      //! the wrapped value
      underlying_type _value;
    };

    namespace wrapping {
    template <typename T>
    struct traits<T, meta::when<is_base_of<wrapper_base, T>::value>>
    {
        template <class U>
        static
        auto underlying(U && u)
        JASEL_DECLTYPE_RETURN_NOEXCEPT (
                        forward<U>(u).underlying()
        )
    };
    };

    //! public_wrapper is a wrapper that provides implicit conversion to the underlying type
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct public_wrapper
    : wrapper<UT>
    {
      using base_type = wrapper<UT>;
      using typename base_type::underlying_type;
      using base_type::base_type;
      using base_type::underlying;

      //! @par Returns the underlying value
      constexpr operator UT () const noexcept
      { return this->_value;}
    };

    //! protected_wrapper is a wrapper that provides explicit conversion to the underlying type
    //!
    //! @tparam UT the underlying type
    template <class UT>
    struct protected_wrapper
    : wrapper<UT>
    {
      using base_type = wrapper<UT>;
      using typename base_type::underlying_type;
      using base_type::base_type;
      using base_type::underlying;

      //! @par Returns the underlying value
      explicit constexpr operator UT () const noexcept
      { return this->_value;}
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
