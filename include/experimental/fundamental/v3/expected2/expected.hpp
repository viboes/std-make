// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba
//
// The design of this class follows the design of std::optional as implemented in libc++

//===-------------------------- optional ----------------------------------===//
//
//                     The LLVM Compiler Infrastructure
//
// This file is dual licensed under the MIT and the University of Illinois Open
// Source Licenses. See LICENSE.TXT for details.
//
//===----------------------------------------------------------------------===//

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_EXPECTED_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_EXPECTED_HPP

#if __cplusplus > 201402L && defined __clang__

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/result/helpers_detail.hpp>
#include <experimental/fundamental/v3/result/success_failure.hpp>
#include <experimental/meta/v1/uncvref.hpp>
#include <type_traits>
#include <utility>
#if __cplusplus <= 201402L || ! defined __clang__
#include <experimental/fundamental/v3/in_place.hpp>
#endif
#include <experimental/fundamental/v3/expected2/unexpected.hpp>
#include <experimental/fundamental/v3/expected2/bad_expected_access.hpp>

#include <memory>
#include <cassert>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

struct unexpect_t
{
    explicit unexpect_t() = default;
};
inline constexpr unexpect_t unexpect{};

namespace expected_detail
{

template <class T, class E, bool = (is_void<T>::value || is_trivially_destructible<T>::value) && is_trivially_destructible<E>::value>
struct expected_destruct_base;

template <class T, class E>
struct expected_destruct_base<T, E, false>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    struct dummy{};
    union
    {
        dummy           _dummy;
        success_type _success;
        failure_type _failure;
    };
    bool _has_value;

    expected_destruct_base()
        : _success(),
          _has_value(true)
    {
    }

    ~expected_destruct_base()
    {
        if (_has_value)
            _success.~success_type();
        else
            _failure.~failure_type();
    }
    constexpr expected_destruct_base(uninitialized_t)
    {}

    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_t, Args&&... args)
        :  _success(in_place, std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<T>, Args&&... args)
        :  _success(in_place, std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<success_type>, Args&&... args)
        :  _success(std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(unexpect_t, Args&&... args)
        :  _failure(in_place, std::forward<Args>(args)...),
           _has_value(false) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<failure_type>, Args&&... args)
        :  _failure(std::forward<Args>(args)...),
           _has_value(false) {}

};

template <class T, class E>
struct expected_destruct_base<T, E, true>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    struct dummy{};

    union
    {
        dummy           _dummy;
        success_type _success;
        failure_type _failure;
    };
    bool _has_value;

    expected_destruct_base()
        : _success(),
          _has_value(true)
    {
    }
    constexpr expected_destruct_base(uninitialized_t)
    {}

    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_t, Args&&... args)
        :  _success(in_place, std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<T>, Args&&... args)
        :  _success(in_place, std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<success_type>, Args&&... args)
        :  _success(std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(unexpect_t, Args&&... args)
        :  _failure(in_place, std::forward<Args>(args)...),
           _has_value(false) {}
    template <class... Args>
    constexpr explicit expected_destruct_base(in_place_type_t<failure_type>, Args&&... args)
        :  _failure(std::forward<Args>(args)...),
           _has_value(false) {}

};

template <class T, class E>
struct expected_storage_common_base: expected_destruct_base<T, E>
{
    using base_type = expected_destruct_base<T, E>;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    using base_type::base_type;

    expected_storage_common_base() = default;

    constexpr bool has_value() const noexcept
    {
        return this->_has_value;
    }

    constexpr explicit operator bool() const noexcept
    {
        return this->has_value();
    }

    constexpr const success_type& get_success() const&
    {
        assert(this->has_value() && "expected must have a value");
        return this->_success;
    }
    constexpr const success_type&& get_success() const&&
    {
        assert(this->has_value() && "expected must have a value");
        return std::move(this->_success);
    }
    constexpr success_type& get_success() &
    {
        assert(this->has_value() && "expected must have a value");
        return this->_success;
    }
    constexpr success_type&& get_success() &&
    {
        assert(this->has_value() && "expected must have a value");
        return std::move(this->_success);
    }

    constexpr const failure_type& get_failure() const&
    {
        assert(!this->has_value() && "expected must have an error");
        return this->_failure;
    }
    constexpr const failure_type&& get_failure() const&&
    {
        assert(!this->has_value() && "expected must have an error");
        return std::move(this->_failure);
    }
    constexpr failure_type& get_failure() &
    {
        assert(!this->has_value() && "expected must have an error");
        return this->_failure;
    }
    constexpr failure_type&& get_failure() &&
    {
        assert(!this->has_value() && "expected must have an error");
        return std::move(this->_failure);
    }

