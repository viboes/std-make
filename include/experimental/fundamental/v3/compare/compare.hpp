//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

/// This file contains only the interface for the equality and ordering strong types
/// The spaceship operator default definition should depend only on this file

#ifndef JASEL_FUNDAMENTAL_V3_COMPARE_COMPARISON_HPP
#define JASEL_FUNDAMENTAL_V3_COMPARE_COMPARISON_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/utility.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

struct _CmpUnspecifiedType;
using _CmpUnspecifiedParam = void (_CmpUnspecifiedType::*)();


    // Move this to detail
    enum class eq
    { _zero = 0, equal = 0, equivalent = equal, nonequal = 1, nonequivalent = nonequal };
    enum class ord
    { less = -1, greater = 1};
    enum class ncmp
    { unordered = -127};

    // comparison category types

    //============================================================================
    // _equality:
    // - use int as underlying type + default copying semantics
    // - can only be constructed from specific values
    // - can be compared against 0 (== and != only)
    //============================================================================
    //----------------------------------------------------------------------------
    class weak_equality
    {
        eq value;

        constexpr explicit weak_equality(eq v) noexcept
        : value( v )
        {}

    public:
        static const weak_equality equivalent, nonequivalent;

        friend constexpr bool operator==(weak_equality v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator!=(weak_equality v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator==(_CmpUnspecifiedParam, weak_equality v) noexcept;
        friend constexpr bool operator!=(_CmpUnspecifiedParam, weak_equality v) noexcept;
    };

    JASEL_INLINE_VAR constexpr weak_equality weak_equality::equivalent{eq::equivalent};
    JASEL_INLINE_VAR constexpr weak_equality weak_equality::nonequivalent{eq::nonequivalent};

    inline constexpr bool operator==(weak_equality v, _CmpUnspecifiedParam) noexcept
    {   return v.value == eq::_zero;}
    inline constexpr bool operator!=(weak_equality v, _CmpUnspecifiedParam) noexcept
    {   return v.value != eq::_zero;}
    inline constexpr bool operator==(_CmpUnspecifiedParam, weak_equality v) noexcept
    {   return eq::_zero == v.value;}
    inline constexpr bool operator!=(_CmpUnspecifiedParam, weak_equality v) noexcept
    {   return eq::_zero != v.value;}

    //----------------------------------------------------------------------------
    class strong_equality
    {
        eq value;

        constexpr explicit strong_equality(eq v) noexcept
        : value(v)
        {}

    public:

        static const strong_equality equal, equivalent, nonequal, nonequivalent;

        constexpr operator weak_equality() const noexcept;

        friend constexpr bool operator==(strong_equality v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator!=(strong_equality v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator==(_CmpUnspecifiedParam, strong_equality v) noexcept;
        friend constexpr bool operator!=(_CmpUnspecifiedParam, strong_equality v) noexcept;
    };

    JASEL_INLINE_VAR constexpr strong_equality strong_equality::equal
    {   eq::equal};
    JASEL_INLINE_VAR constexpr strong_equality strong_equality::equivalent
    {   eq::equivalent}; // for convenient substitutability in generic code
    JASEL_INLINE_VAR constexpr strong_equality strong_equality::nonequal
    {   eq::nonequal};
    JASEL_INLINE_VAR constexpr strong_equality strong_equality::nonequivalent
    {   eq::nonequivalent}; // for convenient substitutability in generic code

    inline constexpr strong_equality::operator weak_equality() const noexcept
    {   return this->value == equal.value ? weak_equality::equivalent : weak_equality::nonequivalent;}


    inline constexpr bool operator==(strong_equality v, _CmpUnspecifiedParam) noexcept
    {   return v.value == eq::_zero;}
    inline constexpr bool operator!=(strong_equality v, _CmpUnspecifiedParam) noexcept
    {   return v.value != eq::_zero;}
    inline constexpr bool operator==(_CmpUnspecifiedParam, strong_equality v) noexcept
    {   return eq::_zero == v.value;}
    inline constexpr bool operator!=(_CmpUnspecifiedParam, strong_equality v) noexcept
    {   return eq::_zero != v.value;}

    //============================================================================
    // _ordering:
    // - use int as underlying type + default copying semantics
    // - can only be constructed from specific values
    // - can be (implicitly) compared against 0
    // - can be explicitly converted to int
    // - do not convert to bool, to prevent the "if( strcmp(...) )" mistake
    //============================================================================
    //----------------------------------------------------------------------------
    class partial_ordering
    {
        using ValueT = signed char;
        ValueT value;
        constexpr explicit partial_ordering(eq v) noexcept
                        : value{ValueT(v)}
        {}
        constexpr explicit partial_ordering(ord v) noexcept
                        : value{ValueT(v)}
        {}
        constexpr explicit partial_ordering(ncmp v) noexcept
                        : value{ValueT(v)}
        {}

        constexpr bool is_ordered() const noexcept {
          return value != ValueT(ncmp::unordered);
      }
    public:
        static const partial_ordering less, equivalent, greater, unordered;

        constexpr operator weak_equality() const noexcept;

        friend constexpr bool operator==(partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator!=(partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator< (partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator<=(partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator> (partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator>=(partial_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator==(_CmpUnspecifiedParam, partial_ordering v) noexcept;
        friend constexpr bool operator!=(_CmpUnspecifiedParam, partial_ordering v) noexcept;
        friend constexpr bool operator< (_CmpUnspecifiedParam, partial_ordering v) noexcept;
        friend constexpr bool operator<=(_CmpUnspecifiedParam, partial_ordering v) noexcept;
        friend constexpr bool operator> (_CmpUnspecifiedParam, partial_ordering v) noexcept;
        friend constexpr bool operator>=(_CmpUnspecifiedParam, partial_ordering v) noexcept;

    };
    JASEL_INLINE_VAR constexpr partial_ordering partial_ordering::less
    {   ord::less };
    JASEL_INLINE_VAR constexpr partial_ordering partial_ordering::equivalent
    {   eq::equivalent };
    JASEL_INLINE_VAR constexpr partial_ordering partial_ordering::greater
    {   ord::greater};
    JASEL_INLINE_VAR constexpr partial_ordering partial_ordering::unordered
    {   ncmp::unordered };

    inline constexpr partial_ordering::operator weak_equality() const noexcept
    { return this->value == equivalent.value ? weak_equality::equivalent : weak_equality::nonequivalent;}

    inline constexpr bool operator==(partial_ordering v, _CmpUnspecifiedParam) noexcept
    { return v.is_ordered() && v.value == 0;}
    inline constexpr bool operator!=(partial_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.is_ordered() && v.value != 0;}
    inline constexpr bool operator< (partial_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.is_ordered() && v.value < 0;}
    inline constexpr bool operator<=(partial_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.is_ordered() && v.value <= 0;}
    inline constexpr bool operator> (partial_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.is_ordered() && v.value > 0;}
    inline constexpr bool operator>=(partial_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.is_ordered() && v.value >= 0;}
    inline constexpr bool operator==(_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 == v.value;}
    inline constexpr bool operator!=(_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 != v.value;}
    inline constexpr bool operator< (_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 < v.value;}
    inline constexpr bool operator<=(_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 <= v.value;}
    inline constexpr bool operator> (_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 > v.value;}
    inline constexpr bool operator>=(_CmpUnspecifiedParam, partial_ordering v) noexcept
    {   return v.is_ordered() && 0 >= v.value;}


    //----------------------------------------------------------------------------
    class weak_ordering
    {
        using ValueT = signed char;
        ValueT value;
        constexpr explicit weak_ordering(eq v) noexcept
        : value(ValueT(v))
        {}
        constexpr explicit weak_ordering(ord v) noexcept
        : value(ValueT(v))
        {}
    public:

        static const weak_ordering less, equivalent, greater;

        constexpr operator weak_equality() const noexcept;
        constexpr operator partial_ordering() const noexcept;

        friend constexpr bool operator==(weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator!=(weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator< (weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator<=(weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator> (weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator>=(weak_ordering v, _CmpUnspecifiedParam) noexcept;
        friend constexpr bool operator==(_CmpUnspecifiedParam, weak_ordering v) noexcept;
        friend constexpr bool operator!=(_CmpUnspecifiedParam, weak_ordering v) noexcept;
        friend constexpr bool operator< (_CmpUnspecifiedParam, weak_ordering v) noexcept;
        friend constexpr bool operator<=(_CmpUnspecifiedParam, weak_ordering v) noexcept;
        friend constexpr bool operator> (_CmpUnspecifiedParam, weak_ordering v) noexcept;
        friend constexpr bool operator>=(_CmpUnspecifiedParam, weak_ordering v) noexcept;

    };
    JASEL_INLINE_VAR constexpr weak_ordering weak_ordering::less{ord::less};
    JASEL_INLINE_VAR constexpr weak_ordering weak_ordering::equivalent{eq::equivalent};
    JASEL_INLINE_VAR constexpr weak_ordering weak_ordering::greater{ord::greater};


    inline constexpr weak_ordering::operator weak_equality() const noexcept
    {   return this->value == equivalent.value ? weak_equality::equivalent : weak_equality::nonequivalent;}
    inline constexpr weak_ordering::operator partial_ordering() const noexcept
    {
        return this->value == equivalent.value ? partial_ordering::equivalent
        : this->value == less.value ? partial_ordering::less
        : partial_ordering::greater;
    }
    inline constexpr bool operator==(weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value == 0;}
    inline constexpr bool operator!=(weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value != 0;}
    inline constexpr bool operator< (weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value < 0;}
    inline constexpr bool operator<=(weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value <= 0;}
    inline constexpr bool operator> (weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value > 0;}
    inline constexpr bool operator>=(weak_ordering v, _CmpUnspecifiedParam) noexcept
    {   return v.value >= 0;}
    inline constexpr bool operator==(_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 == v.value;}
    inline constexpr bool operator!=(_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 != v.value;}
    inline constexpr bool operator< (_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 < v.value;}
    inline constexpr bool operator<=(_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 <= v.value;}
    inline constexpr bool operator> (_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 > v.value;}
    inline constexpr bool operator>=(_CmpUnspecifiedParam, weak_ordering v) noexcept
    {   return 0 >= v.value;}


    //----------------------------------------------------------------------------

    class strong_ordering
    {
        using ValueT = signed char;
        ValueT value;
        constexpr explicit strong_ordering(eq v) noexcept
        : value(ValueT(v))
        {}
        constexpr explicit strong_ordering(ord v) noexcept
        : value(ValueT(v))
        {}
    public:

        static const strong_ordering less, equal, equivalent, greater;

        constexpr operator weak_equality() const noexcept;
        constexpr operator strong_equality() const noexcept;
        constexpr operator partial_ordering() const noexcept;
        constexpr operator weak_ordering() const noexcept;

        friend constexpr bool operator==(strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value == 0;}
        friend constexpr bool operator!=(strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value != 0;}
        friend constexpr bool operator< (strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value < 0;}
        friend constexpr bool operator<=(strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value <= 0;}
        friend constexpr bool operator> (strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value > 0;}
        friend constexpr bool operator>=(strong_ordering v, _CmpUnspecifiedParam) noexcept
        {   return v.value >= 0;}
        friend constexpr bool operator==(_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 == v.value;}
        friend constexpr bool operator!=(_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 != v.value;}
        friend constexpr bool operator< (_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 < v.value;}
        friend constexpr bool operator<=(_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 <= v.value;}
        friend constexpr bool operator> (_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 > v.value;}
        friend constexpr bool operator>=(_CmpUnspecifiedParam, strong_ordering v) noexcept
        {   return 0 >= v.value;}

    };
    JASEL_INLINE_VAR constexpr strong_ordering strong_ordering::less{ord::less};
    JASEL_INLINE_VAR constexpr strong_ordering strong_ordering::equal{eq::equal};
    JASEL_INLINE_VAR constexpr strong_ordering strong_ordering::equivalent{eq::equivalent}; // for convenient substitutability in generic code
    JASEL_INLINE_VAR constexpr strong_ordering strong_ordering::greater{ord::greater};


    inline constexpr strong_ordering::operator weak_equality() const noexcept
    {   return this->value == equal.value ? weak_equality::equivalent : weak_equality::nonequivalent;}
    inline constexpr strong_ordering::operator strong_equality() const noexcept
    {   return this->value == equal.value ? strong_equality::equal : strong_equality::nonequivalent;}
    inline constexpr strong_ordering::operator partial_ordering() const noexcept
    {   return this->value == equal.value ? partial_ordering::equivalent
        : this->value == less.value ? partial_ordering::less : partial_ordering::greater;}
    inline constexpr strong_ordering::operator weak_ordering() const noexcept
    {   return this->value == equal.value ? weak_ordering::equivalent
        : this->value == less.value ? weak_ordering::less : weak_ordering::greater;
    }

    //----------------------------------------------------------------------------

    constexpr bool is_eq (weak_equality cmp) noexcept
    {   return cmp == 0;}
    constexpr bool is_neq (weak_equality cmp) noexcept
    {   return cmp != 0;}
    constexpr bool is_lt (partial_ordering cmp) noexcept
    {   return cmp < 0;}
    constexpr bool is_lteq(partial_ordering cmp) noexcept
    {   return cmp <= 0;}
    constexpr bool is_gt (partial_ordering cmp) noexcept
    {   return cmp > 0;}
    constexpr bool is_gteq(partial_ordering cmp) noexcept
    {   return cmp >= 0;}

    // [cmp.common], common comparison category type
    template<class... Ts>
    struct common_comparison_category { using type = void; };
    template<class... Ts>
    using common_comparison_category_t = typename common_comparison_category<Ts...>::type;

    // zero
    //template <>
    //struct common_comparison_category { using type = void; }

    // one
    template <>
    struct common_comparison_category<weak_equality>  { using type = weak_equality; };
    template <>
    struct common_comparison_category<strong_equality>  { using type = strong_equality; };
    template <>
    struct common_comparison_category<partial_ordering>  { using type = partial_ordering; };
    template <>
    struct common_comparison_category<weak_ordering>  { using type = weak_ordering; };
    template <>
    struct common_comparison_category<strong_ordering>  { using type = strong_ordering; };
    //template <class T>
    //struct common_comparison_category<T>  { using type = void; };

    // more
    template<class... Ts>
    struct common_comparison_category<weak_equality, weak_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_equality, strong_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_equality, partial_ordering, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_equality, weak_ordering, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_equality, strong_ordering, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    //template<class T, class... Ts>
    //struct common_comparison_category<weak_equality, T, Ts...>  : { using type = void; }


    template<class... Ts>
    struct common_comparison_category<strong_equality, weak_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_equality, partial_ordering, Ts...>  : common_comparison_category<strong_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_equality, weak_ordering, Ts...>  : common_comparison_category<strong_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_equality, strong_equality, Ts...>  : common_comparison_category<strong_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_equality, strong_ordering, Ts...>  : common_comparison_category<strong_equality, Ts...> {};
    //template<class T, class... Ts>
    //struct common_comparison_category<strong_equality, T, Ts...>  : { using type = void; }


    template<class... Ts>
    struct common_comparison_category<partial_ordering, weak_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<partial_ordering, strong_equality, Ts...>  : common_comparison_category<strong_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<partial_ordering, partial_ordering, Ts...>  : common_comparison_category<partial_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<partial_ordering, weak_ordering, Ts...>  : common_comparison_category<partial_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<partial_ordering, strong_ordering, Ts...>  : common_comparison_category<partial_ordering, Ts...> {};
    //template<class T, class... Ts>
    //struct common_comparison_category<partial_ordering, T, Ts...>  : { using type = void; }


    template<class... Ts>
    struct common_comparison_category<weak_ordering, weak_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_ordering, strong_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_ordering, partial_ordering, Ts...>  : common_comparison_category<partial_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_ordering, weak_ordering, Ts...>  : common_comparison_category<weak_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<weak_ordering, strong_ordering, Ts...>  : common_comparison_category<weak_ordering, Ts...> {};
    //template<class T, class... Ts>
    //struct common_comparison_category<weak_ordering, T, Ts...>  : { using type = void; }


    template<class... Ts>
    struct common_comparison_category<strong_ordering, weak_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_ordering, strong_equality, Ts...>  : common_comparison_category<weak_equality, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_ordering, partial_ordering, Ts...>  : common_comparison_category<partial_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_ordering, weak_ordering, Ts...>  : common_comparison_category<weak_ordering, Ts...> {};
    template<class... Ts>
    struct common_comparison_category<strong_ordering, strong_ordering, Ts...>  : common_comparison_category<strong_ordering, Ts...> {};

}
}
}
#endif
#endif // header
