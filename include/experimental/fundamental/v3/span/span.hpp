// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2019 Vicente J. Botet Escriba

// see https://en.cppreference.com/w/cpp/container/span
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1024r1.pdf
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1227r2.html
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1024r2.pdf
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1024r3.pdf
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1394r0.pdf
// see http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2019/p1394r1.pdf

#ifndef JASEL_EXPERIMENTAL_SPAN_SPAN_HPP
#define JASEL_EXPERIMENTAL_SPAN_SPAN_HPP

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/fundamental/v3/utility/narrow.hpp>
#include <experimental/type_traits.hpp>
#include <algorithm>
#include <array>
#include <cstddef>
#include <iterator>
#include <limits>
#include <stdexcept>
#include <tuple>
#include <type_traits>
#include <utility>

#include <experimental/fundamental/v3/iterator/count.hpp>
#include <experimental/fundamental/v3/utility/size_of.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

using extent_t = size_t;
//inline
constexpr extent_t dynamic_extent = numeric_limits<size_t>::max();

// span has pointer semantics
template <class ElementType, extent_t Extent = dynamic_extent>
class span;

// span_ref has value semantics

template <class ElementType, extent_t Extent = dynamic_extent>
class span_ref;

namespace details
{

template <class T>
struct is_span_oracle : false_type
{
};

template <class ElementType, extent_t Extent>
struct is_span_oracle<span<ElementType, Extent>> : true_type
{
};

template <class T>
struct is_span : public is_span_oracle<typename remove_cv<T>::type>
{
};

template <class T>
struct is_std_array_oracle : false_type
{
};

template <class ElementType, size_t Extent>
struct is_std_array_oracle<array<ElementType, Extent>> : true_type
{
};

template <class T>
struct is_std_array : public is_std_array_oracle<typename remove_cv<T>::type>
{
};
#if 0
template <class From, class To>
struct is_allowed_pointer_conversion
        : public integral_constant<bool, is_pointer<From>::value &&
                                                 is_pointer<To>::value &&
                                                 is_convertible<From, To>::value>
{
};

template <class From, class To>
struct is_allowed_integral_conversion
        : public integral_constant<
                  bool, is_integral<From>::value && is_integral<To>::value &&
                                sizeof(From) == sizeof(To) && alignof(From) == alignof(To) &&
                                is_convertible<From, To>::value>
{
};
#endif

template <extent_t From, extent_t To>
struct is_allowed_extent_conversion
        : public integral_constant<bool, From == To || From == dynamic_extent ||
                                                 To == dynamic_extent>
{
};

template <class From, class To>
struct is_allowed_element_type_conversion
#if 0
        : public integral_constant<bool, is_same<From, typename remove_cv<To>::type >::value ||
                                                  is_allowed_pointer_conversion<From, To>::value ||
                                                  is_allowed_integral_conversion<From, To>::value>
    {
    };

    template <class From>
    struct is_allowed_element_type_conversion<From, byte>
        : public integral_constant<bool, !is_const<From>::value>
    {
    };

    template <class From>
    struct is_allowed_element_type_conversion<From, const byte> : public true_type
#endif
        : public integral_constant<bool, is_same<From, typename remove_cv<To>::type>::value ||
                                                 is_convertible<From (*)[], To (*)[]>::value>
{
};

template <class Span, bool IsConst>
class span_iterator {
	typedef typename Span::element_type element_type_;

public:
	typedef typename Span::difference_type                                           difference_type;
	typedef typename Span::size_type                                                 size_type;
	typedef random_access_iterator_tag                                               iterator_category;
	typedef typename remove_const<element_type_>::type                               value_type;
	typedef typename conditional<IsConst, const element_type_, element_type_>::type &reference;
	typedef typename add_pointer<reference>::type                                    pointer;

	JASEL_CXX14_CONSTEXPR span_iterator() noexcept
	        : span_(nullptr),
	          index_(0) {}

