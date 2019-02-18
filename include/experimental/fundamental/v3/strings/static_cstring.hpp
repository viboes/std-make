// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1072r3.html for resize_defalt_init motivation
// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1402r0.pdf for a c-string \0 justification
// See http : //www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0259r0.pdf for a motivation of a fixed_string
// See http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p0843r3.html for a justification for a static vector

#ifndef JASEL_EXPERIMENTAL_STRINGS_STATIC_CSTRING_HPP
#define JASEL_EXPERIMENTAL_STRINGS_STATIC_CSTRING_HPP

#include <experimental/contract.hpp>
#include <experimental/cstr_view.hpp>
#include <experimental/cstring_view.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/fundamental/v3/strings/null_terminated.hpp>
#include <experimental/utility.hpp>
#include <array>
#include <iterator>
#include <stdexcept>
#include <string>

#include <utility>

#if __cplusplus > 201402L
#include <string_view>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

//! std::string like class with fixed capacity at compile time.
//! static_cstring is to std::string what static_vector is to std::vector.
//! In order to make operations as efficient as possible it stores like std::string the length of the string
//! As std::string stores as well the zero identifying a null terminated string, and so the storage with be CharT [N+1]
template <class CharT, size_t N, class SizeType = size_t, class Traits = char_traits<CharT>>
class basic_static_cstring;

template <class CharT, size_t N, class SizeType, class Traits>
class basic_static_cstring {
	//! checks is the stored string is null terminated
	constexpr bool null_terminated() noexcept
	{
		return length() <= (N + 1) && ntxs::valid(data(), length());
	}

public:
	// types
	using traits_type            = Traits;
	using value_type             = CharT;
	using pointer                = value_type *;
	using const_pointer          = const value_type *;
	using reference              = value_type &;
	using const_reference        = const value_type &;
	using iterator               = value_type *;
	using const_iterator         = value_type const *;
	using const_reverse_iterator = reverse_iterator<const_iterator>;
	using reverse_iterator       = const_reverse_iterator;
	using size_type              = SizeType;
	using difference_type        = ptrdiff_t;
	using string_view_type       = basic_string_view<CharT, Traits>;
#if __cplusplus > 201402L
	using cstring_view_type = basic_cstring_view<CharT, Traits>;
#endif
	using cstr_view_type = basic_cstr_view<CharT, Traits>;
	using string_type    = basic_string<CharT, Traits>;

	static constexpr size_type npos = size_type(-1);

	// [string.static_cstring], construction and assignment

	//! constructs a NTXS of length 0
	//! ensures empty()
	JASEL_CXX14_CONSTEXPR basic_static_cstring() noexcept
	        : len_(0)
	{
		data_[0] = null_terminated_traits<CharT>::zero;
	}

	//! Added a length_tag_t to have less surprises
	//! static_cstring str(48, 'X'); will not compile, while
	//! static_cstring str{48, 'X'};
	JASEL_CXX14_CONSTEXPR
	basic_static_cstring(length_tag_t, size_type count,
	                     CharT ch)
	{
		discard(assign(count, ch));
	}
	//! trick used to disambiguate the const char* and the const char(&)[N] overloads
	//template <class CharT>
	struct cvt
	{
		const CharT *ptr;
		//cvt(nullptr_t) = delete;
		constexpr cvt(const CharT *p) noexcept
		        : ptr(p)
		{
		}
	};

	//! constructs a NTXS from a const CharT*
	//! expects str to be a NTXS with length lt the N+1
	JASEL_CXX14_CONSTEXPR basic_static_cstring(null_terminated_t, nullptr_t) = delete;

	//todo: make use of assign

	//! constructs a NTXS from a const char(&)[M] representing a NTXS literal
	//! expects str to be a NTXS with length lt the N+1
	// Note: All const CharT (&str)[M] are not NTXS
	template <size_t M, JASEL_REQUIRES(M <= (N + 1))>
	JASEL_CXX14_CONSTEXPR basic_static_cstring(null_terminated_t, const CharT (&str)[M])
	        : len_(M - 1)
	{
		JASEL_EXPECTS(CharT() == str[M - 1]);
		Traits::copy(data(), str, M);
	}