    template <class... Args>
    constexpr void construct(in_place_type_t<success_type>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_success)) success_type(std::forward<Args>(args)...);
        this->_has_value = true;
    }

    template <class... Args>
    constexpr void  construct(in_place_type_t<failure_type>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_failure)) failure_type(std::forward<Args>(args)...);
        this->_has_value = false;
    }
    constexpr void destroy(in_place_type_t<success_type>)
    {
        this->_success.~success_type();
    }
    constexpr void  destroy(in_place_type_t<failure_type>)
    {
        this->_failure.~failure_type();
    }

    template <class That>
    void construct_from(That&& other)
    {
        if (other.has_value())
            construct(in_place_type<success_type>, std::forward<That>(other).get_success());
        else
            construct(in_place_type<failure_type>, std::forward<That>(other).get_failure());
    }

    template <class That>
    void assign_from(That&& other)
    {
        if (this->_has_value)
        {
            if (other.has_value())
            {
                this->_success = std::forward<That>(other).get_success();
            } else {
                if constexpr (is_void_v<T>)
                {
                    construct(in_place_type<failure_type>, std::forward<That>(other).get_failure());
                }
                else if constexpr (is_nothrow_copy_constructible_v<E>)
                {
                    destroy(in_place_type<success_type>);
                    construct(in_place_type<failure_type>, std::forward<That>(other).get_failure());
                }
                else if constexpr (is_nothrow_move_constructible_v<E>)
                {
                    failure_type tmp = std::forward<That>(other).get_failure();
                    destroy(in_place_type<success_type>);
                    construct(in_place_type<failure_type>, std::move(tmp));
                }
                else // is_nothrow_move_constructible_v<T>
                {
                    success_type tmp = this->get_success();
                    destroy(in_place_type<success_type>);
                    try {
                        construct(in_place_type<failure_type>, std::forward<That>(other).get_failure());
                    }
                    catch (...)
                    {
                        construct(in_place_type<success_type>, std::move(tmp));
                        throw;
                    }
                }
            }
        }
        else // ! this->_has_value
        {
            if (other.has_value())
            {
                if constexpr (is_void_v<T>)
                {
                    destroy(in_place_type<failure_type>);
                    this->_has_value = true;
                }
                else if constexpr (is_nothrow_copy_constructible_v<T>)
                {
                    destroy(in_place_type<failure_type>);
                    construct(in_place_type<success_type>, std::forward<That>(other).get_success());
                }
                else if constexpr (is_nothrow_move_constructible_v<T>)
                {
                    success_type tmp = std::forward<That>(other).get_success();
                    destroy(in_place_type<failure_type>);
                    construct(in_place_type<success_type>, std::move(tmp));
                }
                else // `is_nothrow_move_constructible_v<E>`
                {
                    failure_type tmp = this->get_failure();
                    destroy(in_place_type<failure_type>);
                    try {
                        construct(in_place_type<success_type>, std::forward<That>(other).get_success());
                    }
                    catch (...)
                    {
                        construct(in_place_type<failure_type>, std::move(tmp));
                        throw;
                    }
                }
            } else {
                this->_failure = std::forward<That>(other).get_failure();
            }
        }
    }
};

template <class T, class E>
struct expected_storage_common: expected_storage_common_base<T, E>
{
    using base_type = expected_storage_common_base<T, E>;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    using base_type::base_type;


    constexpr const error_type& error() const&
    {
        assert(!this->has_value() && "expected must have an error");
        return this->_failure.value;
    }
    constexpr const error_type&& error() const&&
    {
        assert(!this->has_value() && "expected must have an error");
        return std::move(this->_failure.value);
    }
    constexpr error_type& error() &
    {
        assert(!this->has_value() && "expected must have an error");
        return this->_failure.value;
    }
    constexpr error_type&& error() &&
    {
        assert(!this->has_value() && "expected must have an error");
        return std::move(this->_failure.value);
    }
};

template <class T>
struct expected_storage_common<T, void>: expected_storage_common_base<T, void>
{
    using base_type = expected_storage_common_base<T, void>;
    typedef void E;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    using base_type::base_type;

    constexpr void error() const
    {
    }
};

template <class T, class E, bool = is_reference<T>::value || is_reference<E>::value>
struct expected_storage_base;

template <class T, class E>
struct expected_storage_base<T, E, false>: expected_storage_common<T, E>
{
    using base_type = expected_storage_common<T, E>;
    using base_type::base_type;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    expected_storage_base() = default;