	JASEL_CXX14_CONSTEXPR span_iterator(const Span *span, size_type index)
	        : span_(span), index_(index)
	{
		JASEL_EXPECTS(span == nullptr || (index <= span_->size()));
	}

	friend class span_iterator<Span, true>;
	JASEL_CXX14_CONSTEXPR span_iterator(const span_iterator<Span, false> &other) noexcept
	        : span_(other.span_),
	          index_(other.index_) {}

	JASEL_CXX14_CONSTEXPR span_iterator<Span, IsConst> &operator=(const span_iterator<Span, IsConst> &) noexcept = default;

	JASEL_CXX14_CONSTEXPR reference operator*() const
	{
		JASEL_EXPECTS(span_);
		return (*span_)[index_];
	}

	JASEL_CXX14_CONSTEXPR pointer operator->() const
	{
		JASEL_EXPECTS(span_);
		return &((*span_)[index_]);
	}

	JASEL_CXX14_CONSTEXPR span_iterator &operator++() noexcept
	{
		JASEL_EXPECTS(span_ && index_ < span_->size());
		++index_;
		return *this;
	}

	JASEL_CXX14_CONSTEXPR span_iterator operator++(int) noexcept
	{
		span_iterator ret = *this;
		++(*this);
		return ret;
	}

	JASEL_CXX14_CONSTEXPR span_iterator &operator--() noexcept
	{
		JASEL_EXPECTS(span_ && index_ > 0 && index_ <= span_->size());
		--index_;
		return *this;
	}

	JASEL_CXX14_CONSTEXPR span_iterator operator--(int) noexcept
	{
		span_iterator ret = *this;
		--(*this);
		return ret;
	}

	JASEL_CXX14_CONSTEXPR span_iterator operator+(difference_type n) const noexcept
	{
		span_iterator ret = *this;
		return ret += n;
	}

	JASEL_CXX14_CONSTEXPR span_iterator &operator+=(difference_type n) noexcept
	{
		JASEL_EXPECTS(span_ && (index_ + n) >= 0 && (index_ + n) <= span_->ssize());
		index_ += n;
		return *this;
	}

	JASEL_CXX14_CONSTEXPR span_iterator operator-(difference_type n) const noexcept
	{
		span_iterator ret = *this;
		return ret -= n;
	}

	JASEL_CXX14_CONSTEXPR span_iterator &operator-=(difference_type n) noexcept { return *this += -n; }

	JASEL_CXX14_CONSTEXPR difference_type operator-(const span_iterator &rhs) const noexcept
	{
		JASEL_EXPECTS(span_ == rhs.span_);
		return index_ - rhs.index_;
	}

	JASEL_CXX14_CONSTEXPR reference operator[](difference_type n) const noexcept { return *(*this + n); }

	JASEL_CXX14_CONSTEXPR friend bool operator==(const span_iterator &lhs,
	                                             const span_iterator &rhs) noexcept
	{
		JASEL_EXPECTS(lhs.span_ == rhs.span_);
		return lhs.index_ == rhs.index_;
	}

	JASEL_CXX14_CONSTEXPR friend bool operator!=(const span_iterator &lhs,
	                                             const span_iterator &rhs) noexcept
	{
		return !(lhs == rhs);
	}

	JASEL_CXX14_CONSTEXPR friend bool operator<(const span_iterator &lhs, const span_iterator &rhs) noexcept
	{
		JASEL_EXPECTS(lhs.span_ == rhs.span_);
		return lhs.index_ < rhs.index_;
	}

	JASEL_CXX14_CONSTEXPR friend bool operator<=(const span_iterator &lhs,
	                                             const span_iterator &rhs) noexcept
	{
		return !(rhs < lhs);
	}

	JASEL_CXX14_CONSTEXPR friend bool operator>(const span_iterator &lhs, const span_iterator &rhs) noexcept
	{
		return rhs < lhs;
	}

	JASEL_CXX14_CONSTEXPR friend bool operator>=(const span_iterator &lhs,
	                                             const span_iterator &rhs) noexcept
	{
		return !(rhs > lhs);
	}

