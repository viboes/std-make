//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_OBSERVER_PTR_OBSERVER_PTR_HPP
#define JASEL_FUNDAMENTAL_V3_OBSERVER_PTR_OBSERVER_PTR_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
///////////////////////////////////////////////////////////////////////////////////////
#include <experimental/fundamental/v2/config.hpp>
#include <cstddef>
#include <functional>
#include <type_traits>

// observer_ptr is a non-owning pointer to a single object (no poinyer arithmetic) providing default construction to nullptr.
// See https://en.cppreference.com/w/cpp/experimental/observer_ptr

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <class T>
class observer_ptr {
public:
	template <class U>
	friend class observer_ptr;

	using element_type = T;

	constexpr observer_ptr() noexcept
	        : ptr(nullptr) {}
	constexpr explicit observer_ptr(std::nullptr_t) noexcept
	        : ptr(nullptr) {}
	constexpr explicit observer_ptr(T *ptr) noexcept
	        : ptr(ptr) {}

	observer_ptr(const observer_ptr &other) = default;
	observer_ptr(observer_ptr &&other)      = default;

	template <class W2>
	// requires W2* is convertible T*
	observer_ptr(observer_ptr<W2> other) noexcept
	        : ptr(other.ptr)
	{
	}

	observer_ptr &operator=(const observer_ptr &other) = default;
	observer_ptr &operator=(observer_ptr &&other) = default;

	~observer_ptr() noexcept = default;

	constexpr T &operator*() const noexcept { return *ptr; }
	constexpr T *operator->() const noexcept { return ptr; }
	constexpr T *get() const noexcept { return ptr; }

	constexpr explicit operator element_type *() const noexcept { return ptr; }

	constexpr explicit operator bool() const noexcept { return ptr != nullptr; }

	JASEL_CXX14_CONSTEXPR element_type *release() noexcept
	{
		element_type *res = ptr;
		reset();
		return res;
	}
	JASEL_CXX14_CONSTEXPR void reset(element_type *p = nullptr) noexcept { ptr = p; }
	JASEL_CXX14_CONSTEXPR void swap(observer_ptr &other) noexcept
	{
		using std::swap;
		swap(ptr, other.ptr);
	}

private:
	T *ptr;
};

// specialized algorithms:
// swap:

template <class W>
JASEL_CXX14_CONSTEXPR void swap(observer_ptr<W> &p1, observer_ptr<W> &p2) noexcept
{
	p1.swap(p2);
}

// factory:
template <class W>
constexpr observer_ptr<W> make_observer(W *p) noexcept
{
	return observer_ptr<W>(p);
}

// comparison
template <class W1, class W2>
constexpr bool operator==(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return p1.get() == p2.get();
}

template <class W1, class W2>
constexpr bool operator!=(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return !(p1 == p2);
}

template <class W>
constexpr bool operator==(observer_ptr<W> p, std::nullptr_t) noexcept
{
	return !p;
}

template <class W>
constexpr bool operator==(std::nullptr_t, observer_ptr<W> p) noexcept
{
	return !p;
}

template <class W>
constexpr bool operator!=(observer_ptr<W> p, std::nullptr_t) noexcept
{
	return static_cast<bool>(p);
}

template <class W>
constexpr bool operator!=(std::nullptr_t, observer_ptr<W> p) noexcept
{
	return static_cast<bool>(p);
}

template <class W1, class W2>
constexpr bool operator<(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return std::less<typename std::common_type<W1 *, W2 *>::type>()(p1.get(), p2.get());
}

template <class W1, class W2>
constexpr bool operator>(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return p2 < p1;
}

template <class W1, class W2>
constexpr bool operator<=(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return !(p2 < p1);
}

template <class W1, class W2>
constexpr bool operator>=(observer_ptr<W1> p1, observer_ptr<W2> p2) noexcept
{
	return !(p1 < p2);
}

} // namespace fundamental_v3
} // namespace experimental

// hash specialization
template <class T>
struct hash<experimental::observer_ptr<T>>
{
	size_t operator()(experimental::observer_ptr<T> p) { return hash<T *>()(p.get()); }
};

} // namespace std
#endif // header
