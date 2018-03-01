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

#ifndef JASEL_EXPERIMENTAL_V3_RESULT_RESULT_HPP
#define JASEL_EXPERIMENTAL_V3_RESULT_RESULT_HPP

#if __cplusplus >= 201402L && defined __clang__


#include <experimental/fundamental/v2/config.hpp>

#include <type_traits>
#include <utility>
#include <memory>

#if __cplusplus <= 201402L
#include <experimental/fundamental/v3/in_place.hpp>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace result_detail
{
struct check_implicit {
  template <class U>
  static constexpr bool enable_implicit() {
      return true;
  }

  template <class U>
  static constexpr bool enable_explicit() {
      return false;
  }
};

struct check_fail {
  template <class U>
  static constexpr bool enable_implicit() {
      return false;
  }

  template <class U>
  static constexpr bool enable_explicit() {
      return false;
  }
};
template <class T, class QualU>
struct check_constructible {
  template <class U>
  static constexpr bool enable_implicit() {
      return is_constructible<T, QualU>::value &&
             is_convertible<QualU, T>::value;
  }

  template <class U>
  static constexpr bool enable_explicit() {
      return is_constructible<T, QualU>::value &&
             !is_convertible<QualU, T>::value;
  }
};

template <class T, class U, class QualU>
using check_void_or_constructible = conditional_t<
    !is_void<T>::value,
    check_constructible<T, QualU>,
    result_detail::check_implicit
>;

template <class T, class U, class QualU>
using check_diff_or_constructible = conditional_t<
    !is_same<T, U>::value,
    check_constructible<T, QualU>,
    result_detail::check_fail
>;

}

template <class T>
struct success {
private:
    template <class U, class QualU>
    using check_success_ctor = result_detail::check_diff_or_constructible<T, U, QualU>;

public:
    T value;

    success() = delete;
    constexpr success(success const& e) = default;
    constexpr success(success&& e) = default;
    constexpr success& operator=(success const& e) = delete;
    constexpr success& operator=(success&& e) = delete;

    constexpr explicit success(T const& v) :
        value(v)
    {
    }
    constexpr explicit success(T&& v) :
        value(std::move(v))
    {
    }

    template < class U = T, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr success(success<U> const& other) :
        value(other.value)
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr success(success<U> const& other) :
        value(other.value)
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr success(success<U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr success(success<U> && other) :
        value(std::move(other.value))
    {}
};

template <class T>
success<std::decay_t<T>> make_success(T&& value)
{
    return success<std::decay_t<T>>(std::forward<T>(value));
}

template <>
struct success<void> {

    constexpr success(success const& e) = default;
    constexpr success(success&& e) = default;
    constexpr success& operator=(success const& e) = delete;
    constexpr success& operator=(success&& e) = delete;

    constexpr explicit success()
    {
    }

};

success<void> make_success()
{
    return success<void>();
}


template <class E>
struct failure {
private:
    template <class U, class QualU>
    using check_success_ctor = result_detail::check_diff_or_constructible<E, U, QualU>;

public:
    E value;

    static_assert(! is_same<E,void>::value, "The E of failure<E> can not be void.");

    failure() = delete;
    constexpr failure(failure const& e) = default;
    constexpr failure(failure&& e) = default;
    constexpr failure& operator=(failure const& e) = delete;
    constexpr failure& operator=(failure&& e) = delete;

    constexpr explicit failure(E const& e) :
                value(e)
    {
    }
    constexpr explicit failure(E&& e) :
                value(std::move(e))
    {
    }

    template < class U = E, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr failure(failure<U> const& other) :
        value(other.value)
    {}
    template < class U = E, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr failure(failure<U> const& other) :
        value(other.value)
    {}
    template < class U = E, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr failure(failure<U> && other) :
        value(std::move(other.value))
    {}
    template < class U = E, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr failure(failure<U> && other) :
        value(std::move(other.value))
    {}
};

template <class E>
failure<std::decay_t<E>> make_failure(E&& error)
{
    return failure<std::decay_t<E>>(std::forward<E>(error));
}

namespace result_detail {

struct uninitialized_t {};

template <class T, class E, bool = (is_void<T>::value || is_trivially_destructible<T>::value) && is_trivially_destructible<E>::value>
struct result_destruct_base;

template <class T, class E>
struct result_destruct_base<T, E, false>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;
    static_assert(is_object<value_type>::value,
        "instantiation of result with a non-object type is undefined behavior");
    static_assert(is_object<error_type>::value,
        "instantiation of result with a non-object E type is undefined behavior");
    union
    {
        success_type _success;
        failure_type _failure;
    };
    bool _has_value;

