// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017-2018 Vicente J. Botet Escriba

// todo
// * add view::optional that contains a pointer to a class C and is parameterized by the present data member pointer and
//    the value data member pointer
//  template <class C, class T, T C::*Value, class Bool, Bool C::*Present>
//  class optional_view;
//
// * add a optional that has the type of the mask as parameter
// * what is common to all these classes
//  If we make abstraction of the storage, all these classes define almost the same interface, except maybe the construction
//

#ifndef JASEL_FUNDAMENTAL_V3_POD_OPTIONAL_HPP
#define JASEL_FUNDAMENTAL_V3_POD_OPTIONAL_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/optional.hpp>
#include <experimental/type_traits.hpp>
#include <experimental/utility.hpp>
#include <functional>

#include <experimental/fundamental/v3/pod/initializer/initializer.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace pod
{
using std::experimental::bad_optional_access;
using std::experimental::in_place;
using std::experimental::in_place_t;
using std::experimental::nullopt;
using std::experimental::nullopt_t;

template <class T>
struct optional_data
{
	static_assert(is_pod<T>::value, "T must be a POD");

	using bool_type  = unsigned char;
	using value_type = T;

	constexpr optional_data() = default; // optional is a POD

	constexpr bool     has_value() const { return m_present; }
	explicit constexpr operator bool() const { return has_value(); }

	bool_type  m_present;
	value_type m_value;
};

template <class T>
class optional_buf;

// We can see pod::optional as a specialization of std::optional<T> when T is a POD (trivial),
// that is itself a POD, that is, no specific default construction.
template <class T>
class optional {
public:
	static_assert(is_pod<T>::value, "T must be a POD");

	using bool_type  = unsigned char;
	using value_type = T;

	constexpr optional() = default; // optional is a POD

	//! In C++98 you should use instead
	//!   pod::optional<T> opt;
	//!   opt.default_initialize(); // or
	//!   opt = nullopt; // or
	//!   opt.reset(); // or
	//!   opt = default_initializer{};
	//! or
	//!   pod::optional<T> opt = {};
	//!
	//! @par Effects Constructs the object that does not contain a value.

	void default_initialize() noexcept
	{
		m_present = bool_type(false);
	}

	optional(optional const &) = default;            // optional is a POD
	optional(optional &&)      = default;            // optional is a POD
	optional &operator=(optional const &) = default; // optional is a POD
	optional &operator=(optional &&) = default;      // optional is a POD
	~optional()                      = default;

	//! You could use
	//! opt1.copy_assign(opt2);
	//! which will copy the value type only if m_present.
	//! Note that default copy and assignment will copy all the bytes
	//! up to you to see which is more efficient in a particular case
	void copy_assign(optional const &other) noexcept
	{
		m_present = other.m_present;
		if (bool(m_present))
		{
			m_value = other.m_value;
		}
	}
	void move_assign(optional &&other) noexcept
	{
		if (bool(m_present))
		{
			m_value = move(other.m_value);
		}
		m_present = other.m_present;
	}

	constexpr optional(nullopt_t) noexcept
	        : m_present(bool_type(false))
	{
	}
	static JASEL_CXX14_CONSTEXPR optional none() noexcept
	{
		optional<T> res;
		res.m_present = bool_type(false);
		return res;
	}

	template <class U = value_type, JASEL_REQUIRES(
	                                        is_constructible<T, U &&>::value && !is_same<remove_cvref_t<U>, in_place_t>::value && !is_same<remove_cvref_t<U>, optional<T>>::value && is_convertible<U &&, T>::value)>
	constexpr optional(U &&value) noexcept
	        : m_present(bool_type(true)), m_value(constexpr_move(value))
	{
	}
	template <class U = value_type, JASEL_REQUIRES(
	                                        is_constructible<T, U &&>::value && !is_same<remove_cvref_t<U>, in_place_t>::value && !is_same<remove_cvref_t<U>, optional<T>>::value && !is_convertible<U &&, T>::value)>
	explicit constexpr optional(U &&value) noexcept
	        : m_present(bool_type(true)), m_value(constexpr_move(value))
	{
	}
	template <class U = value_type, JASEL_REQUIRES(
	                                        is_constructible<T, U &&>::value && !is_same<remove_cvref_t<U>, in_place_t>::value && !is_same<remove_cvref_t<U>, optional<T>>::value && !is_convertible<U &&, T>::value)>
	static JASEL_CXX14_CONSTEXPR optional some(U &&value) noexcept
	{
		optional<T> res;
		res.m_present = bool_type(true);
		res.m_value   = constexpr_move(value);
		return res;
	}

#if 0
    // todo: define these operations
    template < class U >
    /* EXPLICIT */optional( const optional<U>& other ) noexcept;
    template < class U >
    /* EXPLICIT */optional( optional<U>&& other ) noexcept;

    template < class U >
    void copy_assign(optional<U> const& other) noexcept
    {
        m_present = other.m_present;
        if ( bool(m_present) )
        {
            m_value = other.m_value;
        }
    }
    template < class U >
    void move_assign(optional<U> && other) noexcept
    {
        if ( bool(m_present) )
        {
            m_value = move(other.m_value);
        }
        m_present = other.m_present;
    }
#endif

	template <class... Args, JASEL_T_REQUIRES(is_constructible<T, Args &&...>::value)>
	JASEL_CXX14_CONSTEXPR explicit optional(in_place_t, Args &&... args) noexcept
	        : m_present(bool_type(true)), m_value(forward<Args>(args)...)
	{
	}
	template <class U, class... Args, JASEL_T_REQUIRES(is_constructible<T, std::initializer_list<U>, Args &&...>::value)>
	JASEL_CXX14_CONSTEXPR explicit optional(in_place_t,
	                                        std::initializer_list<U> ilist,
	                                        Args &&... args) noexcept
	        : m_present(bool_type(true)), m_value(ilist, forward<Args>(args)...)
	{
	}

#if 0
    // todo: uncomment when is_aggregate is available
    template < class... Args
            , JASEL_T_REQUIRES( is_aggregate<T>::value )
    >
    JASEL_CXX14_CONSTEXPR explicit optional( std::in_place_t, Args&&... args ) noexcept
            : m_present(bool_type(true))
            , m_value{   forward<Args>(args)...}
    {
    }
#endif

	// Assignment
	optional &operator=(nullopt_t) noexcept
	{
		m_present = bool_type(false);
		return *this;
	}
	template <class U = T, JASEL_REQUIRES(
	                               is_constructible<T, U>::value && !is_same<remove_cvref_t<U>, optional<T>>::value && is_assignable<T &, U>::value && (!is_same<remove_cvref_t<U>, T>::value || !is_scalar<T>::value))>
	optional &operator=(U &&value) noexcept
	{
		m_value   = move(value);
		m_present = bool_type(true);
		return *this;
	}

#if 0
    // todo: Add constraints to this operation
    template <class U>
    optional& operator=(optional<U>&& x) noexcept
    {
        m_present = x.m_present;
        if (x.m_present)
            m_value = move(x.m_value);
        return *this;
    }
    template <class U>
    optional& operator=(optional<U> const& x) noexcept
    {
        m_present = x.m_present;
        if (x.m_present)
            m_value = x.m_value;
        return *this;
    }
#endif

	template <class U>
	optional &operator=(std::experimental::optional<U> &&x) noexcept
	{
		m_present = bool(x);
		if (x)
			m_value = move(*x);
		return *this;
	}
	template <class U>
	optional &operator=(std::experimental::optional<U> const &x) noexcept
	{
		m_present = bool(x);
		if (x)
			m_value = *x;
		return *this;
	}

	template <class... Args, JASEL_T_REQUIRES(is_constructible<T, Args &&...>::value)>
	void emplace(Args &&... args) noexcept
	{
		m_value   = T(std::forward<Args>(args)...);
		m_present = bool_type(true);
	}

	template <class U, class... Args, JASEL_T_REQUIRES(is_constructible<T, std::initializer_list<U>, Args &&...>::value)>
	void emplace(initializer_list<U> il, Args &&... args) noexcept
	{
		m_value   = T(il, std::forward<Args>(args)...);
		m_present = bool_type(true);
	}

#if 0
    // todo: uncomment when is_aggregate is available
    template <class... Args
            , JASEL_T_REQUIRES( is_aggregate<T>::value )
    >
    void emplace(Args&&... args) noexcept
    {
        m_value = T{std::forward<Args>(args)...};
        m_present = bool_type(true);
    }
#endif

	void swap(optional &rhs) noexcept
	{
		swap(m_value, rhs.m_value);
		swap(m_present, rhs.m_present);
	}

	JASEL_CXX14_CONSTEXPR const T *operator->() const
	{
		return &m_value;
	}
	JASEL_CXX14_CONSTEXPR T *operator->()
	{
		return &m_value;
	}

	JASEL_CXX14_CONSTEXPR const T &operator*() const &
	{
		return m_value;
	}
	JASEL_CXX14_CONSTEXPR T &operator*() &
	{
		return m_value;
	}
	JASEL_CXX14_CONSTEXPR const T &&operator*() const &&
	{
		return constexpr_move(m_value);
	}
	JASEL_CXX14_CONSTEXPR T &&operator*() &&
	{
		return constexpr_move(m_value);
	}

	constexpr bool has_value() const noexcept
	{
		return bool(m_present);
	}
	constexpr explicit operator bool() const noexcept
	{
		return has_value();
	}

	JASEL_CXX14_CONSTEXPR value_type &value() &
	{
		if (!has_value())
		{
			//experimental::bad_optional_access x;
			throw bad_optional_access();
		}
		return m_value;
	}
	JASEL_CXX14_CONSTEXPR value_type const &value() const &
	{
		if (!has_value())
		{
			throw experimental::bad_optional_access();
		}
		return m_value;
	}

	JASEL_CXX14_CONSTEXPR value_type &&value() &&
	{
		if (!has_value())
		{
			throw experimental::bad_optional_access();
		}
		return m_value;
	}
	JASEL_CXX14_CONSTEXPR value_type const &&value() const &&
	{
		if (!has_value())
		{
			throw experimental::bad_optional_access();
		}
		return m_value;
	}

	template <class U>
	JASEL_CXX14_CONSTEXPR value_type value_or(U &&default_value) const &
	{
		return has_value() ? **this : static_cast<T>(forward<U>(default_value));
	}
	template <class U>
	JASEL_CXX14_CONSTEXPR value_type value_or(U &&default_value) &&
	{
		return has_value() ? constexpr_move(**this) : static_cast<T>(forward<U>(default_value));
	}

	void reset() noexcept
	{
		m_present = bool_type(false);
	}

private:
	bool_type  m_present;
	value_type m_value;
};

// 20.5.8, Relational operators
template <class T>
constexpr bool operator==(const optional<T> &x, const optional<T> &y)
{
	return bool(x) != bool(y) ? false : bool(x) == false ? true : *x == *y;
}

template <class T>
constexpr bool operator!=(const optional<T> &x, const optional<T> &y)
{
	return !(x == y);
}

template <class T>
constexpr bool operator<(const optional<T> &x, const optional<T> &y)
{
	return (!y) ? false : (!x) ? true : *x < *y;
}

template <class T>
constexpr bool operator>(const optional<T> &x, const optional<T> &y)
{
	return (y < x);
}

template <class T>
constexpr bool operator<=(const optional<T> &x, const optional<T> &y)
{
	return !(y < x);
}

template <class T>
constexpr bool operator>=(const optional<T> &x, const optional<T> &y)
{
	return !(x < y);
}

#if 0
// 20.5.9, Comparison with nullopt
constexpr bool operator==(nullopt_t, nullopt_t) noexcept
{
    return true;
}
constexpr bool operator!=(nullopt_t, nullopt_t) noexcept
{
    return false;
}
#endif

template <class T>
constexpr bool operator==(const optional<T> &x, nullopt_t) noexcept
{
	return (!x);
}

template <class T>
constexpr bool operator==(nullopt_t, const optional<T> &x) noexcept
{
	return (!x);
}

template <class T>
constexpr bool operator!=(const optional<T> &x, nullopt_t) noexcept
{
	return bool(x);
}

template <class T>
constexpr bool operator!=(nullopt_t, const optional<T> &x) noexcept
{
	return bool(x);
}

template <class T>
constexpr bool operator<(const optional<T> &, nullopt_t) noexcept
{
	return false;
}

template <class T>
constexpr bool operator<(nullopt_t, const optional<T> &x) noexcept
{
	return bool(x);
}

template <class T>
constexpr bool operator<=(const optional<T> &x, nullopt_t) noexcept
{
	return (!x);
}

template <class T>
constexpr bool operator<=(nullopt_t, const optional<T> &) noexcept
{
	return true;
}

template <class T>
constexpr bool operator>(const optional<T> &x, nullopt_t) noexcept
{
	return bool(x);
}

template <class T>
constexpr bool operator>(nullopt_t, const optional<T> &) noexcept
{
	return false;
}

template <class T>
constexpr bool operator>=(const optional<T> &, nullopt_t) noexcept
{
	return true;
}

template <class T>
constexpr bool operator>=(nullopt_t, const optional<T> &x) noexcept
{
	return (!x);
}

#if 0
// 20.5.10, Comparison with T
template <class T>
constexpr bool operator==(const optional<T>& x, const T& v)
{
    return bool(x) ? *x == v : false;
}

template <class T>
constexpr bool operator==(const T& v, const optional<T>& x)
{
    return bool(x) ? v == *x : false;
}

template <class T>
constexpr bool operator!=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x != v : true;
}

