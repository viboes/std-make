//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_PRODUCT_TYPE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_PRODUCT_TYPE_HPP

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED

///////////////////////////////////////////////////////////////////////////////////////
/// @file
/// A type PT is a model of a ProductType if given variable pt of type PT
///
///   product_type::size<PT>::value
///   product_type::element<I,PT>::type
///   product_type::get<I>(pt)
///
/// The definition of these traits/functions depend on whether P0327 is adopted
/// by the standard and
/// can be implemented only by the compiler, as it does for Range-based for
/// loops.
///
/// What follows is just an emulation for the cases 1 (c-arrays) and 2
/// (tuple-like types).
/// The case 3 can be implemented only by the compiler.
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v3/utility/swallow.hpp>
#include <experimental/meta.hpp>
#include <experimental/type_traits.hpp>
#include <cstddef>
#include <tuple>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
#if !defined JASEL_DOXYGEN_INVOKED
namespace product_type_detail
{

template <class T>
struct has_tuple_like_size_access
{
	struct yes
	{
		char a[1];
	};
	struct no
	{
		char a[2];
	};

	template <class U>
	static yes test(decltype(std::tuple_size<U>::value) *);
	template <class U>
	static no         test(...);
	static const bool value = sizeof(test<T>(nullptr)) == sizeof(yes);
};

template <size_t N, class T>
struct has_tuple_like_element_access
{
	struct yes
	{
		char a[1];
	};
	struct no
	{
		char a[2];
	};

	template <size_t  I, class U>
	static yes        test(typename std::tuple_element<I, U>::type *);
	template <size_t  I, class U>
	static no         test(...);
	static const bool value = sizeof(test<N, T>(nullptr)) == sizeof(yes);
};
namespace get_adl
{
using std::get;
template <size_t N, class T>
constexpr auto xget(T &&t) noexcept -> decltype(get<N>(forward<T>(t)))
{
	return get<N>(forward<T>(t));
}
}
template <size_t N, class T>
struct has_tuple_like_get_access
{
	struct yes
	{
		char a[1];
	};
	struct no
	{
		char a[2];
	};

	template <size_t I, class U>
	static yes       test(remove_reference_t<decltype(
	                        product_type_detail::get_adl::xget<I>(declval<U>()))> *);
	template <size_t      I, class U>
	static no             test(...);
	static constexpr bool value = sizeof(test<N, T>(nullptr)) == sizeof(yes);
};
template <size_t I, size_t N, class T>
struct has_tuple_like_get_access<I, T[N]>
#if !defined JASEL_DOXYGEN_INVOKED
        : false_type
{
}
#endif
;

template <class T, class Indexes>
struct has_tuple_like_element_get_access_aux;
#if defined JASEL_HAS_FOLD_EXPRESSIONS
template <class T, size_t... N>
struct has_tuple_like_element_get_access_aux<T, index_sequence<N...>>
        : integral_constant<bool,
                            (has_tuple_like_element_access<N, T>::value && ...) &&
                                    (has_tuple_like_get_access<N, T>::value && ...)>
{
};
#else
template <class T, size_t... N>
struct has_tuple_like_element_get_access_aux<T, index_sequence<N...>>
        : conjunction<has_tuple_like_element_access<N, T>...,
                      has_tuple_like_get_access<N, T>...>
{
};
#endif

template <class T, bool B>
struct has_tuple_like_element_get_access
        : has_tuple_like_element_get_access_aux<
                  T, make_index_sequence<tuple_size<T>::value>>
{
};
template <class T>
struct has_tuple_like_element_get_access<T, false> : false_type
{
};
}
#endif
template <class T>
struct has_tuple_like_access
#if !defined JASEL_DOXYGEN_INVOKED
        : integral_constant<
                  bool,
                  product_type_detail::has_tuple_like_size_access<T>::value &&
                          product_type_detail::has_tuple_like_element_get_access<
                                  T, product_type_detail::has_tuple_like_size_access<
                                             T>::value>::value>
{
}
#endif
;

template <size_t N, class T>
struct has_tuple_like_access<T[N]>
#if !defined JASEL_DOXYGEN_INVOKED
        : false_type
{
}
#endif
;
template <size_t N, class T>
struct has_tuple_like_access<T (&)[N]>
#if !defined JASEL_DOXYGEN_INVOKED
        : false_type
{
}
#endif
;

