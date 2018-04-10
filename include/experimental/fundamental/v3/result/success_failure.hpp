// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_RESULT_SUCCESS_FAILURE_HPP
#define JASEL_EXPERIMENTAL_V3_RESULT_SUCCESS_FAILURE_HPP

#include <experimental/fundamental/v2/config.hpp>
#if 0
#include <experimental/fundamental/v3/result/immutable_wrapper.hpp>
#endif
#include <experimental/fundamental/v3/result/helpers_detail.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <type_traits>
#include <utility>
#if __cplusplus <= 201402L || ! defined __clang__
#include <experimental/fundamental/v3/in_place.hpp>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if 0
struct success_tag {};

template <class T>
struct success : degenerated_immutable_wrapper<success_tag, T>
{
    template <class U>
    using rebind = success<U>;

    using degenerated_immutable_wrapper<success_tag, T>::degenerated_immutable_wrapper;
};

template <class T>
success<typename std::decay<T>::type> make_success(T&& value)
{
    return success<typename std::decay<T>::type>(std::forward<T>(value));
}

success<void> make_success()
{
    return success<void>();
}

struct failure_tag {};

template <class T>
struct failure : immutable_wrapper<failure_tag, T>
{
    template <class U>
    using rebind = failure<U>;

    using immutable_wrapper<failure_tag, T>::immutable_wrapper;
};

template <class E>
failure<typename std::decay<E>::type> make_failure(E&& error)
{
    return failure<typename std::decay<E>::type>(std::forward<E>(error));
}
#else


template <class T>
struct success
{
private:
    template <class U>
    struct T_is_not_success_like {
        static constexpr bool value =
            ! is_constructible<T, success<U>&>::value &&
            ! is_constructible<T, success<U>&&>::value &&
            ! is_constructible<T, const success<U>&>::value &&
            ! is_constructible<T, const success<U>&&>::value &&
            ! is_convertible<success<U>&, T>::value &&
            ! is_convertible<success<U>&&, T>::value &&
            ! is_convertible<const success<U>&, T>::value &&
            ! is_convertible<const success<U>&&, T>::value;
    };

    template <class U, class QualU>
    using check_success_ctor = helpers_detail::check_if_constructible_else_fail<
                    T_is_not_success_like<U>::value,
                    T, QualU>;
public:
    static_assert(is_object<T>::value,
        "instantiation of success with a non-object type is ill defined");
    static_assert(! std::is_array<T>::value,
        "instantiation of success with a array type is ill defined");

    template <class U>
    using rebind = success<U>;

    T value;

    constexpr success() : value() {};
    constexpr success(success const& e) = default;
    constexpr success(success&& e) = default;
    JASEL_CXX14_CONSTEXPR success& operator=(success const& e) = default;
    JASEL_CXX14_CONSTEXPR success& operator=(success&& e) = default;

    JASEL_0_REQUIRES(
          is_copy_constructible<T>::value
    )
    constexpr explicit success(T const& v) :
        value(v)
    {
    }
    JASEL_0_REQUIRES(
          is_move_constructible<T>::value
    )
    constexpr explicit success(T&& v) :
        value(std::move(v))
    {
    }

    template <class... _Args, class = typename enable_if<
        is_constructible<T, _Args...>::value>::type
    >
    constexpr explicit success(in_place_t, _Args&&... __args)
        : value(std::forward<_Args>(__args)...) {}

