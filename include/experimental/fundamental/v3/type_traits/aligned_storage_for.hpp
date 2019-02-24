//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

//see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1413r0.pdf

#ifndef JASEL_STD_ALIGNED_STORAGE_FOR_HPP
#define JASEL_STD_ALIGNED_STORAGE_FOR_HPP

// fixme: adapt as soon as this trait is implemented
#if __cplusplus <= 202303L

#include <type_traits>

namespace std
{

template <typename T>
using aligned_storage_for = aligned_storage<sizeof(T), alignof(T)>;
template <typename T>
using aligned_storage_for_t = typename std::aligned_storage_for<T>::type;

} // namespace std

#endif
#endif // header