namespace product_type
{

namespace detail {
    struct not_a_product_type_tag
    {};
}

template <class PT, class Enabler = void>
struct traits
#if !defined JASEL_DOXYGEN_INVOKED
        : traits<PT, meta::when<true>>
{
}
#endif
;

// Default failing specialization
template <class PT, bool condition>
struct traits<PT, meta::when<condition>> : detail::not_a_product_type_tag
{
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
	template <class T>
	static constexpr auto get(T &&x) = delete;
#endif
};

// Forward to customized class using tuple-like access
template <class PT>
struct traits<PT, meta::when<has_tuple_like_access<PT>::value>>
{
	using size = tuple_size<PT>;

	template <size_t I>
	using element = tuple_element<I, PT>;

	template <size_t I, class PT2,
	          class = std::enable_if_t<I<size::value>> static constexpr decltype(
	                  auto) get(PT2 &&pt) noexcept
	{
		return product_type_detail::get_adl::xget<I>(forward<PT2>(pt));
	}
};


// customization for C-arrays
template <class T, size_t N>
struct traits<T[N]>
{
	using size = integral_constant<size_t, N>;
	template <size_t I>
	struct element
	{
		using type = T;
	};

	template <size_t I, class U, size_t M,
	          class = std::enable_if_t<I<N>> static constexpr U &get(
	                  U (&arr)[M]) noexcept
	{
		return arr[I];
	}
};

template <class T, size_t N>
struct traits<T (&)[N]> : traits<T[N]>
{
};

template <class PT>
struct size : traits<PT>::size
{
};
template <class T>
struct size<const T> : size<T>
{
};
template <class T>
struct size<volatile T> : size<T>
{
};
template <class T>
struct size<const volatile T> : size<T>
{
};
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
template <class PT>
constexpr size_t size_v = size<PT>::value;
#endif
template <size_t I, class PT, class = std::enable_if_t<I<size_v<PT>>>
struct element : traits<PT>::template element<I>
{
};
template <size_t I, class PT>
using element_t = typename element<I, PT>::type;
template <size_t I, class T>
struct element<I, const T>
{
	using type = const element_t<I, T>;
};
template <size_t I, class T>
struct element<I, volatile T>
{
	using type = volatile element_t<I, T>;
};
template <size_t I, class T>
struct element<I, const volatile T>
{
	using type = const volatile element_t<I, T>;
};

template <class PT>
struct empty : integral_constant<bool, 0 == product_type::size_v<PT>>
{
};
template <class PT>
constexpr bool empty_v = empty<PT>::value;
template <class PT>
struct not_empty : integral_constant<bool, (0 != product_type::size_v<PT>)>
{
};
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
template <class PT>
constexpr bool not_empty_v = not_empty<PT>::value;
#endif
template <
        size_t I, class PT,
        class = std::enable_if_t<I<size_v<remove_cvref_t<PT>>>> constexpr decltype(
                auto) get(PT &&pt) noexcept
{
	return traits<remove_cvref_t<PT>>::template get<I>(forward<PT>(pt));
}

template <class PT>
constexpr size_t get_size(PT &&) noexcept
{
	return product_type::size_v<remove_reference_t<PT>>;
}

template <class PT>
using element_sequence_for =
        make_index_sequence<product_type::size_v<remove_reference_t<PT>>>;
}

// fixme redefine it as we did for has_tuple_like_access
template <class T>
struct is_product_type
#if !defined JASEL_DOXYGEN_INVOKED
      : integral_constant<bool,
            ! is_base_of<product_type::detail::not_a_product_type_tag, product_type::traits<T>>::value
        > {}
#endif
;
template <class T>
struct is_product_type<const T> : is_product_type<T>
{
};
template <class T>
struct is_product_type<volatile T> : is_product_type<T>
{
};
template <class T>
struct is_product_type<const volatile T> : is_product_type<T>
{
};
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
template <class T>
constexpr bool is_product_type_v = is_product_type<T>::value;
#endif

// fixme: Is this needed? Do we want that is_product_type<T(&)[N]>::value to be
// true?
// template <class T, size_t N>
// struct is_product_type<T(&)[N]> : true_type {};

namespace product_type
{
template <class PT, template <class> class Trait,
          bool B = is_product_type_v<PT>>
struct friendly_type_trait
#if !defined JASEL_DOXYGEN_INVOKED
        : false_type
{
}
#endif
;
template <class PT, template <class> class Trait>
struct friendly_type_trait<PT, Trait, true>
#if !defined JASEL_DOXYGEN_INVOKED
        : Trait<PT>
{
}
#endif
;
}
}
}
}
#endif
#endif // header