	void swap(span_iterator &rhs) noexcept
	{
		std::swap(index_, rhs.index_);
		std::swap(span_, rhs.span_);
	}

protected:
	const Span *span_;
	size_type   index_;
};

template <class Span, bool IsConst>
JASEL_CXX14_CONSTEXPR span_iterator<Span, IsConst>
operator+(typename span_iterator<Span, IsConst>::difference_type n,
          const span_iterator<Span, IsConst> &                   rhs) noexcept
{
	return rhs + n;
}

template <extent_t Ext>
class extent_type {
public:
	typedef size_of_t size_of_type;
	typedef size_t    size_type;

	static_assert(Ext >= 0, "A fixed-size span must be >= 0 in size.");

	JASEL_CXX14_CONSTEXPR extent_type() noexcept {}

	template <extent_t Other>
	JASEL_CXX14_CONSTEXPR extent_type(extent_type<Other> ext)
	{
		static_assert(Other == Ext || Other == dynamic_extent,
		              "Mismatch between fixed-size extent and size of initializing data.");
		JASEL_JASEL_EXPECTS(ext.size() == Ext);
	}

	JASEL_CXX14_CONSTEXPR extent_type(extent_t size) { JASEL_EXPECTS(size == Ext); }

	JASEL_CXX14_CONSTEXPR inline size_type           size() const noexcept { return Ext; }
	static JASEL_CXX14_CONSTEXPR inline size_of_type ssize() noexcept { return narrow_cast<size_of_type>(Ext); }
};

template <>
class extent_type<dynamic_extent> {
public:
	typedef size_of_t size_of_type;
	typedef size_t    size_type;

	template <extent_t Other>
	explicit JASEL_CONSTEXPR extent_type(extent_type<Other> ext)
	        : size_(narrow_cast<size_of_type>(ext.size()))
	{
	}

	explicit JASEL_CXX14_CONSTEXPR extent_type(extent_t size) : size_(narrow_cast<size_of_type>(size))
	{
	}

	JASEL_CXX14_CONSTEXPR inline size_type size() const noexcept
	{
		return narrow_cast<size_type>(size_);
	}
	JASEL_CONSTEXPR inline size_of_type ssize() const noexcept
	{
		return size_;
	}

private:
	size_of_type size_;
};
} // namespace details

// [span], class template span
template <class ElementType, extent_t Extent>
class span {
public:
	// constants and types
	typedef ElementType               element_type;
	typedef remove_cv_t<element_type> value_type;
	typedef ptrdiff_t                 difference_type;
	typedef size_of_t                 size_of_type;
	typedef size_t                    index_type;
	typedef size_t                    size_type;
	typedef element_type *            pointer;
	typedef element_type &            reference;

	typedef details::span_iterator<span<ElementType, Extent>, false> iterator;
	typedef details::span_iterator<span<ElementType, Extent>, true>  const_iterator;
	typedef std::reverse_iterator<iterator>                          reverse_iterator;
	typedef std::reverse_iterator<const_iterator>                    const_reverse_iterator;

	JASEL_CONSTEXPR static const extent_t extent = Extent;

	// [span.cons], span constructors, copy, assignment, and destructor
	span(span const &) = default;
	span(span &&)      = default;
	span &operator=(span const &) = default;
	span &operator=(span &&) = default;
	~span()                  = default;

	template <bool Dependent = false,
	          // "Dependent" is needed to make "std::enable_if_t<Dependent || Extent <= 0>" SFINAE,
	          // since "std::enable_if_t<Extent <= 0>" is ill-formed when Extent is greater than 0.
	          class = enable_if<(Dependent || Extent == 0 || Extent == dynamic_extent)>>
	JASEL_CONSTEXPR span() noexcept : storage_(nullptr, details::extent_type<0>())
	{
	}