    ~result_destruct_base()
    {
        if (_has_value)
            _success.~success_type();
        else
            _failure.~failure_type();
    }
    constexpr result_destruct_base(uninitialized_t)
    {}

    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<success<T>>, Args&&... args)
        :  _success(std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<failure<E>>, Args&&... args)
        :  _failure(std::forward<Args>(args)...),
           _has_value(false) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<T>, Args&&... args)
        :  _success(value_type(std::forward<Args>(args)...)),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<E>, Args&&... args)
        :  _failure(error_type(std::forward<Args>(args)...)),
           _has_value(false) {}

};

template <class T, class E>
struct result_destruct_base<T, E, true>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;
    static_assert(is_object<value_type>::value || is_void<value_type>::value,
        "instantiation of result with a non-object type or void is undefined behavior");
    static_assert(is_object<error_type>::value,
        "instantiation of result with a non-object E type is undefined behavior");
    union
    {
        success_type _success;
        failure_type _failure;
    };
    bool _has_value;

    constexpr result_destruct_base(uninitialized_t)
    {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<success<T>>, Args&&... args)
        :  _success(std::forward<Args>(args)...),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<failure<E>>, Args&&... args)
        :  _failure(std::forward<Args>(args)...),
           _has_value(false) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<T>, Args&&... args)
        :  _success(value_type(std::forward<Args>(args)...)),
           _has_value(true) {}
    template <class... Args>
    constexpr explicit result_destruct_base(in_place_type_t<E>, Args&&... args)
        :  _failure(error_type(std::forward<Args>(args)...)),
           _has_value(false) {}

};

template <class T, class E, bool = is_reference<T>::value>
struct result_storage_base;

template <class T, class E>
struct result_storage_base<T, E, false>: result_destruct_base<T, E>
{
    using base_type = result_destruct_base<T, E>;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;

    using base_type::base_type;

    constexpr bool has_value() const noexcept
    {
        return this->_has_value;
    }

    constexpr const value_type& value() const& noexcept
    {
        return this->_success.value;
    }
    constexpr const value_type&& value() const&& noexcept
    {
        return std::move(this->_success.value);
    }
    constexpr value_type& value() & noexcept
    {
        return this->_success.value;
    }
    constexpr value_type&& value() && noexcept
    {
        return std::move(this->_success.value);
    }
    constexpr const success_type& get_success() const& noexcept
    {
        return this->_success;
    }
    constexpr const success_type&& get_success() const&& noexcept
    {
        return std::move(this->_success);
    }
    constexpr success_type& get_success() & noexcept
    {
        return this->_success;
    }
    constexpr success_type&& get_success() && noexcept
    {
        return std::move(this->_success);
    }

    constexpr const error_type& error() const& noexcept
    {
        return this->_failure.value;
    }
    constexpr const error_type&& error() const&& noexcept
    {
        return std::move(this->_failure.value);
    }
    constexpr error_type& error() & noexcept
    {
        return this->_failure.value;
    }
    constexpr error_type&& error() && noexcept
    {
        return std::move(this->_failure.value);
    }
    constexpr const failure_type& get_failure() const& noexcept
    {
        return this->_failure;
    }
    constexpr const failure_type&& get_failure() const&& noexcept
    {
        return std::move(this->_failure);
    }
    constexpr failure_type& get_failure() & noexcept
    {
        return this->_failure;
    }
    constexpr failure_type&& get_failure() && noexcept
    {
        return std::move(this->_failure);
    }

    template <class... Args>
    constexpr void construct(in_place_type_t<success<T>>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_success)) success_type(std::forward<Args>(args)...);
        this->_has_value = true;
    }
    template <class... Args>
    constexpr void  construct(in_place_type_t<failure<E>>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_failure)) failure_type(std::forward<Args>(args)...);
        this->_has_value = false;
    }

    template <class That>
    void construct_from(That&& other)
    {
        if (other.has_value())
            construct(in_place_type<success<T>>, std::forward<That>(other).get_success());
        else
            construct(in_place_type<failure<E>>, std::forward<That>(other).get_failure());
    }

};

