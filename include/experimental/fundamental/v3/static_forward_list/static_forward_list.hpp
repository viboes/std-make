// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_STATIC_FORWARD_LIST_FORWARD_LIST_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_STATIC_FORWARD_LIST_FORWARD_LIST_HPP

#if __cplusplus >= 201103L

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/contract/constexpr_assert.hpp>
#include <experimental/type_traits.hpp>
#include <algorithm>
#include <cassert>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
struct length_tag2_t
{
};

template <typename T, size_t N>
class static_forward_list {
public:
	// types:
	using value_type      = T;
	using pointer         = T *;
	using const_pointer   = const T *;
	using reference       = value_type &;
	using const_reference = const value_type &;
	using size_type       = size_t;
	using difference_type = ptrdiff_t;

private:
	using aligned_storage_t    = aligned_storage_for_t<T>;
	using element_storage_type = conditional_t<is_const<T>::value, aligned_storage_t const, aligned_storage_t>;
	struct node
	{
		node *               next_;
		element_storage_type storage_;
	};
	struct node_iterator
	{
		node *ptr_;

		node_iterator() : ptr_(nullptr) {}
		node_iterator(node *it) : ptr_(it) {}
		node_iterator(node_iterator const &) = default;
		node_iterator &operator=(node_iterator const &) = default;

		node_iterator &operator++()
		{
			ptr_ = ptr_->next_;
			return *this;
		}
		node_iterator operator++(int)
		{
			node_iterator tmp = *this;
			ptr_              = ptr_->next_;
			return tmp;
		}

		T const *   operator->() const { return reinterpret_cast<T const *>(&ptr_->storage_); }
		T *         operator->() { return reinterpret_cast<T *>(&ptr_->storage_); }
		T const &   operator*() const { return reinterpret_cast<T const &>(ptr_->storage_); }
		T &         operator*() { return reinterpret_cast<T &>(ptr_->storage_); }
		friend bool operator==(node_iterator const &x, node_iterator const &y)
		{
			return x.ptr_ == y.ptr_;
		}
		friend bool operator!=(node_iterator const &x, node_iterator const &y)
		{
			return x.ptr_ != y.ptr_;
		}
	};

	struct const_node_iterator
	{
		node *ptr_;

		const_node_iterator() : ptr_(nullptr) {}
		const_node_iterator(node const *it) : ptr_(it) {}
		const_node_iterator(node_iterator const it) : ptr_(it.ptr_) {}

		T const *operator->() const { return &ptr_->value; }
		T const *operator->() { return &ptr_->value; }
		T const &operator*() const { return ptr_->value; }
		T const &operator*() { return ptr_->value; }

		friend bool operator==(const_node_iterator const &x, const_node_iterator const &y)
		{
			return x.ptr_ == y.ptr_;
		}
		friend bool operator!=(const_node_iterator const &x, const_node_iterator const &y)
		{
			return x.ptr_ != y.ptr_;
		}
	};

public:
	using iterator       = node_iterator;       // see [container.requirements]
	using const_iterator = const_node_iterator; // see [container.requirements]

private:
	static constexpr std::size_t alignment = (alignof(node) > alignof(T)) ? alignof(node) : alignof(T);
	alignas(alignment) node data_[N + 1];
	node        head_;
	size_t      size_;
	node *&     head_free() { return data_[0].next_; };
	void const *data_ptr(node const *n) const
	{
		return &n->storage_;
	}
	void *data_ptr(node *n)
	{
		return &n->storage_;
	}
	value_type *element_ptr(node *n)
	{
		JASEL_EXPECTS(n != nullptr && "try to access with index out of range");
		return reinterpret_cast<value_type *>(data_ptr(n));
	}
	value_type const *element_ptr(node const *n) const
	{
		JASEL_EXPECTS(n != nullptr && "try to access with index out of range");
		return reinterpret_cast<value_type const *>(data_ptr(n));
	}