	//! constructs a NTXS from a const char* representing a NTXS of length len
	//! expects len <= N+1
	JASEL_CXX14_CONSTEXPR basic_static_cstring(null_terminated_t, const CharT *str, size_type len)
	        : len_(len)
	{
		JASEL_EXPECTS(str != 0);
		JASEL_EXPECTS(len <= N + 1);
		JASEL_EXPECTS(ntxs::valid(str, len));
		Traits::copy(data(), str, len);
	}

#if __cplusplus > 201402L
	//! constructs a NTXS from a sting_view representing a NTXS of length sv.length()
	//! expects sv.length() <= N+1
	// fixme: This overload doesn't support conversion from const char* equal to nullptr.
	constexpr basic_static_cstring(null_terminated_t tag, const string_view_type &sv)
	        : basic_static_cstring(tag, sv.data(), sv.length())
	{
		JASEL_EXPECTS(sv.data() != 0);
	}
#else
	JASEL_CXX14_CONSTEXPR basic_static_cstring(null_terminated_t, cvt str)
	{
		JASEL_EXPECTS(str.ptr != 0);
		bool b = ntxs::test_and_set_length<N>(str.ptr, len_);
		JASEL_EXPECTS(b);
		Traits::copy(data(), str.ptr, len_);
	}
	JASEL_CXX14_CONSTEXPR basic_static_cstring(std::string const &str)
	        : basic_static_cstring(null_terminated_t{}, str.c_str(), str.size())
	{
	}
#endif
#if __cplusplus > 201402L
	//! constructs a NTXS from a sting representing a NTXS of length str.length()
	//! expects str.length() <= N+1
	constexpr basic_static_cstring(const cstring_view_type &str)
	        : basic_static_cstring(null_terminated_t{}, str.c_str(), str.length())
	{
	}
#endif

	basic_static_cstring(std::initializer_list<CharT> ilist)
	{
		discard(assign(ilist));
	}
	// The following operations could be optimized by copying only the significant chars
	// this depends if we want that this class is trivially copyable or not
	JASEL_CXX14_CONSTEXPR basic_static_cstring(const basic_static_cstring &) noexcept = default;
	JASEL_CXX14_CONSTEXPR basic_static_cstring &operator=(const basic_static_cstring &) noexcept = default;

	//
	JASEL_CXX14_CONSTEXPR basic_static_cstring &operator=(const CharT *s)
	{
		return assign(s);
	}

	JASEL_CXX14_CONSTEXPR basic_static_cstring &operator=(CharT ch)
	{
		return assign(ch);
	}

	JASEL_CXX14_CONSTEXPR basic_static_cstring &operator=(std::initializer_list<CharT> ilist)
	{
		return assign(ilist);
	}

	template <class T>
	// requires ConvertibleTo<basic_string_view>
	JASEL_CXX14_CONSTEXPR basic_static_cstring &operator=(const T &t)
	{
		return assign(t);
	}

	// assign
	basic_static_cstring &assign(size_type count,
	                             CharT     ch)
	{
		clear();
		return append(count, ch);
	}

	basic_static_cstring &assign(const basic_static_cstring &str)
	{
		clear();
		return append(str);
	}

	basic_static_cstring &assign(const basic_static_cstring &str,
	                             size_type                   pos,
	                             size_type                   count = npos)
	{
		clear();
		return append(str, pos, count);
	}

	basic_static_cstring &assign(basic_static_cstring &&str) //noexcept(/* see below */);
	{
		clear();
		return append(move(str));
	}

	basic_static_cstring &assign(const CharT *s,
	                             size_type    count)
	{
		clear();
		return append(s, count);
	}

	basic_static_cstring &assign(const CharT *s)
	{
		clear();
		return append(s);
	}

	template <class InputIt>
	// requires InputIterator<InputIt>
	basic_static_cstring &assign(InputIt first,
	                             InputIt last)
	{
		clear();
		return append(first, last);
	}

	basic_static_cstring &assign(std::initializer_list<CharT> ilist)
	{
		return assign(ilist.begin(), ilist.end());
	}

	template <class T>
	// requires ConvertibleTo<basic_string_view>
	basic_static_cstring &assign(const T &t)
	{
		clear();
		return append(t);
	}