    template < class U = T, typename enable_if<
                    check_success_ctor<U, U const&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr success(success<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U const&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr success(success<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U &&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr success(success<U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U &&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr success(success<U> && other) :
        value(std::move(other.value))
    {}
};

template <class E1, class E2>
constexpr bool operator==(const success<E1>& x, const success<E2>& y)
{
  static_assert(is_convertible<decltype(x.value == y.value), bool>::value, "");
  return x.value == y.value;
}

template <class E1, class E2>
constexpr bool operator!=(const success<E1>& x, const success<E2>& y)
{
  static_assert(is_convertible<decltype(x.value != y.value), bool>::value, "");
  return x.value != y.value;
}

template <>
struct success<void>
{
    constexpr success() = default;
    constexpr success(success const& e) = default;
    constexpr success(success&& e) = default;
    JASEL_CXX14_CONSTEXPR success& operator=(success const& e) = default;
    JASEL_CXX14_CONSTEXPR success& operator=(success&& e) = default;

    constexpr explicit success(in_place_t) {}

    template <class U>
    using rebind = success<U>;
};

inline constexpr bool operator==(const success<void>& x, const success<void>& y)
{
  return true;
}
inline constexpr bool operator!=(const success<void>& x, const success<void>& y)
{
  return false;
}

template <class T>
success<typename std::decay<T>::type> make_success(T&& value)
{
    return success<typename std::decay<T>::type>(std::forward<T>(value));
}

inline success<void> make_success()
{
    return success<void>();
}

template <class T>
struct failure
{
private:
    template <class U>
    struct T_is_not_failure_like {
        static constexpr bool value =
            ! is_constructible<T, failure<U>&>::value &&
            ! is_constructible<T, failure<U>&&>::value &&
            ! is_constructible<T, const failure<U>&>::value &&
            ! is_constructible<T, const failure<U>&&>::value &&
            ! is_convertible<failure<U>&, T>::value &&
            ! is_convertible<failure<U>&&, T>::value &&
            ! is_convertible<const failure<U>&, T>::value &&
            ! is_convertible<const failure<U>&&, T>::value;
    };
    template <class U, class QualU>
    using check_failure_ctor = helpers_detail::check_if_constructible_else_fail<
                T_is_not_failure_like<U>::value,
                T, QualU>;
public:
    static_assert(is_object<T>::value,
                  "instantiation of failure with a non-object type is ill defined");
    static_assert(! std::is_array<T>::value,
                  "instantiation of failure with a array type is ill defined");

    T value;

    template <class U>
    using rebind = failure<U>;

    constexpr failure() = default;
    constexpr failure(failure const& e) = default;
    constexpr failure(failure&& e) = default;
    JASEL_CXX14_CONSTEXPR failure& operator=(failure const& e) = default;
    JASEL_CXX14_CONSTEXPR failure& operator=(failure&& e) = default;

    JASEL_0_REQUIRES(
          is_copy_constructible<T>::value
    )
    constexpr explicit failure(T const& v) :
        value(v)
    {
    }
    JASEL_0_REQUIRES(
          is_move_constructible<T>::value
    )
    constexpr explicit failure(T&& v) :
        value(std::move(v))
    {
    }

    template <class... _Args, class = typename enable_if<
        is_constructible<T, _Args...>::value>::type
    >
    constexpr explicit failure(in_place_t, _Args&&... __args)
        : value(std::forward<_Args>(__args)...) {}

    template < class U = T, typename enable_if<
                    check_failure_ctor<U, U const&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr failure(failure<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_failure_ctor<U, U const&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr failure(failure<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_failure_ctor<U, U &&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr failure(failure<U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, typename enable_if<
                    check_failure_ctor<U, U &&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr failure(failure<U> && other) :
        value(std::move(other.value))
    {}

};

template <class E1, class E2>
constexpr bool operator==(const failure<E1>& x, const failure<E2>& y)
{
    static_assert(is_convertible<decltype(x.value == y.value), bool>::value, "");
    return x.value == y.value;
}
template <class E1, class E2>
constexpr bool operator!=(const failure<E1>& x, const failure<E2>& y)
{

    static_assert(is_convertible<decltype(x.value != y.value), bool>::value, "");
    return x.value != y.value;
}

template <>
struct failure<void>
{

    constexpr failure() = default;
    constexpr failure(failure const& e) = default;
    constexpr failure(failure&& e) = default;
    JASEL_CXX14_CONSTEXPR failure& operator=(failure const& e) = default;
    JASEL_CXX14_CONSTEXPR failure& operator=(failure&& e) = default;

    constexpr explicit failure(in_place_t) {}

    template <class U>
    using rebind = failure<U>;
};

inline constexpr bool operator==(const failure<void>& x, const failure<void>& y)
{
  return true;
}
inline constexpr bool operator!=(const failure<void>& x, const failure<void>& y)
{
  return false;
}

template <class T>
failure<typename std::decay<T>::type> make_failure(T&& value)
{
    return failure<typename std::decay<T>::type>(std::forward<T>(value));
}


inline failure<void> make_failure()
{
    return failure<void>();
}
#endif

// CTAD
#if __cplusplus > 201402L && defined __clang__
template <class T>
explicit success(T a) -> success<T>;
explicit success() -> success<void>;
template <class T>
explicit failure(T a) -> failure<T>;
explicit failure() -> failure<void>;
#endif

}
}
}
#endif // header