template <class E>
struct result_storage_base<void, E, false>: result_destruct_base<void, E>
{
    using T = void;
    using base_type = result_destruct_base<T, E>;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;

    using base_type::base_type;

    constexpr bool has_value() const noexcept
    {
        return this->_has_value;
    }

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
    constexpr const success_type& get_success() const& noexcept
    {
        return this->_success;
    }
    constexpr const success_type&& get_success() const&& noexcept
    {
        return std::move(this->_success);
    }
    constexpr success_type& get_success() & noexcept
    {
        return this->_success;
    }
    constexpr success_type&& get_success() && noexcept
    {
        return std::move(this->_success);
    }

    constexpr const error_type& error() const& noexcept
    {
        return this->_failure.value;
    }
    constexpr const error_type&& error() const&& noexcept
    {
        return std::move(this->_failure.value);
    }
    constexpr error_type& error() & noexcept
    {
        return this->_failure.value;
    }
    constexpr error_type&& error() && noexcept
    {
        return std::move(this->_failure.value);
    }
    constexpr const failure_type& get_failure() const& noexcept
    {
        return this->_failure;
    }
    constexpr const failure_type&& get_failure() const&& noexcept
    {
        return std::move(this->_failure);
    }
    constexpr failure_type& get_failure() & noexcept
    {
        return this->_failure;
    }
    constexpr failure_type&& get_failure() && noexcept
    {
        return std::move(this->_failure);
    }

    template <class... Args>
    constexpr void construct(in_place_type_t<success<T>>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_success)) success_type(std::forward<Args>(args)...);
        this->_has_value = true;
    }
    template <class... Args>
    constexpr void  construct(in_place_type_t<failure<E>>, Args&&... args)
    {
        ::new((void*)std::addressof(this->_failure)) failure_type(std::forward<Args>(args)...);
        this->_has_value = false;
    }

    template <class That>
    void construct_from(That&& other)
    {
        if (other.has_value())
            construct(in_place_type<success<T>>, std::forward<That>(other).get_success());
        else
            construct(in_place_type<failure<E>>, std::forward<That>(other).get_failure());
    }
};

template <class T, class E>
struct result_storage_base<T, E, true>
{
    //static_assert(false, "T can not be a reference");
};


template <class T>
struct is_trivially_copy_constructible_or_void : is_trivially_copy_constructible<T> {};
template <>
struct is_trivially_copy_constructible_or_void<void> : true_type {};

template <class T, class E, bool = is_trivially_copy_constructible_or_void<T>::value && is_trivially_copy_constructible<E>::value>
struct result_copy_base : result_storage_base<T, E>
{
    using result_storage_base<T, E>::result_storage_base;
};

template <class T, class E>
struct result_copy_base<T, E, false> : result_storage_base<T, E>
{
    using base_type = result_storage_base<T, E>;
    using base_type::base_type;

    result_copy_base(const result_copy_base& other) : base_type(uninitialized_t{})
    {
        this->construct_from(other);
    }

    result_copy_base(result_copy_base&&) = default;
};

template <class T>
struct is_trivially_move_constructible_or_void : is_trivially_move_constructible<T> {};
template <>
struct is_trivially_move_constructible_or_void<void> : true_type {};

template <class T, class E, bool = is_trivially_move_constructible_or_void<T>::value && is_trivially_move_constructible<E>::value>
struct result_move_base : result_copy_base<T, E>
{
    using result_copy_base<T, E>::result_copy_base;
};

template <class T, class E>
struct result_move_base<T, E, false> : result_copy_base<T, E>
{
    using value_type = T;
    using error_type = E;

    using base_type = result_copy_base<T, E>;
    using base_type::base_type;

    result_move_base(const result_move_base&) = default;

    result_move_base(result_move_base&& other)
        noexcept(is_nothrow_move_constructible<value_type>::value && is_nothrow_move_constructible<error_type>::value)
        : base_type(uninitialized_t{})
    {
        this->construct_from(std::move(other));
    }

};