	// todo
	// template <ContiguousIterator It>
	// //requires ConvertibleTo<remove_reference_t<iter_reference_t<It>> (*)[], ElementType (*)[]>
	// JASEL_CONSTEXPR span(It ptr, size_type count) : storage_(ptr, count);

	JASEL_CONSTEXPR span(pointer ptr, size_type count) : storage_(ptr, count)
	{
	}

	// todo
	// template <ContiguousIterator It, SizedSentinel<It> End>
	// //requires ConvertibleTo<remove_reference_t<iter_reference_t<It>> (*)[], ElementType (*)[]>
	// JASEL_CONSTEXPR span(It firstElem, End lastElem)

	JASEL_CONSTEXPR span(pointer firstElem, pointer lastElem)
	        : storage_(firstElem, std::distance(firstElem, lastElem))
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span(element_type (&arr)[N]) noexcept : storage_(&arr[0], details::extent_type<N>())
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span(array<value_type, N> &arr) noexcept
	        : storage_(&arr[0], details::extent_type<N>())
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span(const array<value_type, N> &arr) noexcept
	        : storage_(&arr[0], details::extent_type<N>())
	{
	}

	// todo
	// template <ranges::ContiguousRange R>
	// requires ranges::SizedRange<R> && (forwarding - range<R> || std::is_const_v<ElementType>)
	//      && ConvertibleTo<remove_reference_t<iter_reference_t<ranges::iterator_t<R>>> (*)[], ElementType (*)[]>
	// constexpr span(R &&r);

	// NB: the SFINAE here uses .data() as a incomplete/imperfect proxy for the requirement
	// on Container to be a contiguous sequence container.
	template <class Container>
	JASEL_CONSTEXPR span(Container &cont,
	                     typename enable_if<
	                             !details::is_span<Container>::value && !details::is_std_array<Container>::value &&
	                             is_convertible<typename Container::pointer, pointer>::value &&
	                             is_convertible<typename Container::pointer,
	                                            decltype(declval<Container>().data())>::value>::type * = 0

	                     )
	        : storage_(cont.data(), std::experimental::ssize(cont))
	{
	}

	template <class Container>
	JASEL_CONSTEXPR span(const Container &cont,
	                     typename enable_if<
	                             is_const<element_type>::value && !details::is_span<Container>::value &&
	                             is_convertible<typename Container::pointer, pointer>::value &&
	                             is_convertible<typename Container::pointer,
	                                            decltype(declval<Container>().data())>::value>::type * = 0)
	        : storage_(cont.data(), std::experimental::ssize(cont))
	{
	}

	template <
	        class OtherElementType, extent_t OtherExtent>
	JASEL_CONSTEXPR span(const span<OtherElementType, OtherExtent> &other,
	                     typename enable_if<
	                             details::is_allowed_extent_conversion<OtherExtent, Extent>::value &&
	                             details::is_allowed_element_type_conversion<OtherElementType, element_type>::value>::type * = 0)
	        : storage_(other.data(),
	                   details::extent_type<OtherExtent>(other.size()))
	{
	}

	// [span.sub], span subviews
	template <extent_t Count>
	JASEL_CXX14_CONSTEXPR span<element_type, Count> first() const
	{
		JASEL_EXPECTS(Count >= 0 && Count <= size());
		return span<element_type, Count>(data(), Count);
	}

	template <extent_t Count>
	JASEL_CXX14_CONSTEXPR span<element_type, Count> last() const
	{
		JASEL_EXPECTS(Count >= 0 && Count <= size());
		return span<element_type, Count>(data() + (size() - Count), Count);
	}

	template <extent_t Offset, extent_t Count>
	JASEL_CXX14_CONSTEXPR span<element_type, Count> subspan() const
	{
		JASEL_EXPECTS((Offset == 0 || (Offset > 0 && Offset <= size())) &&
		              (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())));
		return span<element_type, Count>(data() + Offset, Count == dynamic_extent ? size() - Offset : Count);
	}

