// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_STRINGS_CSTR_VIEW_HPP
#define JASEL_EXPERIMENTAL_STRINGS_CSTR_VIEW_HPP

#include <experimental/contract.hpp>
#include <experimental/cstring_view.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/fundamental/v3/strings/null_terminated.hpp>
#include <cstring>
#include <string> // this should be removed when string woukd depend on cstr_view as we had for string_view
#if __cplusplus > 201402L
#include <string_view>
#endif
#define JASEL_CSTR_VIEW_EXPENSIVE

// See https://en.wikipedia.org/wiki/Null-terminated_string
// See https://en.cppreference.com/w/cpp/string/byte
// wondering if we should name the class ntbs_view instead

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

int cstrcmp(const char *lhs, const char *rhs)
{
	return strcmp(lhs, rhs);
}

int cstrcmp(wchar_t const *lhs, wchar_t const *rhs)
{
	return wcscmp(lhs, rhs);
}

// this class contains a pointer to a char const char* NTCS
template <class charT, class traits = char_traits<charT>>
class basic_cstr_view;

template <class charT, class traits>
class basic_cstr_view {
	const charT *ptr;

public:
	// types
	using traits_type     = traits;
	using value_type      = charT;
	using pointer         = value_type *;
	using const_pointer   = const value_type *;
	using reference       = value_type &;
	using const_reference = const value_type &;

	using const_iterator         = value_type const *; //implementation-defined ; // see 20.4.2.2
	using iterator               = const_iterator;
	using const_reverse_iterator = reverse_iterator<const_iterator>;
	using reverse_iterator       = const_reverse_iterator;
#if __cplusplus > 201402L
	using cstring_view_type = basic_cstring_view<charT, traits>;
	using string_view_type  = basic_string_view<charT, traits>;
#endif
	using string_type = basic_string<charT, traits>;

	struct ntbs_sentiel
	{
		friend constexpr bool operator==(const iterator &x, const ntbs_sentiel &)
		{
			return *x == charT();
		}
		friend constexpr bool operator==(const ntbs_sentiel &x, const iterator &y) { return y == x; }
		friend constexpr bool operator!=(const iterator &x, const ntbs_sentiel &y) { return !(x == y); }
		friend constexpr bool operator!=(const ntbs_sentiel &x, const iterator &y) { return !(y == x); }
	};
	using const_sentinel = ntbs_sentiel;
	using sentinel       = ntbs_sentiel;

	using size_type                 = size_t;
	using difference_type           = ptrdiff_t;
	static constexpr size_type npos = size_type(-1);

	// [string.view.cstr], construction and assignment
	constexpr basic_cstr_view() noexcept : ptr(&null_terminated_traits<charT>::zero) {}
	constexpr basic_cstr_view(const basic_cstr_view &) noexcept = default;
	JASEL_CXX14_CONSTEXPR basic_cstr_view &operator=(const basic_cstr_view &) noexcept = default;
	// expected str is not null and points to a NTBS
	constexpr basic_cstr_view(null_terminated_t, const charT *str) : ptr(str) {}
	JASEL_CXX14_CONSTEXPR basic_cstr_view(null_terminated_t, const charT *str, size_t len) : ptr(str)
	{
		JASEL_EXPECTS(ntxs::valid(str, len));
	}

#if __cplusplus > 201402L
	constexpr explicit basic_cstr_view(null_terminated_t, const string_view_type &strview) noexcept
	        : basic_cstr_view(strview.data(), strview.length())
	{
	}
	constexpr basic_cstr_view(const cstring_view_type &strview) noexcept
	        : ptr(strview.c_str())
	{
	}
#endif
	// [string.view.cstr], iterator support
	constexpr const_iterator begin() const noexcept { return ptr; }
	constexpr const_sentinel end() const noexcept { return const_sentinel{}; }
	constexpr const_iterator cbegin() const noexcept { return ptr; }
	constexpr const_sentinel cend() const noexcept { return const_sentinel{}; }

#if defined JASEL_CSTR_VIEW_EXPENSIVE
	constexpr const_reverse_iterator rbegin() const noexcept
	{
		return const_reverse_iterator(begin() + size());
	}
	constexpr const_reverse_iterator rend() const noexcept
	{
		return const_reverse_iterator(begin());
	}
	constexpr const_reverse_iterator crbegin() const noexcept
	{
		return const_reverse_iterator(cbegin() + size());
	}
	constexpr const_reverse_iterator crend() const noexcept
	{
		return const_reverse_iterator(cbegin());
	}
#endif
//        friend constexpr const_iterator begin(basic_cstr_view sv) noexcept
//        {   return sv.begin();}
//        friend constexpr const_iterator end(basic_cstr_view sv) noexcept
//        {   return sv.end();}
// [string.view.cstr], capacity
#if defined JASEL_CSTR_VIEW_EXPENSIVE
	// these are expensive. better to transform to a string_view that will store the size once
	constexpr size_type size() const noexcept
	{
		return this->length();
	}

