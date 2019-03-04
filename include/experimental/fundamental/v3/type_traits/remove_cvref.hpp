//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_STD_REMOVE_CVREF_HPP
#define JASEL_STD_REMOVE_CVREF_HPP

// fixme: adapt as soon as this trait is implemented
#if (defined __clang__ && __cplusplus > 201703L)
#else

#include <type_traits> //remove_cv_t, remove_reference_t

namespace std
{
template <class T>
struct remove_cvref : remove_cv<typename remove_reference<T>::type>
{
};

template <class T>
using remove_cvref_t = typename remove_cvref<T>::type;
} // namespace std

#endif
#endif // header