	template <class T>
	// requires ConvertibleTo<basic_string_view>
	basic_static_cstring &assign(const T & t,
	                             size_type pos,
	                             size_type count) // = npos)
	{
		clear();
		return append(t, pos, count);
	}

	//append
	basic_static_cstring &append(size_type count, CharT ch)
	{
		JASEL_EXPECTS(size() + count <= N);
		Traits::assign(end(), count, ch);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return *this;
	}

	basic_static_cstring &append(const basic_static_cstring &str)
	{
		JASEL_EXPECTS(size() + str.size() <= N);
		Traits::copy(end(), str.data(), str.size());
		len_ += str.size();
		Traits::assign(data_[len_], CharT{});
		return *this;
	}
	basic_static_cstring &append(const basic_static_cstring &str,
	                             size_type                   pos,
	                             size_type                   count = npos)
	{
		if (count == npos || count > (str.size() - pos))
		{
			count = str.size() - pos;
		}
		JASEL_EXPECTS(size() + count <= N);
		Traits::copy(end(), str.data() + pos, count);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return *this;
	}

	basic_static_cstring &append(const CharT *s)
	{
		// todo: optimize by doing a single pass
		auto count = Traits::length(s);
		return append(s, count);
	}

	basic_static_cstring &append(const CharT *s,
	                             size_type    count)
	{
		JASEL_EXPECTS(size() + count <= N);
		Traits::copy(end(), s, count);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return *this;
	}

	template <class InputIt>
	basic_static_cstring &append(InputIt first, InputIt last)
	{
		while (first != last)
		{
			JASEL_EXPECTS(len_ <= N);
			Traits::assign(data_[len_], *first);
			++len_;
			++first;
		}
		Traits::assign(data_[len_], CharT{});
		return *this;
	}

	basic_static_cstring &append(std::initializer_list<CharT> ilist)
	{
		return append(ilist.begin(), ilist.end());
	}

	// todo:: sfinae
	template <class T>
	basic_static_cstring &append(const T &t)
	{
		std::basic_string_view<CharT, Traits> sv = t;
		return append(sv.data(), sv.size());
	}

	// todo:: sfinae
	template <class T>
	basic_static_cstring &append(const T &t, size_type pos,
	                             size_type count = npos)
	{
		std::basic_string_view<CharT, Traits> sv = t;
		if (count == npos || count > sv.size())
		{
			count = sv.size() - pos + 1;
		}
		return append(sv.data() + pos, count);
	}

	//operator+=
	basic_static_cstring &operator+=(const basic_static_cstring &str)
	{
		return append(str);
	}
	basic_static_cstring &operator+=(CharT ch)
	{
		return append(ch);
	}
	basic_static_cstring &operator+=(const CharT *s)
	{
		return append(s);
	}

	basic_static_cstring &operator+=(std::initializer_list<CharT> ilist)
	{
		return append(ilist);
	}
	template <class T>
	basic_static_cstring &operator+=(const T &t)
	{
		return append(t);
	}

