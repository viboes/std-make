//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//////////////////////////////////////////////////////////////////////////////

/*
 \file
 \brief
 The header \c <experimental/.../bits/bit_pointer.hpp> defines a bit pointer to a bit located inside a word.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf
 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BIT_POINTER_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BIT_POINTER_HPP

#include <experimental/fundamental/v3/bits/binary_digits.hpp>

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
template <class WordType>
class bit_pointer {
	// Assertions
	static_assert(binary_digits<WordType>::value, "");

	// Friendship
	template <class>
	friend class bit_pointer;

	// Types
public:
	using word_type       = WordType;
	using index_type      = bit_value::index_type;
	using size_type       = bit_value::size_type;
	using difference_type = ptrdiff_t;

	// Lifecycle
public:
	constexpr bit_pointer() noexcept
	        : _ref(nullptr)
	{
	}
	constexpr bit_pointer(const bit_pointer &) noexcept = default;
	template <class T, JASEL_REQUIRES(is_convertible<T *, WordType *>::value)>
	constexpr bit_pointer(const bit_pointer<T> &other) noexcept
	        : _ref(other._ref)
	{
	}
	constexpr bit_pointer(nullptr_t) noexcept
	        : _ref(nullptr)
	{
	}
	explicit constexpr bit_pointer(word_type *ptr) noexcept
	        : _ref(ptr)
	{
	}
	constexpr bit_pointer(word_type *ptr, size_type pos)
	        : _ref(ptr, pos)
	{
	}

	// Assignment
public:
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator=(nullptr_t) noexcept
	{
		_ref.assign_as_pointer(nullptr);
		return *this;
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator=(const bit_pointer &other) noexcept
	{
		_ref.assign_as_pointer(other._ref);
		return *this;
	}
	template <class T, JASEL_REQUIRES(is_convertible<T *, WordType *>::value)>
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator=(const bit_pointer<T> &other) noexcept
	{
		_ref.assign_as_pointer(other._ref);
		return *this;
	}

	// Conversion
public:
	explicit constexpr operator bool() const noexcept
	{
		return _ref._ptr;
	}

	// Access
public:
	constexpr bit_reference<WordType> operator*() const noexcept
	{
		return _ref;
	}
	constexpr bit_reference<WordType> *operator->() const noexcept
	{
		return addressof(const_cast<bit_reference<WordType> &>(_ref));
	}
	JASEL_CXX14_CONSTEXPR bit_reference<WordType> operator[](difference_type n) const
	{
		constexpr difference_type digits = binary_digits<word_type>::value;
		const difference_type     sum    = static_cast<difference_type>(_ref.position()) + n;
		difference_type           diff   = sum / digits;
		if (sum < 0 && diff * digits != sum)
		{
			--diff;
		}
		return bit_reference<WordType>(_ref._ptr + diff, static_cast<size_type>(sum - diff * digits));
	}

	// Increment and decrement operators
public:
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator++()
	{
		_ref.inc_as_pointer();
		return *this;
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator--()
	{
		_ref.dec_as_pointer();
		return *this;
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer operator++(int)
	{
		bit_pointer old = *this;
		++(*this);
		return old;
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer operator--(int)
	{
		bit_pointer old = *this;
		--(*this);
		return old;
	}
	JASEL_CXX14_CONSTEXPR bit_pointer operator+(difference_type n) const
	{
		constexpr difference_type digits = binary_digits<word_type>::value;
		const difference_type     sum    = static_cast<difference_type>(_ref.position()) + n;
		difference_type           diff   = sum / digits;
		if (sum < 0 && diff * digits != sum)
		{
			--diff;
		}
		return bit_pointer(_ref._ptr + diff, static_cast<size_type>(sum - diff * digits));
	}
	JASEL_CXX14_CONSTEXPR bit_pointer operator-(difference_type n) const
	{
		constexpr difference_type digits = binary_digits<word_type>::value;
		const difference_type     sum    = static_cast<difference_type>(_ref.position()) - n;
		difference_type           diff   = sum / digits;
		if (sum < 0 && diff * digits != sum)
		{
			--diff;
		}
		return bit_pointer(_ref._ptr + diff, static_cast<size_type>(sum - diff * digits));
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator+=(difference_type n)
	{
		*this = *this + n;
		return *this;
	}
	JASEL_MUTABLE_CONSTEXPR bit_pointer &operator-=(difference_type n)
	{
		*this = *this - n;
		return *this;
	}

	// Implementation details: data members
private:
	bit_reference<WordType> _ref;

public:
	// Non-member arithmetic operators
#ifdef __clang__
	template <class T>
	friend constexpr bit_pointer<T> operator+(
	        typename bit_pointer<T>::difference_type n,
	        bit_pointer<T>                           x)
#else
	friend constexpr bit_pointer operator+(
	        difference_type n,
	        bit_pointer     x)
#endif
	{
		return x + n;
	}

#ifdef __clang__
	template <class T, class U>
	friend JASEL_CXX14_CONSTEXPR typename common_type<
	        typename bit_pointer<T>::difference_type,
	        typename bit_pointer<U>::difference_type>::type
	operator-(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs)
	{
		using lhs_type                       = typename bit_pointer<T>::difference_type;
		using rhs_type                       = typename bit_pointer<U>::difference_type;
		using difference_type                = typename common_type<lhs_type, rhs_type>::type;
		constexpr difference_type lhs_digits = binary_digits<T>::value;
		constexpr difference_type rhs_digits = binary_digits<U>::value;
		constexpr difference_type digits     = rhs_digits;
		static_assert(lhs_digits == rhs_digits, "");
		const difference_type main = lhs._ref.address() - rhs._ref.address();
		return main * digits + static_cast<difference_type>(lhs._ref.position() - rhs._ref.position());
	}
#else
	friend JASEL_CXX14_CONSTEXPR typename common_type<
	        difference_type,
	        difference_type>::type
	operator-(
	        bit_pointer lhs,
	        bit_pointer rhs)
	{
		constexpr difference_type digits = binary_digits<word_type>::value;
		const difference_type     main   = lhs._ref.address() - rhs._ref.address();
		return main * digits + (lhs._ref.position() - rhs._ref.position());
	}
#endif

	// Comparison operators
#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator==(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator==(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept
#endif
	{
		return lhs._ref.address() == rhs._ref.address() && lhs._ref.position() == rhs._ref.position();
	}

#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator!=(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator!=(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept
#endif
	{
		return lhs._ref.address() != rhs._ref.address() || lhs._ref.position() != rhs._ref.position();
	}

#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator<(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator<(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept
#endif
	{
		return lhs._ref.address() < rhs._ref.address() || (lhs._ref.address() == rhs._ref.address() && lhs._ref.position() < rhs._ref.position());
	}

#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator<=(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator<=(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept
#endif
	{
		return lhs._ref.address() < rhs._ref.address() || (lhs._ref.address() == rhs._ref.address() && lhs._ref.position() <= rhs._ref.position());
	}

#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator>(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator>(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept

#endif
	{
		return lhs._ref.address() > rhs._ref.address() || (lhs._ref.address() == rhs._ref.address() && lhs._ref.position() > rhs._ref.position());
	}

#ifdef __clang__
	template <class T, class U>
	friend constexpr bool operator>=(
	        bit_pointer<T> lhs,
	        bit_pointer<U> rhs) noexcept
#else
	friend constexpr bool operator>=(
	        bit_pointer lhs,
	        bit_pointer rhs) noexcept
#endif
	{
		return lhs._ref.address() > rhs._ref.address() || (lhs._ref.address() == rhs._ref.address() && lhs._ref.position() >= rhs._ref.position());
	}
};

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
