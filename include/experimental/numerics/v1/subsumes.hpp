// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2018 Vicente J. Botet Escriba

#ifndef JASEL_NULMERIC_V1_SUBSUMES_HPP
#define JASEL_NULMERIC_V1_SUBSUMES_HPP

#include <experimental/utility.hpp>
#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
namespace numerics
{
inline namespace v1
{
// A type T subsumes a type U if all the instances of U have a corresponding instance in T, that is, T(u) is and
// if u1 != u2 then T(u1) != T(u2)
// if pred(u1, u2) then pred(T)(T(u1) , T(u2)), where pred(T) is the equivalent predicate for T
// e.g.
// signed char < signed short < signed int < signed long < signed long long
// unsigned char < unsigned short < unsigned int < unsigned long < unsigned long long
// unsigned char < signed short
// unsigned short < signed int
// unsigned int < signed long
// unsigned long < signed long long
// In addition, the subsumes relationship must be closed, that is if subsumes<A,B> and subsumes<B,C> then subsumes <A,C>
// This relation could be extended to types that are not numeric as well.
template <class T, class U>
struct subsumes : std::false_type
{
};

template <class T>
struct subsumes<T, T> : std::true_type
{
};

template <>
struct subsumes<signed short, signed char> : std::true_type
{
};
template <>
struct subsumes<signed int, signed char> : std::true_type
{
};
template <>
struct subsumes<signed long, signed char> : std::true_type
{
};
template <>
struct subsumes<signed long long, signed char> : std::true_type
{
};

template <>
struct subsumes<signed int, signed short> : std::true_type
{
};
template <>
struct subsumes<signed long, signed short> : std::true_type
{
};
template <>
struct subsumes<signed long long, signed short> : std::true_type
{
};

template <>
struct subsumes<signed long, signed int> : std::true_type
{
};
template <>
struct subsumes<signed long long, signed int> : std::true_type
{
};

template <>
struct subsumes<signed long long, signed long> : std::true_type
{
};

//////////

template <>
struct subsumes<unsigned short, unsigned char> : std::true_type
{
};
template <>
struct subsumes<unsigned int, unsigned char> : std::true_type
{
};
template <>
struct subsumes<unsigned long, unsigned char> : std::true_type
{
};
template <>
struct subsumes<unsigned long long, unsigned char> : std::true_type
{
};

template <>
struct subsumes<unsigned int, unsigned short> : std::true_type
{
};
template <>
struct subsumes<unsigned long, unsigned short> : std::true_type
{
};
template <>
struct subsumes<unsigned long long, unsigned short> : std::true_type
{
};

template <>
struct subsumes<unsigned long, unsigned int> : std::true_type
{
};
template <>
struct subsumes<unsigned long long, unsigned int> : std::true_type
{
};

template <>
struct subsumes<unsigned long long, unsigned long> : std::true_type
{
};

//////
template <>
struct subsumes<signed short, unsigned char> : std::true_type
{
};
template <>
struct subsumes<signed int, unsigned char> : std::true_type
{
};
template <>
struct subsumes<signed long, unsigned char> : std::true_type
{
};
template <>
struct subsumes<signed long long, unsigned char> : std::true_type
{
};

template <>
struct subsumes<signed int, unsigned short> : std::true_type
{
};
template <>
struct subsumes<signed long, unsigned short> : std::true_type
{
};
template <>
struct subsumes<signed long long, unsigned short> : std::true_type
{
};

template <>
struct subsumes<signed long, unsigned int> : std::true_type
{
};
template <>
struct subsumes<signed long long, unsigned int> : std::true_type
{
};

template <>
struct subsumes<signed long long, unsigned long> : std::true_type
{
};

// domain utilities

//! A value u of a domain U is in the domain of a type T if:
//! * T is U, or
//! * T subsumes U or
//! * can_narrow_to<T>(u)
template <class T, class U>
constexpr
        typename std::enable_if<
                std::is_same<T, U>::value, bool>::type
        in_domain(U const &)
{
	return true;
}

template <class T, class U>
constexpr
        typename std::enable_if<
                !std::is_same<T, U>::value && !subsumes<T, U>::value, bool>::type
        in_domain(U const &u)
{
	return can_narrow_to<T>(u);
}

template <class T, class U>
constexpr
        typename std::enable_if<
                subsumes<T, U>::value, bool>::type
        in_domain(U const &)
{
	return true;
}

// Given a type T that subsumes a type U, subsume<T>(u) is a value of type T that represents the same value as u in U
// it is something like a cast from a reference to a derived class to a reference to a base class,
// we are sure to don't loss information and that that information is coherent.
// While this is always safe it is not trivial as very often the layout changes.
template <class T, class U, typename = typename std::enable_if<subsumes<T, U>::value>::type>
constexpr T subsume(U const &u)
{
	return static_cast<T>(u);
}

} // namespace v1
} // namespace numerics
} // namespace experimental
} // namespace std
#endif // header