	// [string.static_cstring], iterator support
	JASEL_CXX14_CONSTEXPR iterator
	begin() noexcept
	{
		return data_.begin();
	}
	JASEL_CXX14_CONSTEXPR const_iterator begin() const noexcept { return data_.begin(); }
	JASEL_CXX14_CONSTEXPR iterator end() noexcept { return &data_[len_]; }
	JASEL_CXX14_CONSTEXPR const_iterator end() const noexcept { return &data_[len_]; }
	JASEL_CXX14_CONSTEXPR const_iterator cbegin() const noexcept { return data_.begin(); }
	JASEL_CXX14_CONSTEXPR const_iterator cend() const noexcept { return &data_[len_]; }
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
	JASEL_CXX14_CONSTEXPR const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	JASEL_CXX14_CONSTEXPR const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}

	friend constexpr const_iterator begin(basic_static_cstring str) noexcept
	{
		return str.begin();
	}
	friend constexpr const_iterator end(basic_static_cstring str) noexcept
	{
		return str.end();
	}
	// [string.static_cstring], capacity
	constexpr size_type          size() const noexcept { return len_; }
	constexpr size_type          length() const noexcept { return size(); }
	constexpr size_type          max_size() const noexcept { return N; }
	[[nodiscard]] constexpr bool empty() const noexcept { return size() == 0; }
	// [string.static_cstring], element access
	JASEL_CXX14_CONSTEXPR const_reference operator[](size_type pos) const { return data_[pos]; }
	JASEL_CXX14_CONSTEXPR reference operator[](size_type pos) { return data_[pos]; }
	JASEL_CXX14_CONSTEXPR const_reference at(size_type pos) const
	{
		if (pos > size())
			throw out_of_range("bad index");
		return data_[pos];
	}
	JASEL_CXX14_CONSTEXPR reference at(size_type pos)
	{
		if (pos > size())
			throw out_of_range("bad index");
		return data_[pos];
	}
	JASEL_CXX14_CONSTEXPR reference front() { return data_[0]; }
	JASEL_CXX14_CONSTEXPR const_reference front() const { return data_[0]; }
	JASEL_CXX14_CONSTEXPR reference back() { return data_[size() - 1]; }
	JASEL_CXX14_CONSTEXPR const_reference back() const { return data_[size() - 1]; }
	JASEL_CXX14_CONSTEXPR pointer data() noexcept { return &data_[0]; }
	JASEL_CXX14_CONSTEXPR const_pointer data() const noexcept { return &data_[0]; }
	JASEL_CXX14_CONSTEXPR const CharT *c_str() const noexcept { return &data_[0]; }

	// implicit conversions

	//! implicit conversion to a cstr view
	operator basic_cstr_view<CharT, Traits>() const noexcept
	{
		return basic_cstr_view<CharT, Traits>(null_terminated_t{}, data(), size());
	}
#if __cplusplus > 201402L
	//! implicit conversion to a cstring view
	operator basic_cstring_view<CharT, Traits>() const noexcept
	{
		return basic_cstring_view<CharT, Traits>(null_terminated_t{}, data(), size());
	}
	basic_string_view<CharT, Traits> to_string_view() const noexcept
	{
		return basic_string_view<CharT, Traits>(data(), size());
	}
	//! implicit conversion to a string view
	operator basic_string_view<CharT, Traits>() const noexcept { return to_string_view(); }
