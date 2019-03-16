// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STATIC_VECTOR_STATIC_VECTOR_HPP
#define JASEL_EXPERIMENTAL_STATIC_VECTOR_STATIC_VECTOR_HPP

#if __cplusplus >= 201103L

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/contract/constexpr_assert.hpp>
#include <experimental/fundamental/v3/utility/length_tag.hpp>
#include <experimental/type_traits.hpp>
#include <algorithm>
#include <cassert>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

template <typename T, size_t N>
class static_vector {
public:
	// types:
	using value_type      = T;
	using pointer         = T *;
	using const_pointer   = const T *;
	using reference       = value_type &;
	using const_reference = const value_type &;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;
	using iterator        = pointer;       // see [container.requirements]
	using const_iterator  = const_pointer; // see [container.requirements]
	//struct iterator;
	//struct const_iterator;

	using reverse_iterator       = std::reverse_iterator<iterator>;
	using const_reverse_iterator = std::reverse_iterator<const_iterator>;

private:
	using aligned_storage_t    = aligned_storage_for_t<remove_const_t<T>>;
	using element_storage_type = conditional_t<is_const<T>::value, aligned_storage_t const, aligned_storage_t>;
	alignas(alignof(T)) element_storage_type data_[N];
	size_t      size_;
	void const *data_ptr(size_t i) const
	{
		return &data_[i];
	}
	void *data_ptr(size_t i)
	{
		return &data_[i];
	}
	value_type *element_ptr(size_t i)
	{
		JASEL_EXPECTS(size() > i && "try to access with index out of range");
		return reinterpret_cast<value_type *>(data_ptr(i));
	}
	value_type const *element_ptr(size_t i) const
	{
		JASEL_EXPECTS(size() > i && "try to access with index out of range");
		return reinterpret_cast<value_type const *>(data_ptr(i));
	}

	template <class... Args>
	JASEL_CXX14_CONSTEXPR void construct(size_t index, Args &&... args)
	{
		new (data_ptr(index)) T(forward<Args>(args)...);
	}
	JASEL_CXX14_CONSTEXPR void destruct(size_t index)
	{
		element_ptr(index)->~T();
	}
	template <class It>
	JASEL_CXX14_CONSTEXPR void destruct(It first, It last)
	{
		for (; first < last; ++first)
			first->~T();
	}

	template <typename It>
	constexpr bool iterator_in_range(It it) noexcept
	{
		//static_assert(is_iterator_v<It>);
		return (begin() <= it) && (it <= end());
	}

	template <typename It0, typename It1>
	constexpr bool valid_iterator_range(It0 first,
	                                    It1 last) noexcept
	{
		//static_assert(is_iterator_v<It0>);
		//static_assert(is_iterator_v<It1>);
		return (first <= last);
	}

public:
	// 5.2, copy/move construction:
	/**
        Effects: Constructs an empty static_vector.
        Ensures: empty().
        Complexity: Constant.
     */
	constexpr static_vector() noexcept
	        : size_(0) {}
	JASEL_CXX14_CONSTEXPR static_vector(const static_vector &other)
	        : size_(0)
	{
		insert(begin(), other.begin(), other.end());
		// for (size_ = 0; size_ < other.size(); ++size_)
		// {
		// 	construct(size_, other[size_]);
		// }
	}
	JASEL_CXX14_CONSTEXPR static_vector(static_vector &&other)
	        : size_(0)
	{
		move_insert(begin(), other.begin(), other.end());
		// for (size_ = 0; size_ < other.size(); ++size_)
		// {
		// 	construct(size_, move(other[size_]));
		// }
	}
	constexpr explicit static_vector(length_tag_t tag, size_type n)
	        : static_vector(tag, n, T())
	{
	}
	JASEL_CXX14_CONSTEXPR static_vector(length_tag_t tag, size_type n, const value_type &value)
	        : size_(0)
	{
		insert(begin(), tag, n, value);
		// JASEL_EXPECTS(n <= capacity() && "try to insert beyond capacity");
		// for (size_ = 0; size_ < n; ++size_)
		// {
		// 	construct(size_, value);
		// }
	}

	template <class InputIterator>
	// requires IsInputIterator<InputIterator>
	JASEL_CXX14_CONSTEXPR static_vector(InputIterator first, InputIterator last)
	        : size_(0)
	{
		insert(begin(), first, last);
		// for (size_ = 0; first < last; ++first, ++size_)
		// {
		// 	construct(size_, *first);
		// 	JASEL_EXPECTS(size_ < capacity() && "try to insert beyond capacity");
		// }
	}

	constexpr static_vector(initializer_list<value_type> il)
	        : static_vector(il.begin(), il.end())
	{
	}

	// 5.3, copy/move assignment:

	//! Replaces the contents of the container.