    constexpr const value_type& deref() const&
    {
        assert(this->has_value() && "expected must have a value");
        return this->_success.value;
        //return value_impl(*this);
    }
    constexpr const value_type&& deref() const&&
    {
        assert(this->has_value() && "expected must have a value");
        return std::move(this->_success.value);
        //return value_impl(std::move(*this));
    }
    constexpr value_type& deref() &
    {
        assert(this->has_value() && "expected must have a value");
        return this->_success.value;
        //return value_impl(*this);
        // error: non-const lvalue reference to type 'xxx' (aka 'int') cannot bind to a temporary of type 'int'
        // fixme: why this doesn't work. See p0847r0
    }
    constexpr value_type&& deref() &&
    {
        assert(this->has_value() && "expected must have a value");
        return std::move(this->_success.value);
        //return value_impl(std::move(*this));
    }

    constexpr const T* operator->() const noexcept
    {
        return &this->deref();
    }
    constexpr T* operator->() noexcept
    {
        return &this->deref();
    }
    constexpr const T& operator *() const&
    {
        return this->deref();
    }
    constexpr T& operator*() & noexcept
    {
        return this->deref();
    }
    constexpr const T&& operator*() const &&
    {
        return this->deref();
    }
    constexpr T&& operator*() &&
    {
        return this->deref();
    }


    constexpr const value_type& value() const&
    {
        if (this->has_value())
        {
            return this->deref();
        }
        throw bad_expected_access<E>(this->get_failure());
    }
    constexpr const value_type&& value() const&&
    {
        if (this->has_value())
        {
            return std::move(this->_success.value);
        }
        throw bad_expected_access<E>(this->get_failure());
    }
    constexpr value_type& value() &
    {
        if (this->has_value())
        {
            return this->deref();
        }
        throw bad_expected_access<E>(this->get_failure());
    }
    constexpr value_type&& value() &&
    {
        if (this->has_value())
        {
            return this->deref();
        }
        throw bad_expected_access<E>(this->get_failure());
    }

private:
    template <typename Self>
    static decltype(auto) deref_impl(Self&& self)
    {
        assert(self.has_value() && "expected must have a value");
        return std::forward<Self>(self)._success.value;
    }
};

template <class E>
struct expected_storage_base<void, E, false>: expected_storage_common<void, E>
{
    using base_type = expected_storage_common<void, E>;
    using base_type::base_type;

    expected_storage_base() = default;

    constexpr void value() const& noexcept
    {
    }
    constexpr void value() const&& noexcept
    {
    }
    constexpr void value() & noexcept
    {
    }
    constexpr void value() && noexcept
    {
    }
};

template <class T, class E>
struct expected_storage_base<T, E, true>
{
    //static_assert(false, "Neither T nor E can be reference types");
};

template <class T, class E, bool = is_trivially_copy_constructible_or_void<T>::value && is_trivially_copy_constructible<E>::value>
struct expected_copy_base : expected_storage_base<T, E>
{
    using base_type = expected_storage_base<T, E>;
    using base_type::base_type;
};

template <class T, class E>
struct expected_copy_base<T, E, false> : expected_storage_base<T, E>
{
    using base_type = expected_storage_base<T, E>;
    using base_type::base_type;

    expected_copy_base() = default;

    expected_copy_base(const expected_copy_base& other) : base_type(uninitialized_t{})
    {
        this->construct_from(other);
    }

    expected_copy_base(expected_copy_base&&) = default;
    expected_copy_base& operator=(const expected_copy_base&) = default;
    expected_copy_base& operator=(expected_copy_base&&) = default;

};


template <class T, class E, bool = is_trivially_move_constructible_or_void<T>::value && is_trivially_move_constructible<E>::value>
struct expected_move_base : expected_copy_base<T, E>
{
    using base_type = expected_copy_base<T, E>;
    using base_type::base_type;
};

template <class T, class E>
struct expected_move_base<T, E, false> : expected_copy_base<T, E>
{
    using value_type = T;
    using error_type = E;

    using base_type = expected_copy_base<T, E>;
    using base_type::base_type;

    expected_move_base() = default;
    expected_move_base(const expected_move_base&) = default;

    expected_move_base(expected_move_base&& other)
        noexcept(is_nothrow_move_constructible<value_type>::value && is_nothrow_move_constructible<error_type>::value)
        : base_type(uninitialized_t{})
    {
        this->construct_from(std::move(other));
    }

    expected_move_base& operator=(const expected_move_base&) = default;
    expected_move_base& operator=(expected_move_base&&) = default;
};

template <class T, class E, bool =
                is_trivially_destructible_or_void<T>::value && is_trivially_copy_constructible_or_void<T>::value && is_trivially_copy_assignable_or_void<T>::value &&
                is_trivially_destructible<E>::value && is_trivially_copy_constructible<E>::value && is_trivially_copy_assignable<E>::value>
