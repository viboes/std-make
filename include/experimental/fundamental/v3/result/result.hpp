// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_RESULT_RESULT_HPP
#define JASEL_EXPERIMENTAL_V3_RESULT_RESULT_HPP

#include <experimental/fundamental/v2/config.hpp>

#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class T>
struct success {
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
        value(move(v))
    {
    }
};

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

template <class E>
struct failure {
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
                value(move(e))
    {
    }
};
namespace result_detail {

struct uninitialized_t {};

template <class T, class E, bool = is_trivially_destructible<T>::value && is_trivially_destructible<E>::value>
struct result_destruct_base;

template <class T, class E>
struct result_destruct_base<T, E, false>
{
    typedef T value_type;
    typedef success<T> success_type;
    typedef E error_type;
    typedef failure<E> failure_type;
    static_assert(is_object_v<value_type>,
        "instantiation of result with a non-object type is undefined behavior");
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
    static_assert(is_object_v<value_type>,
        "instantiation of result with a non-object type is undefined behavior");
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

template <class T, class E>
struct result_storage_base<T, E, true>
{
    //static_assert(false, "T can not be a reference");
};

template <class T, class E, bool = is_trivially_copy_constructible<T>::value && is_trivially_copy_constructible<E>::value>
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

template <class T, class E, bool = is_trivially_move_constructible<T>::value && is_trivially_move_constructible<E>::value>
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
        noexcept(is_nothrow_move_constructible_v<value_type> && is_nothrow_move_constructible_v<error_type>)
        : base_type(uninitialized_t{})
    {
        this->construct_from(std::move(other));
    }

};
template <class T, class E>
struct result_base : result_move_base<T, E>
{
    using result_move_base<T, E>::result_move_base;
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


template <class T, class E>
using result_sfinae_ctor_base = sfinae_ctor_base<
    is_copy_constructible<T>::value && is_copy_constructible<E>::value,
    is_move_constructible<T>::value && is_move_constructible<E>::value
>;

}

template <class T, class E>
class result : private result_detail::result_base<T,E>
             , private result_detail::result_sfinae_ctor_base<T,E>
{
    static_assert(is_destructible_v<T>,
        "instantiation of result with a non-destructible T type is ill-formed");
    static_assert(is_destructible_v<E>,
        "instantiation of result with a non-destructible E type is ill-formed");

    using base_type = result_detail::result_base<T, E>;

public:
    result() = delete;
    result(const result& rhs) = default;
    result(result&& rhs)  = default;
    result& operator=(result const& e) = delete;
    result& operator=(result && e) = delete;
    ~result() = default;

    constexpr result(success<T> const& value) :
        base_type(in_place_type<success<T>>, value)
    {}
    constexpr result(success<T> && value) :
        base_type(in_place_type<success<T>>, std::move(value))
    {}
    constexpr result(failure<E> const& error) :
        base_type(in_place_type<failure<E>>, error)
    {}
    constexpr result(failure<E> && error) :
        base_type(in_place_type<failure<E>>, std::move(error))
    {}

    using base_type::has_value;
    using base_type::value;
    using base_type::get_success;
    using base_type::error;
    using base_type::get_failure;
};

template <class E>
class result<void,E> {
    using T = void;
public:
    result() = delete;
    result(const result& rhs) = default;
    result(result&& rhs)  = default;
    result& operator=(result const& e) = delete;
    result& operator=(result && e) = delete;
    ~result() = default;

    constexpr result(success<void> const& value) :
        _has_value(true), _success(value)
    {}
    constexpr result(success<void> && value) :
        _has_value(true), _success(move(value))
    {}
    constexpr result(failure<E> const& error) :
        _has_value(false), _failure(error)
    {}
    constexpr result(failure<E> && error) :
        _has_value(false), _failure(move(error))
    {}

    constexpr bool has_value() const { return _has_value; }

    constexpr void value() const  { }

    constexpr success<T> const& get_success() const & { return _success; }
    constexpr success<T> && get_success() const && { return move(_success); }

    constexpr E const& error() const & { return _failure.value; }
    constexpr E && error() const && { return move(_failure.value); }
    constexpr failure<E> const& get_failure() const & { return _failure; }
    constexpr failure<E> && get_failure() const && { return move(_failure); }

private:
    const bool _has_value;
    union {
        success<T> _success;
        failure<E> _failure;
    };
};

}
}
}
#endif // header