	JASEL_CXX14_CONSTEXPR static_vector &operator=(const static_vector &other)
	{
		if (this == &other)
			return *this;
		// fixme: Could we optimize it by limiting the destructions and constructions, and using assignments when possible?
		assign(other.begin(), other.end());

		return *this;
	}
	JASEL_CXX14_CONSTEXPR static_vector &operator=(static_vector &&other)
	{
		if (this == &other)
			return *this;

		clear();
		move_insert(this->begin(), other.begin(), other.end());

		return *this;
	}

	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR void assign(InputIterator first, InputIterator last)
	{
		clear();
		insert(this->begin(), first, last);
	}
	JASEL_CXX14_CONSTEXPR void assign(length_tag_t tag, size_type n, const value_type &u)
	{
		clear();
		insert(this->begin(), tag, n, u);
	}

	JASEL_CXX14_CONSTEXPR void assign(initializer_list<value_type> il)
	{
		clear();
		insert(this->begin(), il);
	}

	// 5.4, destruction
	~static_vector()
	{
		clear();
	}

	// iterators
	JASEL_CXX14_CONSTEXPR iterator begin() noexcept
	{
		return data();
	}
	JASEL_CXX14_CONSTEXPR const_iterator begin() const noexcept
	{
		return data();
	}
	JASEL_CXX14_CONSTEXPR iterator end() noexcept
	{
		return data() + size();
	}
	JASEL_CXX14_CONSTEXPR const_iterator end() const noexcept
	{
		return data() + size();
	}
	JASEL_CXX14_CONSTEXPR reverse_iterator rbegin() noexcept
	{
		return reverse_iterator(end());
	}
	JASEL_CXX14_CONSTEXPR const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}
	JASEL_CXX14_CONSTEXPR reverse_iterator rend() noexcept
	{
		return reverse_iterator(begin());
	}
	JASEL_CXX14_CONSTEXPR const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}
	JASEL_CXX14_CONSTEXPR const_iterator cbegin() noexcept
	{
		return begin();
	}
	JASEL_CXX14_CONSTEXPR const_iterator cend() const noexcept
	{
		return end();
	}
	JASEL_CXX14_CONSTEXPR const_reverse_iterator crbegin() noexcept
	{
		return const_reverse_iterator(end());
	}
	JASEL_CXX14_CONSTEXPR const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(begin());
	}

	// 5.5, size/capacity:
	constexpr bool full() const noexcept
	{
		return size() == capacity();
	}

	constexpr bool empty() const noexcept
	{
		return size() == 0;
	}

	constexpr size_type size() const noexcept
	{
		return size_;
	}

	static constexpr size_type max_size() noexcept
	{
		return N;
	}
	static constexpr size_type capacity() noexcept
	{
		return N;
	}
	JASEL_CXX14_CONSTEXPR void resize(size_type sz)
	{
		resize(sz, T());
	}
	JASEL_CXX14_CONSTEXPR void resize(size_type sz, const value_type &c)
	{
		if (sz < size())
			erase(element_ptr(sz), end());
		else
			insert(end(), sz - size(), c);
	}

	// 5.6, element and data access:

	JASEL_CXX14_CONSTEXPR reference operator[](size_type n)
	{
		return *element_ptr(n);
	}
	JASEL_CXX14_CONSTEXPR const_reference operator[](size_type n) const
	{
		return *element_ptr(n);
	}
	JASEL_CXX14_CONSTEXPR reference front()
	{
		return *element_ptr(0);
	}

	JASEL_CXX14_CONSTEXPR const_reference front() const
	{
		return *element_ptr(0);
	}
	JASEL_CXX14_CONSTEXPR reference back()
	{
		return *element_ptr(size() - 1);
	}
	JASEL_CXX14_CONSTEXPR const_reference back() const
	{
		return *element_ptr(size() - 1);
	}
	JASEL_CXX14_CONSTEXPR T *data() noexcept
	{
		return reinterpret_cast<value_type *>(data_ptr(0));
	}
	JASEL_CXX14_CONSTEXPR const T *data() const noexcept
	{
		return reinterpret_cast<value_type const *>(data_ptr(0));
	}

	// 5.7, modifiers:
	constexpr iterator insert(const_iterator position, const value_type &value)
	{
		return insert(position, length_tag_t{}, size_type(1), value);
	}

	constexpr iterator insert(const_iterator position, value_type &&value)
	{
		return move_insert(position, &value, &value + 1);
	}

	JASEL_CXX14_CONSTEXPR iterator insert(const_iterator position, length_tag_t, size_type n, const value_type &x)
	{
		JASEL_EXPECTS(n <= capacity() - size() && "try to insert beyond capacity");
		// memorize the new first
		auto new_first = end();
		// emplace back the elements to insert
		while (n != 0)
		{
			push_back(x);
			--n;
		}
		// make the pointer writable
		auto writable_position = begin() + (position - begin());
		// rotate the added elements
		rotate(writable_position, new_first, end());
		return writable_position;
	}
	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR iterator insert(const_iterator position, InputIterator first, InputIterator last)
	{
		JASEL_EXPECTS(iterator_in_range(position) && "try to insert in position out of range");
		JASEL_EXPECTS(valid_iterator_range(first, last) && "try to insert invalid iterator");
		JASEL_EXPECTS(distance(first, last) <= capacity() - size() && "try to insert beyond capacity");
		// memorize the new first
		auto new_first = end();
		// emplace back the elements to insert
		for (; first < last; ++first)
		{
			emplace_back(*first);
		}
		// make the pointer writable
		auto writable_position = begin() + (position - begin());
		// rotate the added elements
		rotate(writable_position, new_first, end());
		return writable_position;
	}
	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR iterator move_insert(const_iterator position, InputIterator first, InputIterator last)
	{
		JASEL_EXPECTS(iterator_in_range(position) && "try to insert in position out of range");
		JASEL_EXPECTS(valid_iterator_range(first, last) && "try to insert invalid iterator");
		JASEL_EXPECTS(distance(first, last) <= capacity() - size() && "try to insert beyond capacity");
		// memorize the new first
		auto new_first = end();
		// emplace back the elements to insert
		for (; first < last; ++first)
		{
			emplace_back(move(*first));
		}
		// make the pointer writable
		auto writable_position = begin() + (position - begin());
		// rotate the added elements
		rotate(writable_position, new_first, end());
		return writable_position;
	}
	JASEL_CXX14_CONSTEXPR iterator insert(const_iterator position, initializer_list<value_type> il)
	{
		return insert(position, il.begin(), il.end());
	}

	template <class... Args>
	JASEL_CXX14_CONSTEXPR iterator emplace(const_iterator position, Args &&... args)
	{
		JASEL_EXPECTS(!full() && "try to emplace in a full collection");
		JASEL_EXPECTS(iterator_in_range(position) && "try to emplace in position out of range");

		// memorize the new first
		auto new_first = end();
		// emplace back the elements to insert
		emplace_back(forward<Args>(args)...);
		// make the pointer writable
		auto writable_position = begin() + (position - begin());
		// rotate the added elements
		rotate(writable_position, new_first, end());
		return writable_position;
	}

	template <class... Args>
	JASEL_CXX14_CONSTEXPR reference emplace_back(Args &&... args)
	{
		JASEL_EXPECTS(!full() && "try to emplace_back in a full collection");
		construct(size_, forward<Args>(args)...);
		++size_;
		return back();
	}
	JASEL_CXX14_CONSTEXPR void push_back(const value_type &x)
	{
		JASEL_EXPECTS(!full() && "try to push_back in a full collection");
		construct(size_, x);
		++size_;
	}

	JASEL_CXX14_CONSTEXPR void push_back(value_type &&x)
	{
		JASEL_EXPECTS(!full() && "try to push_back in a full collection");
		construct(size_, move(x));
		++size_;
	}

	JASEL_CXX14_CONSTEXPR void pop_back()
	{
		JASEL_EXPECTS(!empty() && "try to pop_back in an empty collection");
		destruct(size_ - 1);
		--size_;
	}

	JASEL_CXX14_CONSTEXPR iterator erase(const_iterator position)
	{
		return erase(position, position + 1);
	}

	JASEL_CXX14_CONSTEXPR iterator erase(const_iterator first, const_iterator last)
	{
		JASEL_EXPECTS(iterator_in_range(first));
		JASEL_EXPECTS(iterator_in_range(last));
		JASEL_EXPECTS(valid_iterator_range(first, last));

		iterator p = begin() + (first - begin());
		if (first != last)
		{
			destruct(
			        move(p + (last - first), end(), p), end());
			size_ = (size() - static_cast<size_type>(last - first));
		}

		return p;
	}

	JASEL_CXX14_CONSTEXPR void clear() noexcept
	{
		for (; size_ > 0; size_--)
		{
			destruct(size_ - 1);
		}
	}

	JASEL_CXX14_CONSTEXPR void swap(static_vector &other)
	//noexcept(is_nothrow_swappable<value_type>::value && is_nothrow_move_constructible<value_type>::value)
	{
		static_vector tmp = move(other);
		other             = move(*this);
		(*this)           = move(tmp);
	}
};

template <typename T, size_t N>
constexpr bool operator==(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return equal(a.begin(), a.end(), b.begin());
}
template <typename T, size_t N>
constexpr bool operator!=(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return !(a == b);
}

template <typename T, size_t N>
constexpr bool operator<(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, size_t N>
constexpr bool operator<=(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return !(b < a);
}

template <typename T, size_t N>
constexpr bool operator>(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return b < a;
}

template <typename T, size_t N>
constexpr bool operator>=(const static_vector<T, N> &a, const static_vector<T, N> &b)
{
	return !(a < b);
}

// 5.8, specialized algorithms:
template <typename T, size_t N>
JASEL_CXX14_CONSTEXPR void swap(static_vector<T, N> &x, static_vector<T, N> &y) noexcept(noexcept(x.swap(y)))
{
	x.swap(y);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif
#endif // header