struct expected_copy_assign_base : expected_move_base<T, E>
{
    using base_type = expected_move_base<T, E>;
    using base_type::base_type;
};

template <class T, class E>
struct expected_copy_assign_base<T, E, false> : expected_move_base<T, E>
{
    using value_type = T;
    using error_type = E;

    using base_type = expected_move_base<T, E>;
    using base_type::base_type;

    expected_copy_assign_base() = default;
    expected_copy_assign_base(const expected_copy_assign_base&) = default;
    expected_copy_assign_base(expected_copy_assign_base&& other) = default;
    expected_copy_assign_base& operator=(const expected_copy_assign_base& other)
    {
        this->assign_from(other);
        return *this;
    }
    expected_copy_assign_base& operator=(expected_copy_assign_base&& other) = default;
};

template <class T, class E, bool =
                is_trivially_destructible_or_void<T>::value && is_trivially_move_constructible_or_void<T>::value && is_trivially_move_assignable_or_void<T>::value &&
                is_trivially_destructible<E>::value && is_trivially_move_constructible<E>::value && is_trivially_move_assignable<E>::value>
struct expected_move_assign_base : expected_copy_assign_base<T, E>
{
    using base_type = expected_copy_assign_base<T, E>;
    using base_type::base_type;
};

template <class T, class E>
struct expected_move_assign_base<T, E, false> : expected_copy_assign_base<T, E>
{
    using value_type = T;
    using error_type = E;

    using base_type = expected_copy_assign_base<T, E>;
    using base_type::base_type;

    expected_move_assign_base() = default;
    expected_move_assign_base(const expected_move_assign_base&) = default;
    expected_move_assign_base(expected_move_assign_base&& other) = default;
    expected_move_assign_base& operator=(const expected_move_assign_base&) = default;
    expected_move_assign_base& operator=(expected_move_assign_base&& other)
    {
        this->assign_from(std::move(other));
        return *this;
    }
};

template <class T, class E>
using expected_sfinae_default_ctor_base = helpers_detail::sfinae_default_ctor_base<
                is_default_constructible_or_void<T>::value
>;

template <class T, class E>
using expected_sfinae_ctor_base = helpers_detail::sfinae_ctor_base<
                is_copy_constructible_or_void<T>::value && is_copy_constructible<E>::value,
                is_move_constructible_or_void<T>::value && is_move_constructible<E>::value
>;

template <class T, class E>
using expected_sfinae_assign_base = helpers_detail::sfinae_assign_base<
                is_copy_constructible_or_void<T>::value && is_copy_constructible<E>::value &&
                is_copy_assignable_or_void<T>::value && is_copy_assignable<E>::value &&
                (is_nothrow_move_constructible_v<E> || is_nothrow_move_constructible_v<T>),
                is_move_constructible_or_void<T>::value && is_move_constructible<E>::value &&
                is_move_assignable_or_void<T>::value && is_move_assignable<E>::value &&
                (is_nothrow_move_constructible_v<E> || is_nothrow_move_constructible_v<T>)
>;

template <class T, class E>
class expected_base : protected expected_move_assign_base<T,E>
, private expected_sfinae_default_ctor_base<T,E>
, private expected_sfinae_ctor_base<T,E>
, private expected_sfinae_assign_base<T,E>
{
    static_assert(is_destructible<T>::value || is_void<T>::value,
        "instantiation of expected with a non-destructible non-void T type is ill-formed");
    static_assert(is_destructible<E>::value || is_void<E>::value,
        "instantiation of expected with a non-destructible non-void E type is ill-formed");

    using base_type = expected_detail::expected_move_assign_base<T, E>;

    template <class U, class QualU>
    using check_expected_failure_ctor = helpers_detail::check_constructible<U, QualU>;

public:
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    using base_type::base_type;

    expected_base() = default;
    expected_base(const expected_base& rhs) = default;
    expected_base(expected_base&& rhs)  = default;
    expected_base& operator=(expected_base const& e) = default;
    expected_base& operator=(expected_base && e) = default;
    ~expected_base() = default;

    template < class U = E, enable_if_t<
                    check_expected_failure_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr expected_base(unexpected<U> const& other) :
    base_type(in_place_type<failure_type>, other)
    {}
    template < class U = E, enable_if_t<
                    check_expected_failure_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr expected_base(unexpected<U> const& other) :
    base_type(in_place_type<failure_type>, other)
    {}
    template < class U = E, enable_if_t<
                    check_expected_failure_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr expected_base(unexpected<U> && other) :
    base_type(in_place_type<failure_type>, std::move(other))
    {}
    template < class U = E, enable_if_t<
                    check_expected_failure_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr expected_base(unexpected<U> && other) :
    base_type(in_place_type<failure_type>, std::move(other))
    {}

    using base_type::has_value;
    using base_type::value;
    using base_type::get_success;
    using base_type::error;
    using base_type::get_failure;
};

}

