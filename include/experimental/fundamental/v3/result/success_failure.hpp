// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_RESULT_SUCCESS_FAILURE_HPP
#define JASEL_EXPERIMENTAL_V3_RESULT_SUCCESS_FAILURE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/result/immutable_wrapper.hpp>
#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

struct success_tag {};

template <class T>
struct success : degenerated_immutable_wrapper<success_tag, T>
{
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
    using immutable_wrapper<failure_tag, T>::immutable_wrapper;
};

template <class E>
failure<typename std::decay<E>::type> make_failure(E&& error)
{
    return failure<typename std::decay<E>::type>(std::forward<E>(error));
}

#if __cplusplus > 201402L && defined __clang__
template <class T>
explicit success(T a) -> success<T>;
template <class T>
explicit failure(T a) -> failure<T>;
#endif

}
}
}
#endif // header