#if defined __clang__
	template <extent_t Offset, bool b = false, extent_t ResExtent = Extent == dynamic_extent ? dynamic_extent : Extent - Offset>
	JASEL_CXX14_CONSTEXPR span<element_type, ResExtent> subspan() const
	{
		const extent_t Count = dynamic_extent;
		JASEL_EXPECTS((Offset == 0 || (Offset > 0 && Offset <= size())) &&
		              (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())));
		return span<element_type, ResExtent>(data() + Offset,
		                                     Count == dynamic_extent ? size() - Offset : Count);
	}
#endif
	JASEL_CXX14_CONSTEXPR span<element_type, dynamic_extent> first(size_type count) const
	{
		JASEL_EXPECTS(count <= size());
		return span<element_type, dynamic_extent>(data(), count);
	}

	JASEL_CXX14_CONSTEXPR span<element_type, dynamic_extent> last(size_type count) const
	{
		JASEL_EXPECTS(count <= size());
		return span<element_type, dynamic_extent>(data() + (size() - count), count);
	}

	JASEL_CXX14_CONSTEXPR span<element_type, dynamic_extent> subspan(size_type offset,
	                                                                 size_type count = dynamic_extent) const
	{
		JASEL_EXPECTS((offset <= size()) &&
		              (count == dynamic_extent || (offset + count <= size())));
		return span<element_type, dynamic_extent>(data() + offset, count == dynamic_extent ? size() - offset : count);
	}

	// [span.obs], span observers
	JASEL_CONSTEXPR size_type size() const noexcept { return storage_.size(); }
	JASEL_CONSTEXPR size_of_type ssize() const noexcept { return storage_.ssize(); }
	JASEL_CONSTEXPR size_type size_bytes() const noexcept { return size() * size_of<element_type>(); }
	JASEL_NODISCARD
	JASEL_CONSTEXPR bool empty() const noexcept { return size() == 0; }

	// [span.elem], span element access
	JASEL_CXX14_CONSTEXPR reference operator[](size_type idx) const
	{
		JASEL_EXPECTS(idx < storage_.size());
		return data()[idx];
	}
	JASEL_CONSTEXPR reference front() const { return operator[](0); }
	JASEL_CONSTEXPR reference back() const { return operator[](size() - 1); }
	JASEL_CONSTEXPR pointer data() const noexcept { return storage_.data(); }

	// [span.iter], span iterator support
	iterator begin() const noexcept { return iterator(this, 0); }
	iterator end() const noexcept { return iterator(this, size()); }

	const_iterator cbegin() const noexcept { return const_iterator(this, 0); }
	const_iterator cend() const noexcept { return const_iterator(this, size()); }

	reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
	reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

	friend constexpr iterator begin(span s) noexcept { return s.begin(); }
	friend constexpr iterator end(span s) noexcept { return s.end(); }

	JASEL_CXX14_CONSTEXPR span_ref<element_type, Extent> operator*() const;

private:
	// this implementation detail class lets us take advantage of the
	// empty base class optimization to pay for only storage of a single
	// pointer in the case of fixed-size spans
	template <class ExtentType>
	class storage_type : public ExtentType {
	public:
		template <class OtherExtentType>
		JASEL_CXX14_CONSTEXPR storage_type(pointer data, OtherExtentType ext) : ExtentType(ext), data_(data)
		{
			//JASEL_EXPECTS((!data && ExtentType::ssize() == 0) || (data && ExtentType::ssize() > 0));
		}

		JASEL_CONSTEXPR inline pointer data() const noexcept { return data_; }

	private:
		pointer data_;
	};

	storage_type<details::extent_type<Extent>> storage_;
};
// todo: add TCAD

template <class ElementType, extent_t Extent>
class span_ref {
public:
	// constants and types
	typedef ElementType               element_type;
	typedef remove_cv_t<element_type> value_type;
	typedef ptrdiff_t                 difference_type;
	typedef size_of_t                 size_of_type;
	typedef size_t                    index_type;
	typedef size_t                    size_type;
	typedef element_type *            pointer;
	typedef element_type &            reference;