template <class T, class E>
class expected : public expected_detail::expected_base<T, E>
{
    using base_type = expected_detail::expected_base<T, E>;

    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    template <class U, class G, class Exp = expected<U, G>>
    using check_constructible_from_exp = disjunction<
        is_constructible<T, Exp&>,
        is_constructible<T, Exp const&>,
        is_constructible<T, Exp&&>,
        is_constructible<T, Exp const&&>,
        is_convertible<Exp&, T>,
        is_convertible<Exp const&, T>,
        is_convertible<Exp&&, T>,
        is_convertible<Exp const&&, T>,

        is_constructible<success_type, Exp&>,
        is_constructible<success_type, Exp const&>,
        is_constructible<success_type, Exp&&>,
        is_constructible<success_type, Exp const&&>,
        is_convertible<Exp&, success_type>,
        is_convertible<Exp const&, success_type>,
        is_convertible<Exp&&, success_type>,
        is_convertible<Exp const&&, success_type>,

        is_constructible<failure_type, Exp&>,
        is_constructible<failure_type, Exp const&>,
        is_constructible<failure_type, Exp&&>,
        is_constructible<failure_type, Exp const&&>,
        is_convertible<Exp&, failure_type>,
        is_convertible<Exp const&, failure_type>,
        is_convertible<Exp&&, failure_type>,
        is_convertible<Exp const&&, failure_type>
    >;

    template <class U, class G, class Exp = expected<U, G>>
    using check_assignable_from_exp = disjunction<
        is_assignable<T&, Exp&>,
        is_assignable<T&, Exp const&>,
        is_assignable<T&, Exp&&>,
        is_assignable<T&, Exp const&&>,

        is_assignable<success_type&, Exp&>,
        is_assignable<success_type&, Exp const&>,
        is_assignable<success_type&, Exp&&>,
        is_assignable<success_type&, Exp const&&>,

        is_assignable<failure_type&, Exp&>,
        is_assignable<failure_type&, Exp const&>,
        is_assignable<failure_type&, Exp&&>,
        is_assignable<failure_type&, Exp const&&>
    >;

    template <class QualU>
    using check_expected_success_ctor = helpers_detail::check_constructible<T, QualU>;

    template <class U>
    using check_expected_u_ctor = typename conditional<
                    !is_same_v<meta::uncvref_t<U>, in_place_t> &&
                    !is_same_v<meta::uncvref_t<U>, in_place_type_t<T>> &&
                    !is_same_v<meta::uncvref_t<U>, in_place_type_t<success_type>> &&
                    !is_same_v<meta::uncvref_t<U>, in_place_type_t<failure_type>> &&
                    !is_same_v<meta::uncvref_t<U>, expected<T, E>>,
                    helpers_detail::check_constructible<T, U>,
                    helpers_detail::check_fail
                >::type;


    template <class U, class G, class QualU, class QualG>
    using check_expected_expected_ctor = typename conditional<
        (!is_same<T, U>::value || !is_same<E, G>::value) && ! check_constructible_from_exp<U,G>::value,
        helpers_detail::check_constructibles<T, E, QualU, QualG>,
        helpers_detail::check_fail
    >::type;

    template <class U, class G, class QualU, class QualG>
    using check_expected_expected_assign = typename conditional<
        (!is_same<T, U>::value || !is_same<E, G>::value) && ! check_constructible_from_exp<U,G>::value && ! check_assignable_from_exp<U,G>::value,
        helpers_detail::check_constructibles<T, E, QualU, QualG>,
        helpers_detail::check_fail
    >::type;

public:
    //static_assert(! is_success<T>::value, "Error: T can not be success<U>.");
    //static_assert(! is_in_place_type<T>::value, "Error: T can not be in_place_type<U>.");
    //static_assert(! is_failure<E>::value, "Error: E can not be failure<G>.");
    //static_assert(! is_in_place_type<E>::value, "Error: E can not be in_place_type<G>.");

    template <class U>
    using rebind = expected<U, E>;

    using base_type::base_type;

    expected() = default;
    expected(const expected& rhs) = default;
    expected(expected&& rhs)  = default;
    expected& operator=(expected const& e) = default;
    expected& operator=(expected && e) = default;
    ~expected() = default;

