// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_RESULT_UNEXPECTED_HPP
#define JASEL_EXPERIMENTAL_V3_RESULT_UNEXPECTED_HPP

#include <experimental/fundamental/v2/config.hpp>
#if 0
#include <experimental/fundamental/v3/result/immutable_wrapper.hpp>
#endif
#include <experimental/fundamental/v3/result/helpers_detail.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <type_traits>
#include <utility>
#include <experimental/fundamental/v3/in_place.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

#if 0


struct unexpected_tag {};

template <class T>
struct unexpected : immutable_wrapper<unexpected_tag, T>
{
    template <class U>
    using rebind = unexpected<U>;

    using immutable_wrapper<unexpected_tag, T>::immutable_wrapper;
};

template <class E>
unexpected<typename std::decay<E>::type> make_unexpected(E&& error)
{
    return unexpected<typename std::decay<E>::type>(std::forward<E>(error));
}

#else

template <class T>
struct unexpected
{
private:
    template <class U>
    struct T_is_not_unexpected_like {
        static constexpr bool value =
            ! is_constructible<T, unexpected<U>&>::value &&
            ! is_constructible<T, unexpected<U>&&>::value &&
            ! is_constructible<T, const unexpected<U>&>::value &&
            ! is_constructible<T, const unexpected<U>&&>::value &&
            ! is_convertible<unexpected<U>&, T>::value &&
            ! is_convertible<unexpected<U>&&, T>::value &&
            ! is_convertible<const unexpected<U>&, T>::value &&
            ! is_convertible<const unexpected<U>&&, T>::value;
    };
    template <class U, class QualU>
    using check_unexpected_ctor = helpers_detail::check_if_constructible_else_fail<
                T_is_not_unexpected_like<U>::value,
                T, QualU>;
public:
    static_assert(is_object<T>::value,
                  "instantiation of unexpected with a non-object type is ill defined");
    static_assert(! std::is_array<T>::value,
                  "instantiation of unexpected with a array type is ill defined");

    T value;

    template <class U>
    using rebind = unexpected<U>;

    constexpr unexpected() = default;
    constexpr unexpected(unexpected const& e) = default;
    constexpr unexpected(unexpected&& e) = default;
    JASEL_CXX14_CONSTEXPR unexpected& operator=(unexpected const& e) = default;
    JASEL_CXX14_CONSTEXPR unexpected& operator=(unexpected&& e) = default;

    JASEL_0_REQUIRES(
          is_copy_constructible<T>::value
    )
    constexpr explicit unexpected(T const& v) :
        value(v)
    {
    }
    JASEL_0_REQUIRES(
          is_move_constructible<T>::value
    )
    constexpr explicit unexpected(T&& v) :
        value(std::move(v))
    {
    }

    template <class... _Args, class = typename enable_if<
        is_constructible<T, _Args...>::value>::type
    >
    constexpr explicit unexpected(in_place_t, _Args&&... __args)
        : value(std::forward<_Args>(__args)...) {}

    template < class U = T, typename enable_if<
                    check_unexpected_ctor<U, U const&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr unexpected(unexpected<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_unexpected_ctor<U, U const&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr unexpected(unexpected<U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_unexpected_ctor<U, U &&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr unexpected(unexpected<U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, typename enable_if<
                    check_unexpected_ctor<U, U &&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr unexpected(unexpected<U> && other) :
        value(std::move(other.value))
    {}

};

template <class E1, class E2>
constexpr bool operator==(const unexpected<E1>& x, const unexpected<E2>& y)
{
    static_assert(is_convertible<decltype(x.value == y.value), bool>::value, "");
    return x.value == y.value;
}
template <class E1, class E2>
constexpr bool operator!=(const unexpected<E1>& x, const unexpected<E2>& y)
{

    static_assert(is_convertible<decltype(x.value != y.value), bool>::value, "");
    return x.value != y.value;
}

template <>
struct unexpected<void>
{

    constexpr unexpected() = default;
    constexpr unexpected(unexpected const& e) = default;
    constexpr unexpected(unexpected&& e) = default;
    JASEL_CXX14_CONSTEXPR unexpected& operator=(unexpected const& e) = default;
    JASEL_CXX14_CONSTEXPR unexpected& operator=(unexpected&& e) = default;

    template <class U>
    using rebind = unexpected<U>;
};

inline constexpr bool operator==(const unexpected<void>& x, const unexpected<void>& y)
{
  return true;
}
inline constexpr bool operator!=(const unexpected<void>& x, const unexpected<void>& y)
{
  return false;
}

template <class T>
unexpected<typename std::decay<T>::type> make_unexpected(T&& value)
{
    return unexpected<typename std::decay<T>::type>(std::forward<T>(value));
}


inline unexpected<void> make_unexpected()
{
    return unexpected<void>();
}
#endif

// CTAD
#if __cplusplus > 201402L && defined __clang__
template <class T>
explicit unexpected(T a) -> unexpected<T>;
explicit unexpected() -> unexpected<void>;
#endif

}
}
}
#endif // header
