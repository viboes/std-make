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

    template <class UT, class Default = default_initialized_t>
    struct wrapper
    {
      using underlying_t = UT;

      constexpr wrapper() noexcept : wrapper(Default{}) {}
      explicit constexpr wrapper(default_initialized_t) noexcept : value() {}
      explicit constexpr wrapper(zero_initialized_t) noexcept : value{} {}
      explicit constexpr wrapper(uninitialized_t) noexcept {}

      explicit constexpr wrapper(underlying_t v): value(v) {}

#if __cplusplus >= 201402L
      constexpr
#endif
      underlying_t& underlying() noexcept
      { return value; }
      constexpr underlying_t const& underlying() const noexcept
      { return value; }

      // relational operators
      friend constexpr bool operator==(wrapper const& x, wrapper const& y) noexcept
      { return x.value == y.value; }
      friend constexpr bool operator!=(wrapper const& x, wrapper const& y) noexcept
      { return x.value != y.value; }

    protected:
      underlying_t value;
    };

    template <class UT, class D>
    struct underlying_type<wrapper<UT, D>>
    { typedef UT type; };

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