	constexpr size_type length() const noexcept
	{
		return traits::length(ptr);
	}
#endif
	constexpr size_type max_size() const noexcept;

	[[nodiscard]] constexpr bool empty() const noexcept
	{
		return *ptr == charT();
	}
	// [string.view.cstr], element access
	constexpr const_reference operator[](size_type pos) const
	{
		return *(ptr + pos);
	}
#if defined JASEL_CSTR_VIEW_EXPENSIVE
#if __cplusplus > 201402L
	// this is expensive. better to move to a string_view
	constexpr const_reference at(size_type pos) const
	{
		return to_string_view().at(pos);
	}
#endif
#endif
	constexpr const_reference front() const
	{
		return *ptr;
	}
#if defined JASEL_CSTR_VIEW_EXPENSIVE
#if __cplusplus > 201402L
	constexpr const_reference back() const
	{
		return to_string_view().back();
	}
#endif
#endif

	constexpr const_pointer data() const noexcept
	{
		return ptr;
	}
	constexpr const charT *c_str() const noexcept { return ptr; }

#if __cplusplus > 201402L
	basic_string_view<charT, traits> to_string_view() const noexcept
	{
		return basic_string_view<charT, traits>(ptr);
	}
	explicit operator basic_string_view<charT, traits>() const noexcept { return to_string_view(); }
#endif
	basic_string<charT, traits> to_string() const noexcept
	{
		return basic_string<charT, traits>(ptr);
	}
	explicit operator basic_string<charT, traits>() const noexcept { return to_string(); }

	// [string.view.cstr], modifiers
	JASEL_CXX14_CONSTEXPR void remove_prefix(size_type n) { ptr += n; }
	// this function is not provided as can not ensure the invariant
	//constexpr void remove_suffix(size_type n);
	JASEL_CXX14_CONSTEXPR void swap(basic_cstr_view &s) noexcept
	{
		swap(ptr, s.ptr);
	}
	// [string.view.cstr], string operations
#if defined JASEL_CSTR_VIEW_EXPENSIVE
#if __cplusplus > 201402L
	constexpr size_type copy(charT *s, size_type n, size_type pos = 0) const
	{
		return to_string_view().copy(s, n, pos);
	}

	constexpr basic_cstr_view substr(size_type pos = 0) const
	{
		return basic_cstr_view(to_string_view().substr(pos).data());
	}