	typedef details::span_iterator<span_ref<ElementType, Extent>, false> iterator;
	typedef details::span_iterator<span_ref<ElementType, Extent>, true>  const_iterator;
	typedef std::reverse_iterator<iterator>                              reverse_iterator;
	typedef std::reverse_iterator<const_iterator>                        const_reverse_iterator;

	JASEL_CONSTEXPR static const extent_t extent = Extent;

	// [span.cons], span constructors, copy, assignment, and destructor
	span_ref(span_ref const &) = default;
	span_ref(span_ref &&)      = default;

	span_ref &operator=(span_ref const &other)
	{
		if (&other == this)
			return *this;
		std::copy(other.data(), other.data() + other.size(), data());
		return *this;
	}

	span_ref &operator=(span_ref &&other)
	{
		if (&other == this)
			return *this;
		std::move(other.data(), other.data() + other.size(), data());
		return *this;
	}

	~span_ref() = default;

	template <bool Dependent = false,
	          // "Dependent" is needed to make "std::enable_if_t<Dependent || Extent <= 0>" SFINAE,
	          // since "std::enable_if_t<Extent <= 0>" is ill-formed when Extent is greater than 0.
	          class = enable_if<(Dependent || Extent == 0 || Extent == dynamic_extent)>>
	JASEL_CONSTEXPR span_ref() noexcept : storage_(nullptr, details::extent_type<0>())
	{
	}

	// todo
	// template <ContiguousIterator It>
	// //requires ConvertibleTo<remove_reference_t<iter_reference_t<It>> (*)[], ElementType (*)[]>
	// JASEL_CONSTEXPR span_ref(It ptr, size_type count) : storage_(ptr, count);

	JASEL_CONSTEXPR span_ref(pointer ptr, size_type count) : storage_(ptr, count)
	{
	}

	// todo
	// template <ContiguousIterator It, SizedSentinel<It> End>
	// //requires ConvertibleTo<remove_reference_t<iter_reference_t<It>> (*)[], ElementType (*)[]>
	// JASEL_CONSTEXPR span_ref(It firstElem, End lastElem)

	JASEL_CONSTEXPR span_ref(pointer firstElem, pointer lastElem)
	        : storage_(firstElem, std::distance(firstElem, lastElem))
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span_ref(element_type (&arr)[N]) noexcept : storage_(&arr[0], details::extent_type<N>())
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span_ref(array<value_type, N> &arr) noexcept
	        : storage_(&arr[0], details::extent_type<N>())
	{
	}

	template <size_t N>
	JASEL_CONSTEXPR span_ref(const array<value_type, N> &arr) noexcept
	        : storage_(&arr[0], details::extent_type<N>())
	{
	}

	// todo
	// template <ranges::ContiguousRange R>
	// requires ranges::SizedRange<R> && (forwarding - range<R> || std::is_const_v<ElementType>)
	//      && ConvertibleTo<remove_reference_t<iter_reference_t<ranges::iterator_t<R>>> (*)[], ElementType (*)[]>
	// constexpr span_ref(R &&r);

	// NB: the SFINAE here uses .data() as a incomplete/imperfect proxy for the requirement
	// on Container to be a contiguous sequence container.
	template <class Container>
	JASEL_CONSTEXPR span_ref(Container &cont,
	                         typename enable_if<
	                                 !details::is_span<Container>::value && !details::is_std_array<Container>::value &&
	                                 is_convertible<typename Container::pointer, pointer>::value &&
	                                 is_convertible<typename Container::pointer,
	                                                decltype(declval<Container>().data())>::value>::type * = 0

	                         )
	        : storage_(cont.data(), std::experimental::ssize(cont))
	{
	}

