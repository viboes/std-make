// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STRINGS_STRING_VIEW_HPP
#define JASEL_EXPERIMENTAL_STRINGS_STRING_VIEW_HPP

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <string> // this should be removed when string would depend on string_view as we had for std::string_view
#if __cplusplus > 201703L
#include <string_view>
#endif

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
#if __cplusplus > 201703L
using std::basic_string_view;
using std::string_view;
//using std::u8string_view;
using std::u16string_view;
using std::u32string_view;
using std::wstring_view;

#else
// this is a temporary implementation

// this class contains a pointer to a char const char* NTCS
template <class CharT, class Traits = char_traits<CharT>>
class basic_string_view;

template <class CharT, class Traits>
class basic_string_view {

public:
	// types
	using traits_type     = Traits;
	using value_type      = CharT;
	using pointer         = value_type *;
	using const_pointer   = const value_type *;
	using reference       = value_type &;
	using const_reference = const value_type &;

	using const_iterator            = value_type const *; //implementation-defined ; // see 20.4.2.2
	using iterator                  = const_iterator;
	using const_reverse_iterator    = reverse_iterator<const_iterator>;
	using reverse_iterator          = const_reverse_iterator;
	using string_type               = basic_string<CharT, Traits>;
	using size_type                 = size_t;
	using difference_type           = ptrdiff_t;
	static constexpr size_type npos = size_type(-1);

	// [string.view.cstr], construction and assignment
	constexpr basic_string_view() noexcept : ptr(nullptr), len(0) {}
	constexpr basic_string_view(const basic_string_view &) noexcept = default;
	JASEL_CXX14_CONSTEXPR basic_string_view &operator=(const basic_string_view &) noexcept = default;
	constexpr basic_string_view(const CharT *str) : ptr(str), len(Traits::length(str)) {}
	JASEL_CXX14_CONSTEXPR basic_string_view(const CharT *str, size_t len) : ptr(str), len(len)
	{
	}

	//! implicit conversions from a string
	// this should be moved to the class string
	constexpr basic_string_view(const string_type &str) noexcept
	        : ptr(str.data()), len(str.size())
	{
	}