// move to sfinae_delete_base
template <bool CanCopy, bool CanMove>
struct sfinae_ctor_base {};
template <>
struct sfinae_ctor_base<false, false>
{
  sfinae_ctor_base() = default;
  sfinae_ctor_base(sfinae_ctor_base const&) = delete;
  sfinae_ctor_base(sfinae_ctor_base &&) = delete;
  sfinae_ctor_base& operator=(sfinae_ctor_base const&) = default;
  sfinae_ctor_base& operator=(sfinae_ctor_base&&) = default;
};
template <>
struct sfinae_ctor_base<true, false>
{
  sfinae_ctor_base() = default;
  sfinae_ctor_base(sfinae_ctor_base const&) = default;
  sfinae_ctor_base(sfinae_ctor_base &&) = delete;
  sfinae_ctor_base& operator=(sfinae_ctor_base const&) = default;
  sfinae_ctor_base& operator=(sfinae_ctor_base&&) = default;
};
template <>
struct sfinae_ctor_base<false, true>
{
  sfinae_ctor_base() = default;
  sfinae_ctor_base(sfinae_ctor_base const&) = delete;
  sfinae_ctor_base(sfinae_ctor_base &&) = default;
  sfinae_ctor_base& operator=(sfinae_ctor_base const&) = default;
  sfinae_ctor_base& operator=(sfinae_ctor_base&&) = default;
};

template <class T>
struct is_copy_constructible_or_void : is_copy_constructible<T> {};
template <>
struct is_copy_constructible_or_void<void> : true_type {};

template <class T>
struct is_move_constructible_or_void : is_move_constructible<T> {};
template <>
struct is_move_constructible_or_void<void> : true_type {};


template <class T, class E>
using result_sfinae_ctor_base = sfinae_ctor_base<
                is_copy_constructible_or_void<T>::value && is_copy_constructible<E>::value,
                is_move_constructible_or_void<T>::value && is_move_constructible<E>::value
>;



template <class T, class E>
class result_base : private result_move_base<T,E>
             , private result_sfinae_ctor_base<T,E>
{
    static_assert(is_destructible<T>::value || is_void<T>::value,
        "instantiation of result with a non-destructible non-void T type is ill-formed");
    static_assert(is_destructible<E>::value,
        "instantiation of result with a non-destructible E type is ill-formed");

    using base_type = result_detail::result_move_base<T, E>;

    template <class U, class QualU>
    using check_result_failure_ctor = check_constructible<U, QualU>;

public:
    using base_type::base_type;

    result_base() = delete;
    result_base(const result_base& rhs) = default;
    result_base(result_base&& rhs)  = default;
    result_base& operator=(result_base const& e) = delete;
    result_base& operator=(result_base && e) = delete;
    ~result_base() = default;
#if 0
    constexpr result_base(failure<E> const& error) :
        base_type(in_place_type<failure<E>>, error)
    {}
    constexpr result_base(failure<E> && error) :
        base_type(in_place_type<failure<E>>, std::move(error))
    {}
#else
    template < class U = E, enable_if_t<
                    check_result_failure_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result_base(failure<U> const& other) :
    base_type(in_place_type<failure<E>>, other)
    {}
    template < class U = E, enable_if_t<
                    check_result_failure_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result_base(failure<U> const& other) :
    base_type(in_place_type<failure<E>>, other)
    {}
    template < class U = E, enable_if_t<
                    check_result_failure_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result_base(failure<U> && other) :
    base_type(in_place_type<failure<E>>, std::move(other))
    {}
    template < class U = E, enable_if_t<
                    check_result_failure_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result_base(failure<U> && other) :
    base_type(in_place_type<failure<E>>, std::move(other))
    {}
#endif
    using base_type::has_value;
    using base_type::value;
    using base_type::get_success;
    using base_type::error;
    using base_type::get_failure;
};



}

template <class T, class E>
class result : public result_detail::result_base<T, E>
{
    static_assert(is_destructible<T>::value,
        "instantiation of result with a non-destructible non-void T type is ill-formed");

    using base_type = result_detail::result_base<T, E>;


    template <class U, class QualU>
    using check_result_success_ctor = result_detail::check_void_or_constructible<T, U, QualU>;

public:
    using base_type::base_type;

    template < class U = T, enable_if_t<
                    check_result_success_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(success<U> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(success<U> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(success<U> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(success<U> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}
};

template <class E>
class result<void, E> : public result_detail::result_base<void, E>
{
    using T = void;
    using base_type = result_detail::result_base<T, E>;

public:
    using base_type::base_type;

    constexpr result(success<void> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    constexpr result(success<void> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}
};

}
}
}
#endif
#endif // header
