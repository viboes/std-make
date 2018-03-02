// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_IMMUTABLE_WRAPPER_HPP
#define JASEL_EXPERIMENTAL_V3_IMMUTABLE_WRAPPER_HPP

#if __cplusplus >= 201402L && defined __clang__

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/result/helpers_detail.hpp>

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

    template <class... _Args, class = enable_if_t<
        is_constructible<T, _Args...>::value>
    >
    _LIBCPP_INLINE_VISIBILITY
    constexpr explicit immutable_wrapper(in_place_t, _Args&&... __args)
        : value(std::forward<_Args>(__args)...) {}

    template < class U = T, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_implicit<U>()
                , int> = 0>
    constexpr immutable_wrapper(immutable_wrapper<Tag, U> const& other) :
        value(other.value)
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U const&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr immutable_wrapper(immutable_wrapper<Tag, U> const& other) :
        value(other.value)
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_implicit<U>()
                , int> = 0>
    constexpr immutable_wrapper(immutable_wrapper<Tag, U> && other) :
        value(std::move(other.value))
    {}
    template < class U = T, enable_if_t<
                    check_success_ctor<U, U &&>::template enable_explicit<U>()
                , int> = 0>
    explicit constexpr immutable_wrapper(immutable_wrapper<Tag, U> && other) :
        value(std::move(other.value))
    {}
};

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

}
}
}
#endif
#endif // header