#endif
	// explicit conversions
	//!  named to_string() instead of str() because we are not accessing to a string but we are constructing it an it takes time
	basic_string<CharT, Traits> to_string() const noexcept
	{
		// we are missing a construction of std::string from a const char* with a know length
		basic_string<CharT, Traits> res(data());
		res.resize(size());
		return res;
	}
	//! explicit conversion to a string (expensive as allocation and copy takes place)
	explicit operator basic_string<CharT, Traits>() const noexcept { return to_string(); }

	JASEL_CXX14_CONSTEXPR void clear() noexcept
	{
		len_ = 0;
		Traits::assign(data_[len_], CharT{});
	}
	// insert
	basic_static_cstring &insert(size_type index, size_type count, CharT ch)
	{

		JASEL_EXPECTS(index <= size());
		JASEL_EXPECTS(size() + count <= N);
		Traits::move(data() + index + count, data() + index, len_ - index + 1);
		Traits::assign(data() + index, count, ch);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return *this;
	}
	basic_static_cstring &insert(size_type index, const CharT *s)
	{
		auto count = Traits::length(s);
		return insert(index, s, count);
	}

	basic_static_cstring &insert(size_type index, const CharT *s, size_type count)
	{
		JASEL_EXPECTS(index <= size());
		JASEL_EXPECTS(size() + count <= N);
		Traits::move(data() + index + count, data() + index, len_ - index + 1);
		Traits::copy(data() + index, s, count);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return *this;
	}

	basic_static_cstring &insert(size_type index, const basic_static_cstring &str)
	{
		return insert(index, str.data(), str.size());
	}

	basic_static_cstring &insert(size_type index, const basic_static_cstring &str,
	                             size_type index_str, size_type count = npos)
	{
		return insert(index, str.data() + index_str, count);
	}
	iterator insert(const_iterator pos, CharT ch)
	{
		return insert(pos, 1, &ch);
	}
	iterator insert(const_iterator pos, size_type count, CharT ch)
	{
		JASEL_EXPECTS(size() + count <= N);
		iterator pos_it = begin() + (pos - begin());
		Traits::move(pos_it + count, pos, end() - pos_it + 1);
		Traits::assign(pos_it, count, ch);
		len_ += count;
		Traits::assign(data_[len_], CharT{});
		return pos_it;
	}

	template <class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last)
	{
		auto last_end = end();
		append(first, last);
		iterator pos_it = begin() + (pos - begin());
		return rotate(pos_it, last_end, end());
	}

	iterator insert(const_iterator pos, std::initializer_list<CharT> ilist)
	{
		return insert(pos, ilist.begin(), ilist.end());
	}
	template <class T>
	// requires convertible to string view
	basic_static_cstring &insert(size_type pos, const T &t)
	{
		std::basic_string_view<CharT, Traits> sv = t;
		return insert(pos, sv.data(), sv.size());
	}

	template <class T>
	// requires convertible to string view
	basic_static_cstring &insert(size_type index, const T &t,
	                             size_type index_str, size_type count) // = npos)
	{
		std::basic_string_view<CharT, Traits> sv = t;
		if (count == npos || count > sv.size())
		{
			count = sv.size() - index_str + 1;
		}
		return insert(index, sv.data() + index_str, count);
	}

	// erase

	basic_static_cstring &erase(size_type index = 0, size_type count = npos)
	{
		JASEL_EXPECTS(index + count <= N);
		Traits::move(begin() + index, begin() + index + count, end() - (index + count) + 1);
		len_ -= count;
		return *this;
	}
	iterator erase(const_iterator position)
	{
		//return erase(position-begin(), 1);
		Traits::move(position, position + 1, end() - position + 1);
		len_--;
		return *this;
	}
	iterator erase(const_iterator first, const_iterator last)
	{
		//return erase(position-begin(), 1);
		Traits::move(first, last, last - first + 1);
		len_ -= last - first + 1;
		return *this;
	}

	//! expects lengtn() < N
	JASEL_CXX14_CONSTEXPR void push_back(CharT ch)
	{
		JASEL_EXPECTS(len_ < N);

		Traits::assign(data_[len_], ch);
		++len_;
		Traits::assign(data_[len_], CharT{});
	}
	//! expects ! empty()
	JASEL_CXX14_CONSTEXPR void pop_back()
	{
		JASEL_EXPECTS(!empty());
		--len_;
		Traits::assign(data_[len_], CharT{});
	}

	// replace
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              const basic_static_cstring &str);
	basic_static_cstring &replace(size_type pos, size_type count,
	                              const basic_static_cstring &str,
	                              size_type pos2, size_type count2 = npos);
	template <class InputIt>
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              InputIt first2, InputIt last2);
	basic_static_cstring &replace(size_type pos, size_type count,
	                              const CharT *cstr, size_type count2);
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              const CharT *cstr, size_type count2);
	basic_static_cstring &replace(size_type pos, size_type count,
	                              const CharT *cstr);
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              const CharT *cstr);
	basic_static_cstring &replace(size_type pos, size_type count,
	                              size_type count2, CharT ch);
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              size_type count2, CharT ch);
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              std::initializer_list<CharT> ilist);
	template <class T>
	basic_static_cstring &replace(size_type pos, size_type count,
	                              const T &t);
	template <class T>
	basic_static_cstring &replace(const_iterator first, const_iterator last,
	                              const T &t);
	template <class T>
	basic_static_cstring &replace(size_type pos, size_type count, const T &t,
	                              size_type pos2, size_type count2 = npos);

	//! expects count <= N
	JASEL_CXX14_CONSTEXPR void resize_default_init(size_type count)
	{
		JASEL_EXPECTS(count <= N);
		len_ = count;
		Traits::assign(data_[len_], CharT{});
	}

	//! expects count <= N
	JASEL_CXX14_CONSTEXPR void resize(size_type count)
	{
		resize(count, CharT{});
	}

	//! expects count <= N
	JASEL_CXX14_CONSTEXPR void resize(size_type count, CharT ch)
	{
		JASEL_EXPECTS(count <= N);
		if (count > len_)
		{
			Traits::assign(end(), count - len_, ch);
			len_ = count;
		}
		else
		{
			len_ = count;
		}
		Traits::assign(data_[len_], CharT{});
	}

	// [string.static_cstring], modifiers
	JASEL_CXX14_CONSTEXPR void remove_prefix(size_type n)
	{
		JASEL_EXPECTS(length() >= n);
		Traits::move(data(), data() + n, len_ - n + 1);
		len_ -= n;
	}
	// This implies to move set the \0
	JASEL_CXX14_CONSTEXPR void remove_suffix(size_type n)
	{
		JASEL_EXPECTS(length() >= n);
		len_ -= n;
		data_[len_] = CharT();
	}
	JASEL_CXX14_CONSTEXPR void swap(basic_static_cstring &other) noexcept
	{
		// possible optimization to swap only the significant chars
		data_.swap(other.data_);
		std::swap(len_, other.len_);
	}
