//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <utility> // enable_if

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_TYPE_ERASE_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_TYPE_ERASE_CAST_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// Intead of static_cast<T*>, use type_erase_cast only when converting a void* to another pointer T*.
// erased_ptr_cast has the advantage over static_cast in that it conveys more explicitly the type-erasure danger,
// which static_cast hides, as static_cast is considered as safe, even if it s not in this case.
// Should be replaced by a more safe way, some kind of any_ptr when the additional performaces are reasonable.
// (see http://www.drdobbs.com/cpp/twisting-the-rtti-system-for-safe-dynami/229401004 and https://github.com/scottslacksmith/any_ptr#any_ptr), tracking the stored pointer type
// An alternative when we know all the pointer types is to use variant std::variant<T1*, ..., Tn*>.
// We could have a variant_ptr<T1, ..., Tn> with a more appropriated interface or even variant_non_null_ptr<T1, ..., Tn>
template <class TPtr, class U, typename = typename enable_if<is_void<U>::value && is_pointer<TPtr>::value>::type>
TPtr erased_ptr_cast(U *u)
{
	return static_cast<TPtr>(u);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