	template <class Container>
	JASEL_CONSTEXPR span_ref(const Container &cont,
	                         typename enable_if<
	                                 is_const<element_type>::value && !details::is_span<Container>::value &&
	                                 is_convertible<typename Container::pointer, pointer>::value &&
	                                 is_convertible<typename Container::pointer,
	                                                decltype(declval<Container>().data())>::value>::type * = 0)
	        : storage_(cont.data(), std::experimental::ssize(cont))
	{
	}

	template <
	        class OtherElementType, extent_t OtherExtent>
	JASEL_CONSTEXPR span_ref(const span_ref<OtherElementType, OtherExtent> &other,
	                         typename enable_if<
	                                 details::is_allowed_extent_conversion<OtherExtent, Extent>::value &&
	                                 details::is_allowed_element_type_conversion<OtherElementType, element_type>::value>::type * = 0)
	        : storage_(other.data(),
	                   details::extent_type<OtherExtent>(other.size()))
	{
	}

	template <
	        class OtherElementType, extent_t OtherExtent>
	JASEL_CONSTEXPR span_ref(const span<OtherElementType, OtherExtent> &other,
	                         typename enable_if<
	                                 details::is_allowed_extent_conversion<OtherExtent, Extent>::value &&
	                                 details::is_allowed_element_type_conversion<OtherElementType, element_type>::value>::type * = 0)
	        : storage_(other.data(),
	                   details::extent_type<OtherExtent>(other.size()))
	{
	}

	// [span.sub], span subviews
	template <extent_t Count>
	JASEL_CXX14_CONSTEXPR span_ref<element_type, Count> first() const
	{
		JASEL_EXPECTS(Count >= 0 && Count <= size());
		return span_ref<element_type, Count>(data(), Count);
	}

	template <extent_t Count>
	JASEL_CXX14_CONSTEXPR span_ref<element_type, Count> last() const
	{
		JASEL_EXPECTS(Count >= 0 && Count <= size());
		return span_ref<element_type, Count>(data() + (size() - Count), Count);
	}

	template <extent_t Offset, extent_t Count>
	JASEL_CXX14_CONSTEXPR span_ref<element_type, Count> subspan() const
	{
		JASEL_EXPECTS((Offset == 0 || (Offset > 0 && Offset <= size())) &&
		              (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())));
		return span_ref<element_type, Count>(data() + Offset, Count == dynamic_extent ? size() - Offset : Count);
	}

#if defined __clang__
	template <extent_t Offset, bool b = false, extent_t ResExtent = Extent == dynamic_extent ? dynamic_extent : Extent - Offset>
	JASEL_CXX14_CONSTEXPR span_ref<element_type, ResExtent> subspan() const
	{
		const extent_t Count = dynamic_extent;
		JASEL_EXPECTS((Offset == 0 || (Offset > 0 && Offset <= size())) &&
		              (Count == dynamic_extent || (Count >= 0 && Offset + Count <= size())));
		return span_ref<element_type, ResExtent>(data() + Offset,
		                                         Count == dynamic_extent ? size() - Offset : Count);
	}
