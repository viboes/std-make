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

    struct default_initialized_t
    {};
    struct uninitialized_t
    {};
    struct zero_initialized_t
    {};

    //! wrapper wraps an underlying type providing access to the underlying value
    //!
    //! @tparam UT the underlying type
    //! @tparam Default the default constructor policy, but default default initialized

    template <class UT, class Default = default_initialized_t>
    struct wrapper
    {
      //! the underlying type reflected type
      using underlying_t = UT;

      constexpr wrapper() noexcept : wrapper(Default{}) {}

      //! default initialize the underlying value
      explicit constexpr wrapper(default_initialized_t) noexcept : value() {}
      //! zero initialize the underlying value
      explicit constexpr wrapper(zero_initialized_t) noexcept : value{} {}
      //! not initialize the underlying value
      explicit constexpr wrapper(uninitialized_t) noexcept {}

      //! explicit conversion from the underlying type
      explicit constexpr wrapper(underlying_t v): value(v) {}

      //! underlying value access
#if __cplusplus >= 201402L
      constexpr
#endif
      underlying_t& underlying() noexcept
      { return value; }
      constexpr underlying_t const& underlying() const noexcept
      { return value; }

    protected:
      underlying_t value;
    };

    template <class UT, class D>
    struct underlying_type<wrapper<UT, D>>
    { typedef UT type; };

    //! public_wrapper is a wrapper that provides implicit conversion to the underlying type
    template <class UT, class Default = default_initialized_t>
    struct public_wrapper
    : wrapper<UT, Default>
    {
      using base_type = wrapper<UT, Default>;
      using typename base_type::underlying_t;
      using base_type::wrapper;
      using base_type::underlying;

      constexpr operator UT&() noexcept
      { return underlying();}
      constexpr operator UT const&() const noexcept
      { return underlying();}
    };

    //! protected_wrapper is a wrapper that provides explicit conversion to the underlying type
    template <class UT, class Default = default_initialized_t>
    struct protected_wrapper
    : wrapper<UT, Default>
    {
      using base_type = wrapper<UT, Default>;
      using typename base_type::underlying_t;
      using base_type::wrapper;
      using base_type::underlying;

      explicit constexpr operator UT&() noexcept
      { return underlying();}
      explicit constexpr operator UT const&() const noexcept
      { return underlying();}
    };

    //! private_wrapper is a wrapper that provides no conversion to the underlying type
    template <class UT, class Default = default_initialized_t>
    struct private_wrapper
    : wrapper<UT, Default>
    {
      using base_type = wrapper<UT, Default>;
      using base_type::wrapper;
      using base_type::underlying;

      operator UT&() = delete;
      operator UT const&() const = delete;
    };

}
}
}

#endif // header