    template < class U = T, enable_if_t<
                    check_expected_u_ctor<U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr expected(U&& value) :
        base_type(in_place_type<T>, std::forward<U>(value))
    {}
    template < class U = T, enable_if_t<
                    check_expected_u_ctor<U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr expected(U&& value) :
        base_type(in_place_type<T>, std::forward<U>(value))
    {}


    template < class U = T, enable_if_t<
                    check_expected_success_ctor<U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr expected(success<U> const& value) :
        base_type(in_place_type<success_type>, value)
    {}
    template < class U = T, enable_if_t<
                    check_expected_success_ctor<U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr expected(success<U> const& value) :
        base_type(in_place_type<success_type>, value)
    {}
    template < class U = T, enable_if_t<
                    check_expected_success_ctor<U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr expected(success<U> && value) :
        base_type(in_place_type<success_type>, std::move(value))
    {}
    template < class U = T, enable_if_t<
                    check_expected_success_ctor<U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr expected(success<U> && value) :
        base_type(in_place_type<success_type>, std::move(value))
    {}


    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_ctor<U, G, U const&, G const&>::template enable_implicit<U, G>()
                , int> = 0>
    constexpr expected(expected<U, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_ctor<U, G, U const&, G const&>::template enable_explicit<U, G>()
                , int> = 0>
    explicit constexpr expected(expected<U, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }

    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_ctor<U, G, U &&, G&&>::template enable_implicit<U, G>()
                , int> = 0>
    constexpr expected(expected<U, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }

    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_ctor<U, G, U &&, G&&>::template enable_explicit<U, G>()
                , int> = 0>
    explicit constexpr expected(expected<U, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }



    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_assign<U, G, U const&, G const&>::template enable_assign<U, G>()
                , int> = 0>
    constexpr expected& operator=(expected<U, G> const& other)
    {
        this->assign_from(other);
        return *this;
    }

    template < class U = T, class G = E, enable_if_t<
                    check_expected_expected_assign<U, G, U &&, G&&>::template enable_assign<U, G>()
                , int> = 0>
    constexpr expected& operator=(expected<U, G> && other)
    {
        this->assign_from(std::move(other));
        return *this;
    }

    template <class... Args,
        enable_if_t<
            is_constructible_v<value_type, Args...>
        , int> = 0
    >
        T& emplace(Args&&... args)
    {
        if (this->has_value())
        {
            this->deref() = T{std::forward<Args>(args)...};
        } else {
            if constexpr(is_nothrow_constructible_v<T, Args&&...>)
            {
                this->destroy(in_place_type<failure_type>);
                this->construct(in_place_type<success_type>, std::forward<Args>(args)...);
            }
            else if constexpr(is_nothrow_move_constructible_v<T>)
            {
                auto tmp =  T{std::forward<Args>(args)...};
                this->destroy(in_place_type<failure_type>);
                this->construct(in_place_type<success_type>, std::move(tmp));
            }
            else
            {
                auto tmp =  std::move(this->get_failure());
                this->destroy(in_place_type<failure_type>);
                try {
                    this->construct(in_place_type<success_type>, std::forward<Args>(args)...);
                } catch (...) {
                    this->construct(in_place_type<failure_type>, std::move(tmp));
                    throw;
                }
            }
        }
        return this->deref();
    }

    template <class U, class... Args,
        enable_if_t<
            is_constructible_v<value_type, initializer_list<U>&, Args...>
        , int> = 0
    >
        T& emplace(initializer_list<U> il, Args&&... args)
    {
        if (this->has_value())
        {
            this->deref() = T{il, std::forward<Args>(args)...};
        } else {
            if constexpr(is_nothrow_constructible_v<T, initializer_list<U>&, Args&&...>)
            {
                this->destroy(in_place_type<failure_type>);
                this->construct(in_place_type<success_type>, il, std::forward<Args>(args)...);
            }
            else if constexpr(is_nothrow_move_constructible_v<T>)
            {
                auto tmp =  T{il, std::forward<Args>(args)...};
                this->destroy(in_place_type<failure_type>);
                this->construct(in_place_type<success_type>, std::move(tmp));
            }
            else
            {
                auto tmp =  std::move(this->get_failure());
                this->destroy(in_place_type<failure_type>);
                try {
                    this->construct(in_place_type<success_type>, il, std::forward<Args>(args)...);
                } catch (...) {
                    this->construct(in_place_type<failure_type>, std::move(tmp));
                    throw;
                }
            }
        }
        return this->deref();
    }

    template < class U = T,
                    enable_if_t<
                        is_constructible_v<value_type, U&&>
                    , int> = 0
                >
    constexpr expected& operator=(U&& v)
    {
        this->emplace(std::forward<U>(v));
        return *this;
    }

    void swap(expected& other)
        noexcept(is_nothrow_move_constructible_v<T> && is_nothrow_swappable_v<T> &&
                 is_nothrow_move_constructible_v<E> && is_nothrow_swappable_v<E>)
     {
        if (this->_has_value)
        {
            if (other.has_value())
            {
                using std::swap;
                swap(this->get_success(), other.get_success());
            }
            else
            {
                if constexpr(is_nothrow_move_constructible_v<E>)
                {
                    failure_type tmp = std::move(other.get_failure());
                    try {
                        other.destroy(in_place_type<failure_type>);
                        other.construct(in_place_type<success_type>, std::move(this->get_success()));
                        this->destroy(in_place_type<success_type>);
                        this->construct(in_place_type<failure_type>, std::move(tmp));
                    }
                    catch (...)
                    {
                        other.construct(in_place_type<failure_type>, std::move(tmp));
                        throw;
                    }
                }
                else //if constexpr(is_nothrow_move_constructible_v<T>)
                {
                    success_type tmp = std::move(this->get_success());
                    this->destroy(in_place_type<success_type>);
                    try {
                        this->construct(in_place_type<failure_type>, std::move(other.get_failure()));
                        other.destroy(in_place_type<failure_type>);
                        other.construct(in_place_type<success_type>, std::move(tmp));
                    }
                    catch (...)
                    {
                        this->construct(in_place_type<success_type>, std::move(tmp));
                        throw;
                    }
                }
            }
        }
        else
        {
            if (other.has_value())
            {
                other.swap(*this);

            }
            else
            {
                using std::swap;
                swap(this->get_failure(), other.get_failure());
            }
        }


     }

};

template <class E>
class expected<void, E> : public expected_detail::expected_base<void, E>
{
    using T = void;
    using base_type = expected_detail::expected_base<T, E>;