#endif
	JASEL_CXX14_CONSTEXPR span_ref<element_type, dynamic_extent> first(size_type count) const
	{
		JASEL_EXPECTS(count <= size());
		return span_ref<element_type, dynamic_extent>(data(), count);
	}

	JASEL_CXX14_CONSTEXPR span_ref<element_type, dynamic_extent> last(size_type count) const
	{
		JASEL_EXPECTS(count <= size());
		return span_ref<element_type, dynamic_extent>(data() + (size() - count), count);
	}

	JASEL_CXX14_CONSTEXPR span_ref<element_type, dynamic_extent> subspan(size_type offset,
	                                                                     size_type count = dynamic_extent) const
	{
		JASEL_EXPECTS((offset <= size()) &&
		              (count == dynamic_extent || (offset + count <= size())));
		return span_ref<element_type, dynamic_extent>(data() + offset, count == dynamic_extent ? size() - offset : count);
	}

	// [span.obs], span observers
	JASEL_CONSTEXPR size_type size() const noexcept { return storage_.size(); }
	JASEL_CONSTEXPR size_of_type ssize() const noexcept { return storage_.ssize(); }
	JASEL_CONSTEXPR size_type size_bytes() const noexcept { return size() * size_of<element_type>(); }
	JASEL_NODISCARD
	JASEL_CONSTEXPR bool empty() const noexcept { return size() == 0; }

	// [span.elem], span element access
	JASEL_CXX14_CONSTEXPR reference operator[](size_type idx) const
	{
		JASEL_EXPECTS(idx < storage_.size());
		return data()[idx];
	}
	JASEL_CONSTEXPR reference front() const { return operator[](0); }
	JASEL_CONSTEXPR reference back() const { return operator[](size() - 1); }
	JASEL_CONSTEXPR pointer data() const noexcept { return storage_.data(); }

	// [span.iter], span iterator support
	iterator begin() const noexcept { return iterator(this, 0); }
	iterator end() const noexcept { return iterator(this, size()); }

	const_iterator cbegin() const noexcept { return const_iterator(this, 0); }
	const_iterator cend() const noexcept { return const_iterator(this, size()); }

	reverse_iterator rbegin() const noexcept { return reverse_iterator(end()); }
	reverse_iterator rend() const noexcept { return reverse_iterator(begin()); }

	const_reverse_iterator crbegin() const noexcept { return const_reverse_iterator(cend()); }
	const_reverse_iterator crend() const noexcept { return const_reverse_iterator(cbegin()); }

	friend constexpr iterator begin(span_ref s) noexcept { return s.begin(); }
	friend constexpr iterator end(span_ref s) noexcept { return s.end(); }

private:
	// this implementation detail class lets us take advantage of the
	// empty base class optimization to pay for only storage of a single
	// pointer in the case of fixed-size spans
	template <class ExtentType>
	class storage_type : public ExtentType {
	public:
		template <class OtherExtentType>
		JASEL_CXX14_CONSTEXPR storage_type(pointer data, OtherExtentType ext) : ExtentType(ext), data_(data)
		{
			//JASEL_EXPECTS((!data && ExtentType::ssize() == 0) || (data && ExtentType::ssize() > 0));
		}

		JASEL_CONSTEXPR inline pointer data() const noexcept { return data_; }

	private:
		pointer data_;
	};

	storage_type<details::extent_type<Extent>> storage_;
};

template <class ElementType, extent_t Extent>
JASEL_CXX14_CONSTEXPR span_ref<ElementType, Extent> span<ElementType, Extent>::operator*() const
{
	return span_ref<ElementType, Extent>(*this);
}

template <class ElementType, extent_t Extent>
JASEL_CXX14_CONSTEXPR span_ref<ElementType, Extent> make_ref(span<ElementType, Extent> that)
{
	return span_ref<ElementType, Extent>(that);
}

} // namespace fundamental_v3
} // namespace experimental

// 26.7.X Tuple interface
// template <class T>
// class tuple_size;
// template <size_t I, class T>
// class tuple_element;

template <class ElementType, experimental::extent_t Extent>
class tuple_size<experimental::span<ElementType, Extent>>
        : public integral_constant<size_t, Extent> {
};
template <class ElementType>
class tuple_size<experimental::span<ElementType, experimental::dynamic_extent>>;

template <size_t I, class ElementType, experimental::extent_t Extent>
class tuple_element<I, experimental::span<ElementType, Extent>> {
public:
	static_assert(I < static_cast<size_t>(Extent), "");
	using type = ElementType;
};

template <size_t I, class ElementType>
class tuple_element<I, experimental::span<ElementType, experimental::dynamic_extent>>;

template <size_t I, class ElementType, experimental::extent_t Extent,
          typename = typename enable_if<
                  experimental::dynamic_extent != Extent &&
                  (I < static_cast<size_t>(Extent))>::type>
constexpr ElementType &get(experimental::span<ElementType, Extent> const &sp) noexcept
{
	return sp[I];
}

} // namespace std
#endif // header
