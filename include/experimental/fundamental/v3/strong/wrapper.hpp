// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP

#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/ordinal.hpp>
#include <functional>

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
      //! @par Effects Constructs a wrapper from its underlying type
      explicit constexpr wrapper(underlying_t v): value(v) {}

      //! underlying value access
      //! @par Returns the underlying value
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

      //! @par Returns the underlying value
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

      //! @par Returns the underlying value
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

    template <class W>
    struct wrapped_hash  {
      using argument_type = W;
      using UT = typename underlying_type<W>::type ;
      using result_type = std::size_t;
      result_type operator()(argument_type const& s) const
      {
          return std::hash<UT>{}(s.underlying());
      }
    };

    template <class W
    , class = typename enable_if<is_ordinal<typename W::underlying_t>{}>::type
    >
    struct wrapped_ordinal_traits
    {
      using T =  typename W::underlying_t;
      //static_assert(is_ordinal<T>::value, "");
      using size_type = size_t;
      using size = integral_constant<size_type, ordinal::traits<T>::max()-numeric_limits<T>::min()+1>;
      static W val(size_type p) { return W{T{p}}; }

      static size_type pos(W w)  { return size_type{w.underlying()}; };
    };
}
}
}

#endif // header
