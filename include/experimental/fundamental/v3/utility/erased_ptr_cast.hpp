//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2019.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#include <experimental/type_traits.hpp>
#include <utility> // enable_if

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_TYPE_ERASE_CAST_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_TYPE_ERASE_CAST_HPP

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
// Intead of `static_cast<T*>`, use `erased_ptr_cast` when converting a non polymorphic `Base*` (`Base const*`)
// to another pointer `Derived*` (`Derived const*`). where the base class is used only to erase the type a little bit, not as a base interface.
// `erased_ptr_cast` has the advantage over `static_cast` in that it conveys more explicitly the type-erasure danger,
// which `static_cast` hides, as `static_cast` is considered as safe, even if it is not in this case.
// When the additional performaces are reasonable,
// the design could be replaced by a more safe design, some kind of `erased_ptr` tracking the stored pointer type
// (see http://www.drdobbs.com/cpp/twisting-the-rtti-system-for-safe-dynami/229401004
// and https://github.com/scottslacksmith/any_ptr#any_ptr).
// The difference between void_ptr and `erased_ptr<Base>` is that we require that all the pointee classes must inherit from a base class.
// In some way, `void_ptr` is the same as `erased_ptr<void>` if we interpret that inheriting from void is inheriting from nothing.
// When the number of possible types is known in advance an alternative is to use
// variant std::variant<T1*, ..., Tn*>.
// We could have a variant_ptr<T1, ..., Tn> with a more appropriated interface or even variant_non_null_ptr<T1, ..., Tn>
// how is erased_ptr_cast different from static_downcast?
template <
        class TPtr,
        class U,
        typename = enable_if_t<
                not is_void<U>::value &&
                is_pointer<TPtr>::value &&
                is_base_of<remove_cv_t<U>, remove_cv_t<remove_pointer_t<TPtr>>>::value &&
                (is_const<remove_pointer_t<TPtr>>::value == is_const<U>::value)>>

TPtr erased_ptr_cast(U *u)
{
	return static_cast<TPtr>(u);
}
// Do we need a erased_ref_cast
template <
        class TRef,
        class U,
        typename = enable_if_t<
                not is_void<U>::value &&
                is_lvalue_reference<TRef>::value &&
                not is_lvalue_reference<U>::value &&
                is_base_of<remove_cv_t<U>, remove_cv_t<remove_reference_t<TRef>>>::value &&
                (is_const<remove_reference_t<TRef>>::value == is_const<U>::value)>>

TRef erased_ref_cast(U &u)
{
	return static_cast<TRef>(u);
}

template <
        class TPtr,
        class U,
        typename = enable_if_t<
                not is_void<U>::value &&
                is_pointer<TPtr>::value &&
                is_base_of<remove_cv_t<U>, remove_cv_t<remove_pointer_t<TPtr>>>::value &&
                (is_const<remove_pointer_t<TPtr>>::value == is_const<U>::value)>>
TPtr static_downcast(U *u)
{
	return static_cast<TPtr>(u);
}

// Intead of `static_cast<T*>`, use `any_ptr_cast` when converting a `void*` (`void const*`)
// to another pointer `T*` (`T const*`).
// `any_ptr_cast` has the advantage over `static_cast` in that it conveys more explicitly the type-erasure danger,
// which `static_cast` hides, as `static_cast` is considered by some as safe, even if it is not in this case.
// The design using this function could be replaced by a more safe design,
// some kind of `any_ptr` when the additional cost is reasonable
// (see http://www.drdobbs.com/cpp/twisting-the-rtti-system-for-safe-dynami/229401004
// and https://github.com/scottslacksmith/any_ptr#any_ptr), tracking the stored pointer type
// An alternative when we know all the pointer types is to use variant std::variant<T1*, ..., Tn*>.
// We could have a variant_ptr<T1, ..., Tn> with a more appropriated interface or even variant_non_null_ptr<T1, ..., Tn>

template <
        class TPtr,
        class U,
        typename = enable_if_t<
                is_void<U>::value &&
                is_pointer<TPtr>::value &&
                (is_const<remove_pointer_t<TPtr>>::value == is_const<U>::value)>>
TPtr any_ptr_cast(U *u)
{
	return static_cast<TPtr>(u);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
