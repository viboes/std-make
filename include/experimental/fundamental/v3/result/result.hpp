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
#include <experimental/fundamental/v3/result/helpers_detail.hpp>
#include <experimental/fundamental/v3/result/success_failure.hpp>

#include <type_traits>
#include <utility>
#include <memory>
#include <cassert>

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

template <class T, class E, bool = (is_void<T>::value || is_trivially_destructible<T>::value) && is_trivially_destructible<E>::value>
struct result_destruct_base;

template <class T, class E>
struct result_destruct_base<T, E, false>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;

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
};

template <class T, class E>
struct result_destruct_base<T, E, true>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;

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
};


template <class T, class E>
struct result_storage_common_base: result_destruct_base<T, E>
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

    constexpr const success_type& get_success() const&
    {
        assert(this->has_value() && "result must have a value");
        return this->_success;
    }
    constexpr const success_type&& get_success() const&&
    {
        assert(this->has_value() && "result must have a value");
        return std::move(this->_success);
    }
    constexpr success_type& get_success() &
    {
        assert(this->has_value() && "result must have a value");
        return this->_success;
    }
    constexpr success_type&& get_success() &&
    {
        assert(this->has_value() && "result must have a value");
        return std::move(this->_success);
    }

    constexpr const error_type& error() const&
    {
        assert(!this->has_value() && "result must have an error");
        return this->_failure.value;
    }
    constexpr const error_type&& error() const&&
    {
        assert(!this->has_value() && "result must have an error");
        return std::move(this->_failure.value);
    }
    constexpr error_type& error() &
    {
        assert(!this->has_value() && "result must have an error");
        return this->_failure.value;
    }
    constexpr error_type&& error() &&
    {
        assert(!this->has_value() && "result must have an error");
        return std::move(this->_failure.value);
    }
    constexpr const failure_type& get_failure() const&
    {
        assert(!this->has_value() && "result must have an error");
        return this->_failure;
    }
    constexpr const failure_type&& get_failure() const&&
    {
        assert(!this->has_value() && "result must have an error");
        return std::move(this->_failure);
    }
    constexpr failure_type& get_failure() &
    {
        assert(!this->has_value() && "result must have an error");
        return this->_failure;
    }
    constexpr failure_type&& get_failure() &&
    {
        assert(!this->has_value() && "result must have an error");
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

template <class T, class E, bool = is_reference<T>::value || is_reference<E>::value>
struct result_storage_base;

template <class T, class E>
struct result_storage_base<T, E, false>: result_storage_common_base<T, E>
{
    using base_type = result_storage_common_base<T, E>;
    using base_type::base_type;
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;

    constexpr const value_type& value() const&
    {
        assert(this->has_value() && "result must have a value");
        return this->_success.value;
        //return value_impl(*this);
    }
    constexpr const value_type&& value() const&&
    {
        assert(this->has_value() && "result must have a value");
        return std::move(this->_success.value);
        //return value_impl(std::move(*this));
    }
    constexpr value_type& value() &
    {
        assert(this->has_value() && "result must have a value");
        return this->_success.value;
        //return value_impl(*this);
        // error: non-const lvalue reference to type 'xxx' (aka 'int') cannot bind to a temporary of type 'int'
        // fixme: why this doesn't work. See p0847r0
    }
    constexpr value_type&& value() &&
    {
        assert(this->has_value() && "result must have a value");
        return std::move(this->_success.value);
        //return value_impl(std::move(*this));
    }
private:
    template <typename Self>
    static decltype(auto) value_impl(Self&& self)
    {
        assert(self.has_value() && "result must have a value");
        return std::forward<Self>(self)._success.value;
    }
};

template <class E>
struct result_storage_base<void, E, false>: result_storage_common_base<void, E>
{
    using base_type = result_storage_common_base<void, E>;
    using base_type::base_type;

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
struct result_storage_base<T, E, true>
{
    //static_assert(false, "Neither T nor E cannot be reference types");
};

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

template <class T, class E>
using result_sfinae_ctor_base = helpers_detail::sfinae_ctor_base<
                is_copy_constructible_or_void<T>::value && is_copy_constructible<E>::value,
                is_move_constructible_or_void<T>::value && is_move_constructible<E>::value
>;

template <class T, class E>
class result_base : protected result_move_base<T,E>
             , private result_sfinae_ctor_base<T,E>
{
    static_assert(is_destructible<T>::value || is_void<T>::value,
        "instantiation of result with a non-destructible non-void T type is ill-formed");
    static_assert(is_destructible<E>::value,
        "instantiation of result with a non-destructible E type is ill-formed");

    using base_type = result_detail::result_move_base<T, E>;

    template <class U, class QualU>
    using check_result_failure_ctor = helpers_detail::check_constructible<U, QualU>;

public:
    using base_type::base_type;

    result_base() = delete;
    result_base(const result_base& rhs) = default;
    result_base(result_base&& rhs)  = default;
    result_base& operator=(result_base const& e) = delete;
    result_base& operator=(result_base && e) = delete;
    ~result_base() = default;

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
    using base_type = result_detail::result_base<T, E>;

    template <class QualU>
    using check_result_success_ctor = helpers_detail::check_void_or_constructible<T, QualU>;

    template <class U, class G, class QualU, class QualG>
    using check_result_result_ctor = helpers_detail::check_diffs_or_constructibles<T, E, U, G, QualU, QualG>;

public:
    using base_type::base_type;

    template < class U = T, enable_if_t<
                    check_result_success_ctor<U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(success<U> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(success<U> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(success<U> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}
    template < class U = T, enable_if_t<
                    check_result_success_ctor<U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(success<U> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}




    template < class U = T, class G = E, enable_if_t<
                    check_result_result_ctor<U, G, U const&, G const&>::template enable_implicit<U, G>()
                , int> = 0>
    constexpr result(result<U, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class U = T, class G = E, enable_if_t<
                    check_result_result_ctor<U, G, U const&, G const&>::template enable_explicit<U, G>()
                , int> = 0>
    explicit constexpr result(result<U, G> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class U = T, class G = E, enable_if_t<
                    check_result_result_ctor<U, G, U &&, G&&>::template enable_implicit<U, G>()
                , int> = 0>
    constexpr result(result<U, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }
    template < class U = T, class G = E, enable_if_t<
                    check_result_result_ctor<U, G, U &&, G&&>::template enable_explicit<U, G>()
                , int> = 0>
    explicit constexpr result(result<U, G> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }

};

template <class E>
class result<void, E> : public result_detail::result_base<void, E>
{
    using T = void;
    using base_type = result_detail::result_base<T, E>;

    template <class QualU>
    using check_result_result_ctor = helpers_detail::check_constructible<E, QualU>;

public:
    using base_type::base_type;

    constexpr result(success<void> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    constexpr result(success<void> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}

    template < class U = E, enable_if_t<
                    check_result_result_ctor<U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(result<void, U> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class U = E, enable_if_t<
                    check_result_result_ctor<U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(result<void, U> const& other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(other);
        }
    template < class U = E, enable_if_t<
                    check_result_result_ctor<U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr result(result<void, U> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }
    template < class U = E, enable_if_t<
                    check_result_result_ctor<U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr result(result<void, U> && other)
    : base_type(uninitialized_t{})
        {
            this->construct_from(std::move(other));
        }

};


template <class T1, class E1, class T2, class E2>
constexpr bool operator==(const result<T1, E1>& x, const result<T2, E2>& y)
{
  return (x.has_value() && y.has_value())
    ? x.get_success() == y.get_success()
    : (!x.has_value() && !y.has_value())
      ?  x.get_failure() == y.get_failure()
      : false;
}

template <class T1, class E1, class T2, class E2>
constexpr bool operator!=(const result<T1, E1>& x, const result<T2, E2>& y)
{
  return !(x == y);
}

template <class T, class E, class G>
  constexpr bool operator==(const result<T, E>& x, const success<G>& y)
  {
    return ! x.has_value() ? false : x.get_success() == y;
  }
template <class G, class T, class E>
  constexpr bool operator==(const success<G>& x, const result<T, E>& y)
  {
    return y == x;
  }
template <class T, class E, class G>
  constexpr bool operator!=(const result<T, E>& x, const success<G>& y)
  {
    return ! x.has_value() ? true : x.get_success() != y;
  }
template <class G, class T, class E>
  constexpr bool operator!=(const success<G>& x, const result<T, E>& y)
  {
    return y != x;
  }

template <class T, class E, class G>
  constexpr bool operator==(const result<T, E>& x, const failure<G>& y)
  {
    return x.has_value() ? false : x.get_failure() == y;
  }
template <class G, class T, class E>
  constexpr bool operator==(const failure<G>& x, const result<T, E>& y)
  {
    return y == x;
  }
template <class T, class E, class G>
  constexpr bool operator!=(const result<T, E>& x, const failure<G>& y)
  {
    return x.has_value() ? true : x.get_failure() != y;
  }
template <class G, class T, class E>
  constexpr bool operator!=(const failure<G>& x, const result<T, E>& y)
  {
    return y != x;
  }

}
}
}
#endif
#endif // header