	// [string.view.cstr], iterator support
	constexpr const_iterator begin() const noexcept { return ptr; }
	constexpr const_iterator end() const noexcept { return ptr + len; }
	constexpr const_iterator cbegin() const noexcept { return ptr; }
	constexpr const_iterator cend() const noexcept { return ptr + len; }

	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(end());
	}
	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}
	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cend());
	}
	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}
	//        friend constexpr const_iterator begin(basic_string_view sv) noexcept
	//        {   return sv.begin();}
	//        friend constexpr const_iterator end(basic_string_view sv) noexcept
	//        {   return sv.end();}
	// [string.view.cstr], capacity
	// these are expensive. better to transform to a string_view that will store the size once
	constexpr size_type size() const noexcept
	{
		return this->length();
	}

	constexpr size_type length() const noexcept
	{
		return len;
	}
	constexpr size_type max_size() const noexcept;

	[[nodiscard]] constexpr bool empty() const noexcept
	{
		return len == 0;
	}
	// [string.view.cstr], element access
	constexpr const_reference operator[](size_type pos) const
	{
		return *(ptr + pos);
	}
	constexpr const_reference at(size_type pos) const
	{
		// todo add exceptional case
		return *(ptr + pos);
	}
	constexpr const_reference front() const
	{
		return *ptr;
	}
	constexpr const_reference back() const
	{
		return *(ptr + len - 1);
	}

	constexpr const_pointer data() const noexcept
	{
		return ptr;
	}
	//constexpr const CharT *c_str() const noexcept { return ptr; }

	basic_string<CharT, Traits> to_string() const noexcept
	{
		return basic_string<CharT, Traits>(ptr);
	}
	explicit operator basic_string<CharT, Traits>() const noexcept { return to_string(); }

	// [string.view.cstr], modifiers
	JASEL_CXX14_CONSTEXPR void remove_prefix(size_type n)
	{
		ptr += n;
		len -= n;
	}
	JASEL_CXX14_CONSTEXPR void remove_suffix(size_type n)
	{
		len -= n;
	}
	JASEL_CXX14_CONSTEXPR void swap(basic_string_view &s) noexcept
	{
		swap(ptr, s.ptr);
		swap(len, s.len);
	}
	// [string.view.cstr], string operations
	constexpr size_type copy(CharT *s, size_type n, size_type pos = 0) const;

	constexpr basic_string_view substr(size_type pos = 0) const
	{
		return basic_string_view(data() + pos, len - pos);
	}

	constexpr basic_string_view substr(size_type pos, size_type n) const
	{
		return basic_string_view(data() + pos, n);
	}
	JASEL_CXX14_CONSTEXPR int compare(basic_string_view v) const noexcept
	{
		auto rlen = min(size(), v.size());
		auto res  = Traits::compare(data(), v.data(), rlen);
		if (res < 0)
			return res;
		if (res > 0)
			return res;
		if (size() < v.size())
			return -1;
		if (size() > v.size())
			return 1;
		return 0;
	}

	constexpr int compare(size_type pos1, size_type count1, basic_string_view v) const
	{
		return substr(pos1, count1).compare(v);
	}
	constexpr int compare(size_type pos1, size_type count1, basic_string_view v,
	                      size_type pos2, size_type count2) const
	{
		return substr(pos1, count1).compare(v.substr(pos2, count2));
	}
	constexpr int compare(const CharT *s) const
	{
		return compare(basic_string_view(s));
	}

	constexpr int compare(size_type pos1, size_type count1, const CharT *s) const
	{
		return substr(pos1, count1).compare(basic_string_view(s));
	}

	constexpr int compare(size_type pos1, size_type count1, const CharT *s, size_type count2) const
	{
		return substr(pos1, count1).compare(basic_string_view(s, count2));
	}

	constexpr bool starts_with(basic_string_view x) const noexcept
	{
		return size() >= x.size() && compare(0, x.size(), x) == 0;
	}
	constexpr bool starts_with(CharT x) const noexcept
	{
		return starts_with(basic_string_view(std::addressof(x), 1));
	}

	constexpr bool starts_with(const CharT *x) const
	{
		return starts_with(basic_string_view(x));
	}
	constexpr bool ends_with(basic_string_view x) const noexcept
	{
		return size() >= x.size() && compare(size() - x.size(), npos, x) == 0;
	}

	constexpr bool ends_with(CharT x) const noexcept
	{
		return ends_with(basic_string_view(std::addressof(x), 1));
	}
	constexpr bool ends_with(const CharT *x) const
	{
		return ends_with(basic_string_view(x));
	}
	// [string.view.cstr], searching
	JASEL_CXX14_CONSTEXPR size_type find(basic_string_view s, size_type pos = 0) const noexcept
	{
		if (s.size() == 0)
			return npos;
		CharT const ch = s.front();
		for (auto pos_it = begin() + pos; pos_it + s.size() < end(); ++pos_it)
		{
			// look for the first
			pos_it = Traits::find(pos_it, end() - pos_it - s.size(), ch);
			if (!pos_it)
				// not found
				return npos;
			if (Traits::compare(pos_it + 1, s.data() + 1, s.size() - 1) == 0)
				// found
				return pos_it - data();
			// not found yet, advance of one and try again
		}
		return npos;
	}
	constexpr size_type find(CharT ch, size_type pos = 0) const noexcept
	{
		return find(basic_string_view(std::addressof(ch), 1), pos);
	}
	constexpr size_type find(const CharT *s, size_type pos, size_type count) const
	{
		return find(basic_string_view(s, count), pos);
	}
	constexpr size_type find(const CharT *s, size_type pos = 0) const
	{
		return find(basic_string_view(s), pos);
	}

	constexpr size_type rfind(basic_string_view s, size_type pos = npos) const noexcept;
	constexpr size_type rfind(CharT c, size_type pos = npos) const noexcept
	{
		return rfind(basic_string_view(std::addressof(c), 1), pos);
	}
	constexpr size_type rfind(const CharT *s, size_type pos, size_type count) const
	{
		return rfind(basic_string_view(s, count), pos);
	}
	constexpr size_type rfind(const CharT *s, size_type pos = npos) const
	{
		return rfind(basic_string_view(s), pos);
	}

	constexpr size_type find_first_of(basic_string_view s, size_type pos = 0) const noexcept;
	constexpr size_type find_first_of(CharT c, size_type pos = 0) const noexcept
	{
		return find_first_of(basic_string_view(std::addressof(c), 1), pos);
	}
	constexpr size_type find_first_of(const CharT *s, size_type pos, size_type count) const
	{
		return find_first_of(basic_string_view(s, count), pos);
	}
	constexpr size_type find_first_of(const CharT *s, size_type pos = 0) const
	{
		return find_first_of(basic_string_view(s), pos);
	}

	constexpr size_type find_last_of(basic_string_view s, size_type pos = npos) const noexcept;
	constexpr size_type find_last_of(CharT c, size_type pos = npos) const noexcept
	{
		return find_last_of(basic_string_view(std::addressof(c), 1), pos);
	}
	constexpr size_type find_last_of(const CharT *s, size_type pos, size_type count) const
	{
		return find_last_of(basic_string_view(s, count), pos);
	}
	constexpr size_type find_last_of(const CharT *s, size_type pos = npos) const
	{
		return find_last_of(basic_string_view(s), pos);
	}

	constexpr size_type find_first_not_of(basic_string_view s, size_type pos = 0) const noexcept;
	constexpr size_type find_first_not_of(CharT c, size_type pos = 0) const noexcept
	{
		return find_first_not_of(basic_string_view(std::addressof(c), 1), pos);
	}
	constexpr size_type find_first_not_of(const CharT *s, size_type pos,
	                                      size_type count) const
	{
		return find_first_not_of(basic_string_view(s, count), pos);
	}
	constexpr size_type find_first_not_of(const CharT *s, size_type pos = 0) const
	{
		return find_first_not_of(basic_string_view(s), pos);
	}

	constexpr size_type find_last_not_of(basic_string_view s,
	                                     size_type         pos = npos) const noexcept;
	constexpr size_type find_last_not_of(CharT c, size_type pos = npos) const noexcept
	{
		return find_last_not_of(basic_string_view(std::addressof(c), 1), pos);
	}

	constexpr size_type find_last_not_of(const CharT *s, size_type pos,
	                                     size_type count) const
	{
		return find_last_not_of(basic_string_view(s, count), pos);
	}

	constexpr size_type find_last_not_of(const CharT *s, size_type pos = npos) const
	{
		return find_last_not_of(basic_string_view(s), pos);
	}

private:
	const value_type *ptr;
	size_type         len;
};

using string_view = basic_string_view<char>;
//using u8string_view = basic_string_view<char8_t>;
using u16string_view = basic_string_view<char16_t>;
using u32string_view = basic_string_view<char32_t>;
using wstring_view   = basic_string_view<wchar_t>;

// hash

// relational operators

template <class CharT, class Traits>
constexpr bool operator==(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) == 0;
}
template <class CharT, class Traits>
constexpr bool operator!=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) != 0;
}
template <class CharT, class Traits>
constexpr bool operator<(basic_string_view<CharT, Traits> lhs,
                         basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) < 0;
}
template <class CharT, class Traits>
constexpr bool operator<=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) <= 0;
}
template <class CharT, class Traits>
constexpr bool operator>(basic_string_view<CharT, Traits> lhs,
                         basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) > 0;
}
template <class CharT, class Traits>
constexpr bool operator>=(basic_string_view<CharT, Traits> lhs,
                          basic_string_view<CharT, Traits> rhs) noexcept
{
	return lhs.compare(rhs) >= 0;
}

// stream

#endif
} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