	template <class... Args>
	JASEL_CXX14_CONSTEXPR void construct(node *n, Args &&... args)
	{
		new (data_ptr(n)) T(forward<Args>(args)...);
	}
	JASEL_CXX14_CONSTEXPR void destruct(node *n)
	{
		element_ptr(n)->~T();
	}
	// template <class It>
	// JASEL_CXX14_CONSTEXPR void destruct(It first, It last)
	// {
	// 	for (; first < last; ++first)
	// 		first->~T();
	// }

	// template <typename It>
	// constexpr bool iterator_in_range(It it) noexcept
	// {
	// 	//static_assert(is_iterator_v<It>);
	// 	return (begin() <= it) && (it <= end());
	// }

	// template <typename It0, typename It1>
	// constexpr bool valid_iterator_range(It0 first,
	//                                     It1 last) noexcept
	// {
	// 	//static_assert(is_iterator_v<It0>);
	// 	//static_assert(is_iterator_v<It1>);
	// 	return (first <= last);
	// }

public:
	// 5.2, copy/move construction:
	/**
	    Effects: Constructs an empty static_forward_list.
	    Ensures: empty().
	    Complexity: Constant.
	 */
	JASEL_CXX14_CONSTEXPR static_forward_list() noexcept
	        : size_(0)
	{
		for (int i = 0; i < N; i++)
		{

			data_[i].next_ = &data_[i + 1];
		}

		data_[N].next_ = nullptr;
		head_.next_    = nullptr;
	}

	JASEL_CXX14_CONSTEXPR static_forward_list(const static_forward_list &other)
	        : size_(0)
	{
		insert_after(before_begin(), other.begin(), other.end());
	}
	JASEL_CXX14_CONSTEXPR static_forward_list(static_forward_list &&other)
	        : size_(0)
	{
		move_insert_after(before_begin(), other.begin(), other.end());
	}
	constexpr explicit static_forward_list(length_tag2_t tag, size_type n)
	        : static_forward_list(tag, n, T())
	{
	}
	JASEL_CXX14_CONSTEXPR static_forward_list(length_tag2_t tag, size_type n, const value_type &value)
	        : size_(0)
	{
		insert_after(before_begin(), tag, n, value);
	}

	template <class InputIterator>
	// requires IsInputIterator<InputIterator>
	JASEL_CXX14_CONSTEXPR static_forward_list(InputIterator first, InputIterator last)
	        : size_(0)
	{
		insert_after(before_begin(), first, last);
	}

	constexpr static_forward_list(initializer_list<value_type> il)
	        : static_forward_list(il.begin(), il.end())
	{
	}

	// 5.3, copy/move assignment:

	//! Replaces the contents of the container.

	JASEL_CXX14_CONSTEXPR static_forward_list &operator=(const static_forward_list &other)
	{
		if (this == &other)
			return *this;
		// fixme: Could we optimize it by limiting the destructions and constructions, and using assignments when possible?
		assign(other.begin(), other.end());

		return *this;
	}
	JASEL_CXX14_CONSTEXPR static_forward_list &operator=(static_forward_list &&other)
	{
		if (this == &other)
			return *this;

		clear();
		move_insert_after(this->before_begin(), other.begin(), other.end());

		return *this;
	}

	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR void assign(InputIterator first, InputIterator last)
	{
		clear();
		insert_after(this->before_begin(), first, last);
	}
	JASEL_CXX14_CONSTEXPR void assign(length_tag2_t tag, size_type n, const value_type &u)
	{
		clear();
		insert_after(this->before_begin(), tag, n, u);
	}

	JASEL_CXX14_CONSTEXPR void assign(initializer_list<value_type> il)
	{
		clear();
		insert_after(this->before_begin(), il);
	}

	// 5.4, destruction
	~static_forward_list()
	{
		clear();
	}

