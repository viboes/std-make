
//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_A3PP_CONST_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_A3PP_CONST_CAST_HPP

#include <utility> // forward, enable_if

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

// This is no more than a `const_cast`, that convey a tag telling why this `const_cast` is needed.
// This tag is usually the name of the library that forces you to do this `const_cast` as you cannot change the interface.
// Sometimes the tag represents a part of your legacy product, but that changing it now would be to risky or expensive to do.
// Ideally, you should wrap the interface avoiding the cast to in the application and use this specific `a3pp_const_cast` in the implementation of this adaptor.
// Example:
// <code>
// char* oldxmlfct(char*);
// const char *param;
// auto str = oldxmlfct(const_cast<char*>(param)); // Why?
// auto str = oldxmlfct(a3pp_const_cast<libxml,char*>(param)); // explicit, but noisy
// auto str = xml::fct(param); // No need for noise at this level of old library issues.
// //where
// namespace xml {
// 	char const *fct(char const *param) {
//         return oldxmlfct(a3pp_const_cast<libxml, char *>(param));
//     }
// }
// <endcode>
template <class Tag, class T>
constexpr T *a3pp_const_cast(T const *ptr) noexcept
{
	return const_cast<T *>(ptr);
}

template <class Tag, class T>
constexpr T &a3pp_const_cast(T const &ptr) noexcept
{
	return const_cast<T &>(ptr);
}

// todo: Move this to a specific file
// This is a low level cast, used to work with chars
// We have found the need for this while using a libxml that uses `unsigned char` as type for the characters.
template <class T>
constexpr char *char_ptr_cast(T *ptr) noexcept
{
	return reinterpret_cast<char *>(ptr);
}
template <class T>
constexpr char const *char_ptr_cast(T const *ptr) noexcept
{
	return reinterpret_cast<char const *>(ptr);
}

template <class T>
constexpr unsigned char *unsigned_char_ptr_cast(T *ptr) noexcept
{
	return reinterpret_cast<unsigned char *>(ptr);
}
template <class T>
constexpr unsigned char const *unsigned_char_ptr_cast(T const *ptr) noexcept
{
	return reinterpret_cast<unsigned char const *>(ptr);
}

// todo: add a cast o convert to bytes when working at the memory byte level.
// In C/C++ any pointer is convertible to a void*, but not to a char*.
// Any pointer is convertible to a `byte*`, but waiting for that, we need a way to
// state explicitly that we are forcing this kind of in the future implicit conversions.
// e.g. a send function can be defined as
// void send(const byte*, int size);
// void send(const void*, int size);

// The first one will require you to do an explicit cast and states explicitly that you are working at the `byte` level.
// The second one supports the implicit construction, and hides this fact.
// Very often, when we want to work at the byte level, is because we have instances of trivial types, that will accept to be copied.
// An alternative is to define a function that state this more explicitly
// template <class Trivial>
// requires Trivial is a trivial type
// void send(Trivial const* mem, int size);
// This interface is easy to use well and hard to use bad.
// The void* interface accepts too much things
// The char* interface requires explicit casts at the application level
// Requiring to have templates at this level is not always desirable.
// In order to avoid the void* interface and the template, we should have some kind of trivial erased interface
// void send(trivial_erased_const_ptr mem, int size);
// trivial_erased_const_ptr will be implicitly convertible from any `T const*` where `Trivial(T)`.


} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
