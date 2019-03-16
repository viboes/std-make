
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018-2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_CONVERT_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_CONVERT_CAST_HPP

#include <type_traits> // is_pointer
#include <utility>     // forward, enable_if

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// The question is if we should use `T(u)` or `T{u}` instead of `(T)u` or `static_cast<T>(u)` when
// `T` is the name of a class and is explicitly convertible from `U`.
// While `T(u)` or `T{u}` looks more C++-ish, these are not easy to grep neither.
// `static_cast` can be used for more things than for converting between types.
// When you want to state clearly that you are calling to the explicit conversion constructor or conversion operator and you
// want to have a way to grep easily use `convert_cast<T>`.
// Note that depending on the scope you will need to look for `convert_cast<NS::T>` or `convert_cast<T>` or something else.
// So this is not yet a panacea.
// Possible name alternatives - `convert<T>`, `convert_to<T>`, `cast<T>`, `to<T>`.
template <class T, class U, typename = typename enable_if<!is_pointer<T>::value && !is_reference<T>::value>::type>
T convert_cast(U &&u)
{
	return static_cast<T>(std::forward<U>(u));
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