	// iterators
	JASEL_CXX14_CONSTEXPR iterator begin() noexcept
	{
		return head_.next_;
	}
	JASEL_CXX14_CONSTEXPR const_iterator begin() const noexcept
	{
		return head_.next_;
	}
	JASEL_CXX14_CONSTEXPR iterator end() noexcept
	{
		return nullptr;
	}
	JASEL_CXX14_CONSTEXPR const_iterator end() const noexcept
	{
		return nullptr;
	}

	JASEL_CXX14_CONSTEXPR const_iterator cbegin() noexcept
	{
		return head_.next_;
	}
	JASEL_CXX14_CONSTEXPR const_iterator cend() const noexcept
	{
		return nullptr;
	}
	iterator before_begin() noexcept
	{
		return &head_;
	}
	const_iterator before_begin() const noexcept
	{
		return &head_;
	}
	const_iterator cbefore_begin() const noexcept
	{
		return &head_;
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
	// JASEL_CXX14_CONSTEXPR void resize(size_type sz)
	// {
	// 	resize(sz, T());
	// }
	// JASEL_CXX14_CONSTEXPR void resize(size_type sz, const value_type &c)
	// {
	// 	if (sz < size())
	// 		erase_after(element_ptr(sz), end());
	// 	else
	// 		insert_after(end(), sz - size(), c);
	// }

	// // 5.6, element and data access:

	JASEL_CXX14_CONSTEXPR reference front()
	{
		//JASEL_EXPECTS(!empty() && "try to access front on an empty collection");
		return *element_ptr(head_.next_);
	}

	JASEL_CXX14_CONSTEXPR const_reference front() const
	{
		//JASEL_EXPECTS(!empty() && "try to access front on an empty collection");
		return *element_ptr(head_.next_);
	}

	// 5.7, modifiers:
	JASEL_CXX14_CONSTEXPR iterator insert_after(const_iterator position, const value_type &value)
	{
		return insert_after(position, length_tag2_t{}, size_type(1), value);
	}

	JASEL_CXX14_CONSTEXPR iterator insert_after(const_iterator position, value_type &&value)
	{
		return move_insert_after(position, &value, &value + 1);
	}

	JASEL_CXX14_CONSTEXPR iterator _insert_first_n_nodes_until_after(size_t num, node *last_updated, const_iterator position)
	{
		node *new_first_free = last_updated->next_;
		node *first_updated  = data_[0].next_;
		node *after_position = position.ptr_->next_;

		position.ptr_->next_ = first_updated;
		last_updated->next_  = after_position;
		data_[0].next_       = new_first_free;
		size_ += num;
		return last_updated;
	}

	JASEL_CXX14_CONSTEXPR iterator insert_after(const_iterator position, length_tag2_t, size_type n, const value_type &x)
	{
		JASEL_EXPECTS(n <= capacity() - size() && "try to insert beyond capacity");

		// emplace back the elements to insert
		node *          last_busy_m_1 = &data_[0];
		size_type const num           = n;

		while (n != 0)
		{
			JASEL_ASSERT(last_busy_m_1->next_ != nullptr && "This must be non null");
			construct(last_busy_m_1->next_, x);
			--n;
			last_busy_m_1 = last_busy_m_1->next_;
		}
#if 1
		return _insert_first_n_nodes_until_after(num, last_busy_m_1, position);
#else
		node *last_busy         = last_busy_m_1->next_;
		node *writable_position = data_[0].next_;
		node *after_position    = position.ptr_->next_;

		position.ptr_->next_ = writable_position;
		last_busy_m_1->next_ = after_position;
		data_[0].next_       = last_busy;
		size_ += num;

		return last_busy_m_1;
#endif
	}
	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR iterator insert_after(const_iterator position, InputIterator first, InputIterator last)
	{
		//JASEL_EXPECTS(iterator_in_range(position) && "try to insert in position out of range");
		//JASEL_EXPECTS(valid_iterator_range(first, last) && "try to insert invalid iterator");
		JASEL_EXPECTS(distance(first, last) <= capacity() - size() && "try to insert beyond capacity");

		node *    last_busy_m_1 = &data_[0];
		size_type num           = 0;

		// emplace back the elements to insert
		while (first != last)
		{
			construct(last_busy_m_1->next_, *first);
			++num;
			last_busy_m_1 = last_busy_m_1->next_;
			++first;
		}
#if 1
		return _insert_first_n_nodes_until_after(num, last_busy_m_1, position);
#else
		node *last_busy         = last_busy_m_1->next_;
		node *writable_position = data_[0].next_;
		node *after_position    = position.ptr_->next_;

		position.ptr_->next_ = writable_position;
		last_busy_m_1->next_ = after_position;
		data_[0].next_       = last_busy;
		size_ += num;

		return last_busy_m_1;
#endif
	}
	template <class InputIterator>
	JASEL_CXX14_CONSTEXPR iterator move_insert_after(const_iterator position, InputIterator first, InputIterator last)
	{
		//JASEL_EXPECTS(iterator_in_range(position) && "try to insert in position out of range");
		//JASEL_EXPECTS(valid_iterator_range(first, last) && "try to insert invalid iterator");
		JASEL_EXPECTS(distance(first, last) <= capacity() - size() && "try to insert beyond capacity");
		node *    last_busy_m_1 = &data_[0];
		size_type num           = 0;

		// emplace back the elements to insert
		while (first != last)
		{
			construct(last_busy_m_1->next_, move(*first));
			++num;
			last_busy_m_1 = last_busy_m_1->next_;
			++first;
		}
#if 1
		return _insert_first_n_nodes_until_after(num, last_busy_m_1, position);
#else

		node *last_busy         = last_busy_m_1->next_;
		node *writable_position = data_[0].next_;
		node *after_position    = position.ptr_->next_;

		position.ptr_->next_ = writable_position;
		last_busy_m_1->next_ = after_position;
		data_[0].next_       = last_busy;
		size_ += num;

		return last_busy_m_1;
#endif
	}
	JASEL_CXX14_CONSTEXPR iterator insert_after(const_iterator position, initializer_list<value_type> il)
	{
		return insert(position, il.begin(), il.end());
	}

	// template <class... Args>
	// JASEL_CXX14_CONSTEXPR iterator emplace_after(const_iterator position, Args &&... args)
	// {
	// 	JASEL_EXPECTS(!full() && "try to emplace in a full collection");
	// 	JASEL_EXPECTS(iterator_in_range(position) && "try to emplace in position out of range");

	// 	// memorize the new first
	// 	auto new_first = end();
	// 	// emplace back the elements to insert
	// 	emplace_back(forward<Args>(args)...);
	// 	// make the pointer writable
	// 	auto writable_position = begin() + (position - begin());
	// 	// rotate the added elements
	// 	rotate(writable_position, new_first, end());
	// 	return writable_position;
	// }

	template <class... Args>
	JASEL_CXX14_CONSTEXPR reference emplace_front(Args &&... args)
	{
		return *emplace_after(this->before_begin(), std::forward<Args>(args)...);
	}

	JASEL_CXX14_CONSTEXPR void move_to_busy(node *n)
	{
		auto next_free = n->next_;
		n->next_       = head_.next_;
		head_.next_    = n;
		data_[0].next_ = next_free;
		++size_;
	}
	JASEL_CXX14_CONSTEXPR void move_to_free(node *n)
	{
		auto nextbusy  = n->next_;
		n->next_       = data_[0].next_->next_;
		data_[0].next_ = n;
		head_.next_    = nextbusy;
		--size_;
	}
	JASEL_CXX14_CONSTEXPR void push_front(const value_type &x)
	{
		JASEL_EXPECTS(!full() && "try to push_back in a full collection");
		auto n = data_[0].next_;
		construct(n, x);
		move_to_busy(n);
	}

	JASEL_CXX14_CONSTEXPR void push_front(value_type &&x)
	{
		JASEL_EXPECTS(!full() && "try to push_back in a full collection");
		auto n = data_[0].next_;
		construct(n, move(x));
		move_to_busy(n);
	}

	JASEL_CXX14_CONSTEXPR void pop_front()
	{
		JASEL_EXPECTS(!empty() && "try to pop_back in an empty collection");
		auto n = head_.next_;
		destruct(n);
		auto nextbusy  = n->next_;
		n->next_       = data_[0].next_->next_;
		data_[0].next_ = n;
		head_.next_    = nextbusy;
		--size_;
	}

	JASEL_CXX14_CONSTEXPR iterator erase_after(const_iterator position)
	{
		return erase(position, position + 1);
	}

	JASEL_CXX14_CONSTEXPR iterator erase_after(const_iterator first, const_iterator last)
	{
		//JASEL_EXPECTS(iterator_in_range(first));
		//JASEL_EXPECTS(iterator_in_range(last));
		//JASEL_EXPECTS(valid_iterator_range(first, last));

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
		for (iterator it = begin(); it != end(); it++)
		{
			destruct(it.ptr_);
		}
	}

	JASEL_CXX14_CONSTEXPR void swap(static_forward_list &other)
	//noexcept(is_nothrow_swappable<value_type>::value && is_nothrow_move_constructible<value_type>::value)
	{
		static_forward_list tmp = move(other);
		other                   = move(*this);
		(*this)                 = move(tmp);
	}

	// Operations
#if 0
	// static_forward_list owns his elements and cannot support neither merge, nor splice operations without copying.
	// While these operations are less expensive to implement for static_forward_list than for contiguous containers 
	// it is however much more expensive than for std::forward_list and std::list.
	// This is why we don't implement them here

	void merge(static_forward_list &other);
	void merge(static_forward_list &&other);
	template <class Compare>
	void merge(static_forward_list &other, Compare comp);
	template <class Compare>
	void merge(static_forward_list &&other, Compare comp);

	void splice_after(const_iterator pos, static_forward_list &other);
#endif

	size_type remove(const T &value)
	{
		remove_if([&value](T const &v) { return v == value; });
	}
	template <class UnaryPredicate>
	size_type remove_if(UnaryPredicate p);

	void reverse() noexcept;

	size_type unique()
	{
		return unique([](T const &x, T const &y) { return x == y; });
	}

	template <class BinaryPredicate>
	size_type unique(BinaryPredicate p);

	void sort()
	{
		return sort([](T const &x, T const &y) { return x < y; });
	}

	template <class Compare>
	void sort(Compare comp);
};

template <typename T, size_t N>
constexpr bool operator==(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return equal(a.begin(), a.end(), b.begin());
}
template <typename T, size_t N>
constexpr bool operator!=(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return !(a == b);
}

template <typename T, size_t N>
constexpr bool operator<(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return lexicographical_compare(a.begin(), a.end(), b.begin(), b.end());
}

template <typename T, size_t N>
constexpr bool operator<=(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return !(b < a);
}

template <typename T, size_t N>
constexpr bool operator>(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return b < a;
}

template <typename T, size_t N>
constexpr bool operator>=(const static_forward_list<T, N> &a, const static_forward_list<T, N> &b)
{
	return !(a < b);
}

// 5.8, specialized algorithms:
template <typename T, size_t N>
JASEL_CXX14_CONSTEXPR void swap(static_forward_list<T, N> &x, static_forward_list<T, N> &y) noexcept(noexcept(x.swap(y)))
{
	x.swap(y);
}

template <class T, size_t N, class U>
void erase(static_forward_list<T, N> &c, const U &value)
{
	c.remove_if([&](T const &elem) { return elem == value; });
}

template <class T, size_t N, class Pred>
void erase_if(static_forward_list<T, N> &c, Pred pred)
{
	c.remove_if(pred);
}

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif
#endif // header