    typedef success<void> success_type;
    typedef E error_type;
    typedef unexpected<E> failure_type;
    typedef unexpected<E> unexpected_type;

    template <class G, class Exp = expected<void, G>>
    using check_constructible_from_exp = disjunction<
        is_constructible<success_type, Exp&>,
        is_constructible<success_type, Exp const&>,
        is_constructible<success_type, Exp&&>,
        is_constructible<success_type, Exp const&&>,
        is_convertible<Exp&, success_type>,
        is_convertible<Exp const&, success_type>,
        is_convertible<Exp&&, success_type>,
        is_convertible<Exp const&&, success_type>,

        is_constructible<failure_type, Exp&>,
        is_constructible<failure_type, Exp const&>,
        is_constructible<failure_type, Exp&&>,
        is_constructible<failure_type, Exp const&&>,
        is_convertible<Exp&, failure_type>,
        is_convertible<Exp const&, failure_type>,
        is_convertible<Exp&&, failure_type>,
        is_convertible<Exp const&&, failure_type>
    >;

    template <class G, class Exp = expected<void, G>>
    using check_assignable_from_exp = disjunction<
        is_assignable<success_type&, Exp&>,
        is_assignable<success_type&, Exp const&>,
        is_assignable<success_type&, Exp&&>,
        is_assignable<success_type&, Exp const&&>,

        is_assignable<failure_type&, Exp&>,
        is_assignable<failure_type&, Exp const&>,
        is_assignable<failure_type&, Exp&&>,
        is_assignable<failure_type&, Exp const&&>
    >;

    template <class G, class QualG>
    using check_expected_expected_ctor = typename conditional<
        (!is_same<E, G>::value) && ! check_constructible_from_exp<G>::value,
        helpers_detail::check_constructible<E, QualG>,
        helpers_detail::check_fail
    >::type;

    template <class G, class QualG>
    using check_expected_expected_assign = typename conditional<
        (!is_same<E, G>::value) && ! check_constructible_from_exp<G>::value && ! check_assignable_from_exp<G>::value,
        helpers_detail::check_constructible<E, QualG>,
        helpers_detail::check_fail
    >::type;

public:
    template <class U>
    using rebind = expected<U, E>;

    using base_type::base_type;

    expected() = default;
    expected(const expected& rhs) = default;
    expected(expected&& rhs)  = default;
    expected& operator=(expected const& e) = default;
    expected& operator=(expected && e) = default;
    ~expected() = default;

    constexpr expected(success<void> const& value) :
        base_type(in_place_type<success_type>, value)
    {}
    constexpr expected(success<void> && value) :
        base_type(in_place_type<success_type>, std::move(value))
    {}