template <class T>
constexpr bool operator!=(const T& v, const optional<T>& x)
{
    return bool(x) ? v != *x : true;
}

template <class T>
constexpr bool operator<(const optional<T>& x, const T& v)
{
    return bool(x) ? *x < v : true;
}

template <class T>
constexpr bool operator>(const T& v, const optional<T>& x)
{
    return bool(x) ? v > *x : true;
}

template <class T>
constexpr bool operator>(const optional<T>& x, const T& v)
{
    return bool(x) ? *x > v : false;
}

template <class T>
constexpr bool operator<(const T& v, const optional<T>& x)
{
    return bool(x) ? v < *x : false;
}

template <class T>
constexpr bool operator>=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x >= v : false;
}

template <class T>
constexpr bool operator<=(const T& v, const optional<T>& x)
{
    return bool(x) ? v <= *x : false;
}

template <class T>
constexpr bool operator<=(const optional<T>& x, const T& v)
{
    return bool(x) ? *x <= v : true;
}

template <class T>
constexpr bool operator>=(const T& v, const optional<T>& x)
{
    return bool(x) ? v >= *x : true;
}
#endif

// 20.5.12, Specialized algorithms
template <class T>
void swap(optional<T> &x, optional<T> &y) noexcept
{
	x.swap(y);
}

template <class T>
constexpr optional<remove_cvref_t<T>> make_optional(T &&v)
{
	return optional<remove_cvref_t<T>>(constexpr_forward<T>(v));
}

#if 0
template <class T, class ... Args>
constexpr optional<T> make_optional(Args&&... args)
{
    return optional<T>(forward<Args>(args)...);
}

template <class T, class U, class ... Args>
constexpr optional<T> make_optional( initializer_list<U> il, Args&&... args)
{
    return optional<T>(il, forward<Args>(args)...);
}
#endif

template <class T>
struct is_default_initializable<optional<T>> : is_pod<T>
{
};

} // namespace pod

static_assert(is_pod<pod::optional<int>>::value, "");
static_assert(is_standard_layout<pod::optional<int>>::value, "");
static_assert(is_trivial<pod::optional<int>>::value, "");

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
namespace std
{
template <typename T>
struct hash<std::experimental::pod::optional<T>>
{
	typedef typename hash<T>::result_type       result_type;
	typedef std::experimental::pod::optional<T> argument_type;

	constexpr result_type operator()(argument_type const &arg) const
	{
		return arg ? std::hash<T>{}(*arg) : result_type{};
	}
};
} // namespace std

#endif // header