#if __cplusplus > 201402L
	// [string.static_cstring], string operations
	constexpr size_type copy(CharT *s, size_type n, size_type pos = 0) const
	{
		return to_string_view().copy(s, n, pos);
	}
	constexpr basic_static_cstring substr(size_type pos = 0, size_type n = npos) const
	{
		return to_string_view().substr(pos, n);
	}
	constexpr int compare(string_view_type s) const noexcept
	{
		return to_string_view().compare(s);
	}
	constexpr int compare(size_type pos1, size_type n1, string_view_type s) const
	{
		return to_string_view().compare(pos1, n1, s);
	}
	constexpr int compare(size_type pos1, size_type n1, string_view_type s,
	                      size_type pos2, size_type n2) const
	{
		return to_string_view().compare(pos1, n1, s, pos2, n2);
	}

	constexpr int compare(const CharT *s) const
	{
		return to_string_view().compare(s);
	}
	constexpr int compare(size_type pos1, size_type n1, const CharT *s) const
	{
		return to_string_view().compare(pos1, n1, s);
	}

	constexpr int compare(size_type pos1, size_type n1, const CharT *s, size_type n2) const
	{
		return to_string_view().compare(pos1, n1, s, n2);
	}
	constexpr bool starts_with(string_view_type x) const noexcept
	{
		return to_string_view().starts_with(x);
	}
	constexpr bool starts_with(CharT x) const noexcept
	{
		return to_string_view().starts_with(x);
	}
	constexpr bool starts_with(const CharT *x) const
	{
		return to_string_view().starts_with(x);
	}
	constexpr bool ends_with(string_view_type x) const noexcept
	{
		return to_string_view().ends_with(x);
	}
	constexpr bool ends_with(CharT x) const noexcept
	{
		return to_string_view().ends_with(x);
	}
	constexpr bool ends_with(const CharT *x) const
	{
		return to_string_view().ends_with(x);
	}
	// [string.static_cstring], searching
	constexpr size_type find(string_view_type s, size_type pos = 0) const noexcept
	{
		return to_string_view().find(s, pos);
	}
	constexpr size_type find(CharT c, size_type pos = 0) const noexcept
	{
		return to_string_view().find(c, pos);
	}
	constexpr size_type find(const CharT *s, size_type pos, size_type n) const
	{
		return to_string_view().find(s, pos, n);
	}
	constexpr size_type find(const CharT *s, size_type pos = 0) const
	{
		return to_string_view().find(s, pos);
	}
	constexpr size_type rfind(string_view_type s, size_type pos = npos) const noexcept
	{
		return to_string_view().rfind(s, pos);
	}
	constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept
	{
		return to_string_view().rfind(c, pos);
	}
	constexpr size_type rfind(const CharT *s, size_type pos, size_type n) const
	{
		return to_string_view().rfind(s, pos, n);
	}
	constexpr size_type rfind(const CharT *s, size_type pos = npos) const
	{
		return to_string_view().rfind(s, pos);
	}
	constexpr size_type find_first_of(string_view_type s, size_type pos = 0) const noexcept
	{
		return to_string_view().find_first_of(s, pos);
	}
	constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept
	{
		return to_string_view().find_first_of(c, pos);
	}
	constexpr size_type find_first_of(const CharT *s, size_type pos, size_type n) const
	{
		return to_string_view().find_first_of(s, pos, n);
	}
	constexpr size_type find_first_of(const CharT *s, size_type pos = 0) const
	{
		return to_string_view().find_first_of(s, pos);
	}
	constexpr size_type find_last_of(string_view_type s, size_type pos = npos) const noexcept
	{
		return to_string_view().find_last_of(s, pos);
	}
	constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept
	{
		return to_string_view().find_last_of(c, pos);
	}
	constexpr size_type find_last_of(const CharT *s, size_type pos, size_type n) const
	{
		return to_string_view().find_last_of(s, pos, n);
	}
	constexpr size_type find_last_of(const CharT *s, size_type pos = npos) const
	{
		return to_string_view().find_last_of(s, pos);
	}
	constexpr size_type find_first_not_of(string_view_type s, size_type pos = 0) const noexcept
	{
		return to_string_view().find_first_not_of(s, pos);
	}
	constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept
	{
		return to_string_view().find_first_not_of(c, pos);
	}
	constexpr size_type find_first_not_of(const CharT *s, size_type pos,
	                                      size_type n) const
	{
		return to_string_view().find_first_not_of(s, pos, n);
	}
	constexpr size_type find_first_not_of(const CharT *s, size_type pos = 0) const
	{
		return to_string_view().find_first_not_of(s, pos);
	}
	constexpr size_type find_last_not_of(string_view_type s,
	                                     size_type        pos = npos) const noexcept
	{
		return to_string_view().find_last_not_of(s, pos);
	}
	constexpr size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept
	{
		return to_string_view().find_last_not_of(c, pos);
	}
	constexpr size_type find_last_not_of(const CharT *s, size_type pos,
	                                     size_type n) const
	{
		return to_string_view().find_last_not_of(s, pos, n);
	}
	constexpr size_type find_last_not_of(const CharT *s, size_type pos = npos) const
	{
		return to_string_view().find_last_not_of(s, pos);
	}
