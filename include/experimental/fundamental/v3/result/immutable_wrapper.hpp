// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_IMMUTABLE_WRAPPER_HPP
#define JASEL_EXPERIMENTAL_V3_IMMUTABLE_WRAPPER_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/result/helpers_detail.hpp>

#include <type_traits>
#include <utility>
#include <memory>
#include <cassert>

#if __cplusplus <= 201402L || ! defined __clang__
#include <experimental/fundamental/v3/in_place.hpp>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class Tag, class T>
struct immutable_wrapper {
private:
    template <class U, class QualU>
    using check_success_ctor = helpers_detail::check_diff_or_constructible<T, U, QualU>;
public:
    static_assert(is_object<T>::value,
        "instantiation of immutable_wrapper with a non-object type is ill defined");
    static_assert(! std::is_array<T>::value,
        "instantiation of immutable_wrapper with a array type is ill defined");

    T value;

    immutable_wrapper() = delete;
    constexpr immutable_wrapper(immutable_wrapper const& e) = default;
    constexpr immutable_wrapper(immutable_wrapper&& e) = default;
    constexpr immutable_wrapper& operator=(immutable_wrapper const& e) = delete;
    constexpr immutable_wrapper& operator=(immutable_wrapper&& e) = delete;

    constexpr explicit immutable_wrapper(T const& v) :
        value(v)
    {
    }
    constexpr explicit immutable_wrapper(T&& v) :
        value(std::move(v))
    {
    }

    template <class... _Args, class = typename enable_if<
        is_constructible<T, _Args...>::value>::type
    >
    constexpr explicit immutable_wrapper(in_place_t, _Args&&... __args)
        : value(std::forward<_Args>(__args)...) {}

    template < class U = T, typename enable_if<
                    check_success_ctor<U, U const&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr immutable_wrapper(immutable_wrapper<Tag, U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U const&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr immutable_wrapper(immutable_wrapper<Tag, U> const& other) :
        value(other.value)
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U &&>::template enable_implicit<U>()
                , int>::type = 0>
    constexpr immutable_wrapper(immutable_wrapper<Tag, U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, typename enable_if<
                    check_success_ctor<U, U &&>::template enable_explicit<U>()
                , int>::type = 0>
    explicit constexpr immutable_wrapper(immutable_wrapper<Tag, U> && other) :
        value(std::move(other.value))
    {}
};

template <class Tag, class E1, class E2>
BOOST_CONSTEXPR bool operator==(const immutable_wrapper<Tag, E1>& x, const immutable_wrapper<Tag, E2>& y)
{
  static_assert(is_convertible<decltype(x.value == y.value), bool>::value, "");
  return x.value == y.value;
}
template <class Tag, class E1, class E2>
BOOST_CONSTEXPR bool operator!=(const immutable_wrapper<Tag, E1>& x, const immutable_wrapper<Tag, E2>& y)
{
  static_assert(is_convertible<decltype(x.value != y.value), bool>::value, "");
  return x.value != y.value;
}

template <class Tag, class T>
void swap(immutable_wrapper<Tag, T>& x, immutable_wrapper<Tag, T>& y)
#if __cplusplus > 201402L && defined __clang__
    noexcept(std::is_nothrow_move_constructible_v<T> && std::is_nothrow_swappable_v<T>)
#endif
{
    using std::swap;
    swap(x.value, y.value);
}

template <class Tag, class T>
struct degenerated_immutable_wrapper : immutable_wrapper<Tag, T>
{
        using immutable_wrapper<Tag, T>::immutable_wrapper;
};

template <class Tag>
struct degenerated_immutable_wrapper<Tag, void> {

    constexpr degenerated_immutable_wrapper(degenerated_immutable_wrapper const& e) = default;
    constexpr degenerated_immutable_wrapper(degenerated_immutable_wrapper&& e) = default;
    constexpr degenerated_immutable_wrapper& operator=(degenerated_immutable_wrapper const& e) = delete;
    constexpr degenerated_immutable_wrapper& operator=(degenerated_immutable_wrapper&& e) = delete;

    constexpr explicit degenerated_immutable_wrapper()
    {
    }
};

template <class Tag, class E1, class E2>
BOOST_CONSTEXPR bool operator==(const degenerated_immutable_wrapper<Tag, E1>& x, const degenerated_immutable_wrapper<Tag, E2>& y)
{
  static_assert(is_convertible<decltype(x.value == y.value), bool>::value, "");
  return x.value == y.value;
}
template <class Tag, class E1, class E2>
BOOST_CONSTEXPR bool operator!=(const degenerated_immutable_wrapper<Tag, E1>& x, const degenerated_immutable_wrapper<Tag, E2>& y)
{
  static_assert(is_convertible<decltype(x.value != y.value), bool>::value, "");
  return x.value != y.value;
}

template <class Tag>
BOOST_CONSTEXPR bool operator==(const degenerated_immutable_wrapper<Tag, void>& x, const degenerated_immutable_wrapper<Tag, void>& y)
{
  return true;
}
template <class Tag>
BOOST_CONSTEXPR bool operator!=(const degenerated_immutable_wrapper<Tag, void>& x, const degenerated_immutable_wrapper<Tag, void>& y)
{
  return false;
}

template <class Tag>
void swap(degenerated_immutable_wrapper<Tag, void>& x, degenerated_immutable_wrapper<Tag, void>& y)
    noexcept
{
}

}
}
}
#endif // header
