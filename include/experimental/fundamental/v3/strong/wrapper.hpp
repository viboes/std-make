// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_WRAPPER_HPP

#include <experimental/ordinal.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/fundamental/v3/in_place.hpp>
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
      constexpr wrapper(wrapper const& e) = default;
      constexpr wrapper(wrapper&& e) = default;
      constexpr wrapper& operator=(wrapper const& e) = default;
      constexpr wrapper& operator=(wrapper&& e) = default;

      template <class... Args, typename = enable_if_t<
          is_constructible<UT, Args&&...>::value == true>
      >
          constexpr explicit wrapper(in_place_t, Args&&...args)
          : _value(std::forward<Args>(args)...) {}

      template <class U, class... Args, typename = enable_if_t<
          is_constructible<UT, std::initializer_list<U>&, Args&&...>::value == true>
      >
          constexpr explicit wrapper(in_place_t, std::initializer_list<U> il, Args&&... args)
          : _value(il, std::forward<Args>(args)...) {}

      //! explicit construction from an convertible to underlying type
      //! @par Effects Constructs a wrapper from its underlying type
      template <class U = UT, typename = enable_if_t<
          is_constructible<UT, U&&>::value == true
          && is_same<remove_cvref_t<U>, in_place_t>::value == false
          && is_same<remove_cvref_t<U>, wrapper<UT>>::value == false
          >>
          constexpr explicit wrapper(U&& u)
          : _value(std::forward<U>(u)) {}

      template <class U, enable_if_t<
          is_constructible<UT, const U&>::value == true &&
          is_convertible<const U&, UT>::value == true &&
          is_constructible<UT, wrapper<U>&>::value == false &&
          is_constructible<UT, wrapper<U>&&>::value == false &&
          is_constructible<UT, const wrapper<U>&>::value == false &&
          is_constructible<UT, const wrapper<U>&&>::value == false &&
          is_convertible<wrapper<U>&, UT>::value == false &&
          is_convertible<wrapper<U>&&, UT>::value == false &&
          is_convertible<const wrapper<U>&, UT>::value == false &&
          is_convertible<const wrapper<U>&&, UT>::value == false
          , int> = 0
      >
          constexpr explicit wrapper(const wrapper<U>& other)
          : _value(other._value) {}

      template <class U, enable_if_t<
          is_constructible<UT, const U&>::value == true &&
          is_convertible<const U&, UT>::value == false &&
          is_constructible<UT, wrapper<U>&>::value == false &&
          is_constructible<UT, wrapper<U>&&>::value == false &&
          is_constructible<UT, const wrapper<U>&>::value == false &&
          is_constructible<UT, const wrapper<U>&&>::value == false &&
          is_convertible<wrapper<U>&, UT>::value == false &&
          is_convertible<wrapper<U>&&, UT>::value == false &&
          is_convertible<const wrapper<U>&, UT>::value == false &&
          is_convertible<const wrapper<U>&&, UT>::value == false
          , int> = 0
      >
          constexpr wrapper(const wrapper<U>& other)
          : _value(other._value) {}

      template <class U, enable_if_t<
          is_constructible<UT, U&&>::value == true &&
          is_convertible<U&&, UT>::value == true &&
          is_constructible<UT, wrapper<U>&>::value == false &&
          is_constructible<UT, wrapper<U>&&>::value == false &&
          is_constructible<UT, const wrapper<U>&>::value == false &&
          is_constructible<UT, const wrapper<U>&&>::value == false &&
          is_convertible<wrapper<U>&, UT>::value == false &&
          is_convertible<wrapper<U>&&, UT>::value == false &&
          is_convertible<const wrapper<U>&, UT>::value == false &&
          is_convertible<const wrapper<U>&&, UT>::value == false
          , int> = 0
      >
          constexpr explicit wrapper(wrapper<U>&& other)
          : _value(std::move(other)._value) {}

      template <class U, enable_if_t<
          is_constructible<UT, U&&>::value == true &&
          is_convertible<U&&, UT>::value == false &&
          is_constructible<UT, wrapper<U>&>::value == false &&
          is_constructible<UT, wrapper<U>&&>::value == false &&
          is_constructible<UT, const wrapper<U>&>::value == false &&
          is_constructible<UT, const wrapper<U>&&>::value == false &&
          is_convertible<wrapper<U>&, UT>::value == false &&
          is_convertible<wrapper<U>&&, UT>::value == false &&
          is_convertible<const wrapper<U>&, UT>::value == false &&
          is_convertible<const wrapper<U>&&, UT>::value == false
          , int> = 0
      >
          constexpr wrapper(wrapper<U>&& other)
          : _value(std::move(other)._value) {}

      //! underlying value access
      //! @par Returns the underlying value
      constexpr const underlying_type& underlying() const& noexcept
      { return _value; }

      constexpr const underlying_type&& underlying() const&& noexcept
      { return std::move(_value); }

    protected:
      //! the wrapped value
      underlying_type _value;
    };

    // fixme: do we need an heterogeneous_wrapper that is constructible from convertibles?

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

#if 0

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

#endif
}
}
}

#endif // header
