// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP

#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/ordinal.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/type_traits.hpp>

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
      explicit constexpr wrapper(underlying_type v): value(v) {}

      //! underlying value access
      //! @par Returns the underlying value
      constexpr underlying_type underlying() const noexcept
      { return value; }


    protected:
      //! the wrapped value
      underlying_type value;
    };

#if 0
    // todo: this could help to find out the underlying type of any type inheriting from wrapper
    // This comes from type_safe library.
    namespace detail
    {
        template <typename T>
        T underlying_type(wrapper<T>&);
    } // namespace detail

    /// The underlying type of the [ts::strong_typedef]().
    /// \exclude target
    template <class T>
    using underlying_type_t2 =
        decltype(detail::underlying_type(std::declval<T>()));

#endif

    //! underlying_type specialization for wrapper
//    template <class UT>
//    struct underlying_type<wrapper<UT>>
//    { typedef UT type; };

    template <typename T>
    struct underlying_type<T, meta::when<is_base_of<wrapper_base, T>::value>>
    { using type = typename T::underlying_type; };

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
      using typename base_type::underlying_type;
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


}
}
}

#endif // header