    template < class G = E, enable_if_t<
                    check_expected_expected_ctor<G, G const&>::template enable_implicit<G>()
                , int> = 0>
    constexpr expected(expected<void, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class G = E, enable_if_t<
                    check_expected_expected_ctor<G, G const&>::template enable_explicit<G>()
                , int> = 0>
    explicit constexpr expected(expected<void, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class G = E, enable_if_t<
                    check_expected_expected_ctor<G, G &&>::template enable_implicit<G>()
                , int> = 0>
    constexpr expected(expected<void, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }
    template < class G = E, enable_if_t<
                    check_expected_expected_ctor<G, G &&>::template enable_explicit<G>()
                , int> = 0>
    explicit constexpr expected(expected<void, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }

    template < class G = E, enable_if_t<
                    check_expected_expected_assign<G, G const&>::template enable_assign<G>()
                , int> = 0>
    constexpr expected& operator=(expected<void, G> const& other)
        {
            this->assign_from(other);
            return *this;
        }

    template < class G = E, enable_if_t<
                    check_expected_expected_assign<G, G&&>::template enable_assign<G>()
                , int> = 0>
    constexpr expected& operator=(expected<void, G> && other)
        {
            this->assign_from(std::move(other));
            return *this;
        }

#if 0
    void emplace()
    {
        if (this->has_value())
        {
        } else {
            this->destroy(in_place_type<failure_type>);
            this->construct(in_place_type<success_type>);
        }
    }
#endif

    void swap(expected& other)
        noexcept(is_nothrow_move_constructible_v<E> && is_nothrow_swappable_v<E>)
     {
        if (this->_has_value)
        {
            if (other.has_value())
            {
                // nothing
            }
            else
            {
                this->construct(in_place_type<failure_type>, other.get_failure()); // this can throw
                other.destroy(in_place_type<failure_type>);
            }
        }
        else
        {
            if (other.has_value())
            {
                other.swap(*this);
            }
            else
            {
                using std::swap;
                swap(this->get_failure(), other.get_failure());
            }
        }
     }

};

template <class T, class E>
void swap(expected<T, E>& lhs, expected<T, E>& rhs)
{
    return lhs.swap(rhs);
}


template <class T1, class E1, class T2, class E2>
constexpr bool operator==(const expected<T1, E1>& x, const expected<T2, E2>& y)
{
  return (x.has_value() && y.has_value())
    ? x.get_success() == y.get_success()
    : (!x.has_value() && !y.has_value())
      ?  x.get_failure() == y.get_failure()
      : false;
}

template <class T1, class E1, class T2, class E2>
constexpr bool operator!=(const expected<T1, E1>& x, const expected<T2, E2>& y)
{
  return !(x == y);
}

template <class T, class E, class G>
  constexpr bool operator==(const expected<T, E>& x, const G& y)
  {
    return ! x.has_value() ? false : x.get_success() == y;
  }
template <class G, class T, class E>
  constexpr bool operator==(const G& x, const expected<T, E>& y)
  {
    return y == x;
  }
template <class T, class E, class G>
  constexpr bool operator!=(const expected<T, E>& x, const G& y)
  {
    return ! x.has_value() ? true : x.get_success() != y;
  }
template <class G, class T, class E>
  constexpr bool operator!=(const G& x, const expected<T, E>& y)
  {
    return y != x;
  }

template <class T, class E, class G>
  constexpr bool operator==(const expected<T, E>& x, const success<G>& y)
  {
    return ! x.has_value() ? false : x.get_success() == y;
  }
template <class G, class T, class E>
  constexpr bool operator==(const success<G>& x, const expected<T, E>& y)
  {
    return y == x;
  }
template <class T, class E, class G>
  constexpr bool operator!=(const expected<T, E>& x, const success<G>& y)
  {
    return ! x.has_value() ? true : x.get_success() != y;
  }
template <class G, class T, class E>
  constexpr bool operator!=(const success<G>& x, const expected<T, E>& y)
  {
    return y != x;
  }

template <class T, class E, class G>
  constexpr bool operator==(const expected<T, E>& x, const unexpected<G>& y)
  {
    return x.has_value() ? false : x.get_failure() == y;
  }
template <class G, class T, class E>
  constexpr bool operator==(const unexpected<G>& x, const expected<T, E>& y)
  {
    return y == x;
  }
template <class T, class E, class G>
  constexpr bool operator!=(const expected<T, E>& x, const unexpected<G>& y)
  {
    return x.has_value() ? true : x.get_failure() != y;
  }
template <class G, class T, class E>
  constexpr bool operator!=(const unexpected<G>& x, const expected<T, E>& y)
  {
    return y != x;
  }

}
}
}
#endif
#endif // header