	constexpr string_view_type substr(size_type pos, size_type n) const
	{
		return to_string_view().substr(pos, n);
	}
#endif
#endif
	constexpr int compare(basic_cstr_view s) const noexcept
	{
		// unfortunately the following doesn't works without comparing before the lengths
		//traits::compare(data(), s.data(), s.size());

		return cstrcmp(data(), s.data());
	}

#if __cplusplus > 201402L
	constexpr int compare(string_view_type s) const noexcept
	{
		return -s.compare(ptr);
	}
	constexpr int compare(cstring_view_type s) const noexcept
	{
		return -s.compare(ptr);
	}
#endif
	//constexpr int compare(size_type pos1, size_type n1, string_view_type s) const;
	//constexpr int compare(size_type pos1, size_type n1, string_view_type s,
	//                size_type pos2, size_type n2) const;
	constexpr int compare(const charT *s) const
	{
		return cstrcmp(data(), s);
	}
	//constexpr int compare(size_type pos1, size_type n1, const charT* s) const;
	//constexpr int compare(size_type pos1, size_type n1, const charT* s, size_type n2) const;

#if __cplusplus > 201402L
	constexpr bool starts_with(string_view_type x) const noexcept;
#endif
	constexpr bool starts_with(charT x) const noexcept;
	constexpr bool starts_with(const charT *x) const;

#if __cplusplus > 201402L
	constexpr bool ends_with(string_view_type x) const noexcept;
#endif
	constexpr bool ends_with(charT x) const noexcept;
	constexpr bool ends_with(const charT *x) const;
	// [string.view.cstr], searching
#if __cplusplus > 201402L
	constexpr size_type find(string_view_type s, size_type pos = 0) const noexcept;
#endif
	constexpr size_type find(charT c, size_type pos = 0) const noexcept;
	constexpr size_type find(const charT *s, size_type pos, size_type n) const;
	constexpr size_type find(const charT *s, size_type pos = 0) const;
#if __cplusplus > 201402L
	constexpr size_type rfind(string_view_type s, size_type pos = npos) const noexcept;
#endif
	constexpr size_type rfind(charT c, size_type pos = npos) const noexcept;
	constexpr size_type rfind(const charT *s, size_type pos, size_type n) const;
	constexpr size_type rfind(const charT *s, size_type pos = npos) const;
#if __cplusplus > 201402L
	constexpr size_type find_first_of(string_view_type s, size_type pos = 0) const noexcept;
#endif
	constexpr size_type find_first_of(charT c, size_type pos = 0) const noexcept;
	constexpr size_type find_first_of(const charT *s, size_type pos, size_type n) const;
	constexpr size_type find_first_of(const charT *s, size_type pos = 0) const;
#if __cplusplus > 201402L
	constexpr size_type find_last_of(string_view_type s, size_type pos = npos) const noexcept;
#endif
	constexpr size_type find_last_of(charT c, size_type pos = npos) const noexcept;
	constexpr size_type find_last_of(const charT *s, size_type pos, size_type n) const;
	constexpr size_type find_last_of(const charT *s, size_type pos = npos) const;
#if __cplusplus > 201402L
	constexpr size_type find_first_not_of(string_view_type s, size_type pos = 0) const noexcept;
#endif
	constexpr size_type find_first_not_of(charT c, size_type pos = 0) const noexcept;
	constexpr size_type find_first_not_of(const charT *s, size_type pos,
	                                      size_type n) const;
	constexpr size_type find_first_not_of(const charT *s, size_type pos = 0) const;
#if __cplusplus > 201402L
	constexpr size_type find_last_not_of(string_view_type s,
	                                     size_type        pos = npos) const noexcept;
#endif
	constexpr size_type find_last_not_of(charT c, size_type pos = npos) const noexcept;
	constexpr size_type find_last_not_of(const charT *s, size_type pos,
	                                     size_type n) const;
	constexpr size_type find_last_not_of(const charT *s, size_type pos = npos) const;

private:
	//string_view_type sv_; // exposition only
};

using cstr_view = basic_cstr_view<char>;
//using u8cstr_view = basic_cstr_view<char8_t>;
//using u16cstr_view = basic_cstr_view<char16_t>;
//using u32cstr_view = basic_cstr_view<char32_t>;
using wcstr_view = basic_cstr_view<wchar_t>;

} // namespace fundamental_v3
} // namespace experimental
} // namespace std

#endif // header