#endif
private:
	size_type                len_;
	std::array<CharT, N + 1> data_;
	//CharT     data_[N + 1];
};

// operator+

// operator==

// swap
template <class CharT, size_t N, class SizeType, class Traits>
void swap(basic_static_cstring<CharT, N, SizeType, Traits> &lhs,
          basic_static_cstring<CharT, N, SizeType, Traits> &rhs) noexcept(noexcept(lhs.swap(rhs)))
{
	lhs.swap(rhs);
}

// erase
template <class CharT, size_t N, class SizeType, class Traits, class U>
void erase(basic_static_cstring<CharT, N, SizeType, Traits> &c, const U &value)
{
	c.erase(std::remove(c.begin(), c.end(), value), c.end());
}

// erase_if
template <class CharT, size_t N, class SizeType, class Traits, class Pred>
void erase_if(basic_static_cstring<CharT, N, SizeType, Traits> &c, Pred pred)
{
	c.erase(std::remove_if(c.begin(), c.end(), pred), c.end());
}

// operator<<
template <class CharT, size_t N, class SizeType, class Traits>
basic_ostream<CharT, Traits> &
operator<<(basic_ostream<CharT, Traits> &                          os,
           const basic_static_cstring<CharT, N, SizeType, Traits> &str)
{
	os << str.c_str();
	return os;
}

// operator>>
template <class CharT, size_t N, class SizeType, class Traits>
basic_istream<CharT, Traits> &
operator>>(basic_istream<CharT, Traits> &                    is,
           basic_static_cstring<CharT, N, SizeType, Traits> &scstr)
{
	// todo: we should read from the stream up to N chars and no more, isn't it?
	basic_string<CharT, Traits> str;
	is >> str;
	scstr.assign(str);
	return is;
}

// getline

// sto

// to_static_cstring
// to_static_cwstring

// hash
// todo: add hash specialization

template <size_t N>
using static_cstring = basic_static_cstring<char, N>;
//template <size_t N>
//using static_u8cstring = basic_static_cstring<char8_t, N>;
template <size_t N>
using static_u16cstring = basic_static_cstring<char16_t, N>;
template <size_t N>
using static_u32cstring = basic_static_cstring<char32_t, N>;
template <size_t N>
using static_wcstring = basic_static_cstring<wchar_t, N>;

} // namespace fundamental_v3
} // namespace experimental
} // namespace std
#endif // header
