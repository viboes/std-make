//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
// Based on the <bitset> implementation
//
//////////////////////////////////////////////////////////////////////////////

/*!
 \file
 \brief
 The header \c <experimental/bit_mask.hpp> defines a class template \c bit_mask<T, N> for managing bit set as std::bitset,
 but ensures that the implementation uses a specific representation. Currently std::bitset uses 64 bits  (the size of long long)
 // and several  related functions for representing and manipulating sets of indexes. We can say that
 \c bit_mask is the counterpart of \c std::bitset when the user forces the representation.
 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BIT_MASK_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BIT_MASK_HPP


#include <stdexcept>
#include <iosfwd>
#include <string>
#include <cstddef>
#include <climits>
#include <string>
#include <limits>

/// todo: mixed operations between masks of different sizes
/// The bits not represented are as if they were set to 0

/// todo: Add constexpr
/// todo: Add noexcept

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  struct pos_tag_t {};
  struct fill_tag_t {};
  struct mask_tag_t {};

  namespace bit_ops
  {
    template <class T>
    constexpr T rotl(T x, unsigned int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      const unsigned int m = s % N;
      return m==0
           ? x
           : (x << m) | (x >> (N - m))
           ;
    }
    template <class T>
    int popcount(T x) noexcept
    {
      return __builtin_popcount(x);
    }

    template <size_t N, class T=unsigned>
    constexpr T up_to() noexcept
    {
      return (N >= sizeof(T) * CHAR_BIT) ?
          T(-1) : (T(1) << N) - 1;
    }

    template <class T=unsigned>
    constexpr T up_to(size_t n) noexcept
    {
      return (n >= sizeof(T) * CHAR_BIT) ?
          T(-1) : (T(1) << n) - 1;
    }

    template <size_t S, class T=unsigned>
    constexpr T single() noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr unsigned int m = S % N;
      return (T(1) << m);
    }
    template <class T>
    constexpr T single(size_t s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      const unsigned int m = s % N;
      return (T(1) << m);
    }
  }
    /**
      @TParams
      @Param{N,the number of significant bits on the storage}
      @Param{T,the storage type}
      @Requires @c N must be @c T must be a model of <em>unsigned Integral</em> type.

     The class template @c bit_mask<N,T> describes an object that can store a sequence
     consisting of a fixed number of bits, given by @c N.

     Each bit represents either the value zero (reset) or one (set). To toggle
     a bit is to change the value zero to one, or the value one to zero. Each
     bit has a non-negative position @c pos. When converting between an object of
     class @c bit_mask<N,T> and a value of some integral type, bit position pos
     corresponds to the bit value 1<<pos. The integral value corresponding to
     two or more bits is the sum of their bit values.

     The functions described in this section can report three kinds of errors,
     each associated with a distinct exception:
     - an invalid-argument error is associated with exceptions of type @c std::invalid_argument;
     - an out-of-range error is associated with exceptions of type @c std::out_of_range;
     - an overflow error is associated with exceptions of type @c std::overflow_error.

     */
    template<size_t N, typename T
    // = boost::uint_t<N>::least
    >
    class bit_mask;

    template<size_t N, typename T>
    class bit_mask
    {
      static constexpr unsigned int digits = std::numeric_limits<T>::digits;

      T bits;
      constexpr void check(size_t testing) const
      {
        if (testing >=  N) throw std::out_of_range("bit_mask");
      }
    public:

      static_assert(is_integral<T>::value, "T must be an integral type");
      static_assert(is_unsigned<T>::value, "T must be a unsigned type");
      //! @brief A proxy class that acts as a reference to a single bit.

      //! proxy class representing a reference to a bit
      //!
      //! It contains an assignment operator, a conversion to @c bool,
      //! an @c operator~, and a member function @c flip.
      //! It exists only as a helper class for @c bit_mask's @c operator[].
      class reference {
        friend class bit_mask<N,T>;
        bit_mask<N,T>& ref_;
        T pos_;
        constexpr reference() noexcept;
        constexpr reference(bit_mask<N,T>& ref, T pos) noexcept
          : ref_(ref), pos_(pos)
        { }
      public:
        ~reference() noexcept
        { }

        //! assignement from bool
        constexpr reference& operator=(bool x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! assignement from another reference
        constexpr reference& operator=(const reference& x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! flip the bit
        constexpr bool operator~() const noexcept
        {
          return ref_.flip(pos_);
        }

        //! implicit conversion to bool
        constexpr operator bool() const noexcept
        {
          return ref_.test(pos_);
        }

        //! flip the bit
        constexpr reference flip() const noexcept
        {
          return ref_.flip(pos_);
        }
      };
      //! @Effects Constructs an object of class \c bit_mask<>, initializing all
      //! ordinals to zero.
      //! @NoExcept
      constexpr bit_mask() noexcept
          : bits(0)
      {
      }
      //! @Effects Constructs an object of class \c bit_mask<>, initializing the
      //! \c pos bit position to \c 1 and the others to \c 0.
      //! @Requires pos <= N
      explicit constexpr bit_mask(pos_tag_t, size_t pos)
        : bits(bit_ops::single<T>(pos))
      {
        check(pos);
      }

      //! @Effects Constructs an object of class \c bit_mask<>, initializing the
      //! all bit position to \c val.
      //! @NoExcept
      explicit constexpr bit_mask(fill_tag_t, bool val) noexcept
        : bits(val?bit_ops::up_to<N,T>():0)
      {
      }

      //! @Effects Constructs an object of class \c bit_mask<>, initializing the
      //! first \c M bit positions to the corresponding bit values in \c val.
      //! \c M is the smaller of \c N and the number of bits in the value
      //! representation of \c unsigned \c long \c long. If \c M<N, the remaining bit
      //! positions are initialized to zero.
      //! @NoExcept

      explicit constexpr bit_mask(mask_tag_t, T val) noexcept
        : bits(val & bit_ops::up_to<N,T>())
      {
      }
      //#if defined(__GNUC__) &&  (__GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 ))
      #if 1
      #else

      //! @Requires <tt>pos <= str.size()</tt>.
      //! @Throws std::out_of_range if <tt>pos > str.size()</tt>.
      //! @Effects Determines the effective length \c rlen of the initializing
      //! string as the smaller of \c n and <tt>str.size()-pos</tt>.
      //! The function then throws \c std::invalid_argument if any of the \c rlen
      //! characters in \c str beginning at position pos is other than zero or one.
      //! The function uses \c traits::eq() to compare the character values.
      //! Otherwise, the function constructs an object of class \c bit_mask<N,T>,
      //! initializing the first \c M bit positions to values determined from the
      //! corresponding characters in the string \c str. \c M is the smaller of \c N and
      //! \c rlen.\n
      //! An element of the constructed string has value zero if the
      //! corresponding character in \c str, beginning at position \c pos, is \c 0 zero.
      //! Otherwise, the element has the value 1. Character position \c pos+M-1
      //! corresponds to bit position zero. Subsequent decreasing character
      //! positions correspond to increasing bit positions.
      //! If \c M<ordinal::meta::size<T>::value, remaining bit positions are initialized to zero.
      template<class charT, class traits, class TAllocator>
        explicit bit_mask(const std::basic_string<charT,traits,TAllocator>& str,
                          typename std::basic_string<charT,traits,TAllocator>::size_type pos = 0,
                          typename std::basic_string<charT,traits,TAllocator>::size_type n =
                                   std::basic_string<charT,traits,TAllocator>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str, pos, n, zero, one)
      {}

      //! @Effects Constructs an object of class bit_mask<N> as if by bit_mask(string(str)).
      template <class charT>
        explicit bit_mask(const charT* str,
                          typename std::basic_string<charT>::size_type n = std::basic_string<charT>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str,n, zero, one)
      {}
      #endif

      //! @Effects Clears each bit in \c *this for which the corresponding bit in
      //! \c rhs is clear, and leaves all other bits unchanged.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& operator&=(const bit_mask& rhs) noexcept
      {
        bits &= rhs.bits;
        return *this;
      }

      //! @Effects Sets each bit in \c *this for which the corresponding bit in
      //! \c rhs is set, and leaves all other bits unchanged.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& operator|=(const bit_mask& rhs) noexcept
      {
        bits |= rhs.bits;
        return *this;
      }

      //! @Effects Toggles each bit in \c *this for which the corresponding bit in
      //! \c rhs is set, and leaves all other bits unchanged.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& operator^=(const bit_mask& rhs) noexcept
      {
        bits ^= rhs.bits;
        return *this;
      }

      //! @Effects Replaces each bit at position \c I in \c *this with a value determined as follows:
      //! - If \c I<pos, the new value is zero;
      //! - If \c I>=pos, the new value is the previous value of the bit at position \c I-pos.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& operator<<=(size_t pos) noexcept
      {
        bits <<= pos;
        return *this;
      }

      //! @Effects Replaces each bit at position \c I in \c *this with a value determined as follows:
      //! - If \c pos>=N-I, the new value is zero;
      //! - If \c pos<N-I, the new value is the previous value of the bit at position \c I+pos.
      //! @Returns \c *this.
      //! @NoExcept

      constexpr bit_mask& operator>>=(size_t pos) noexcept
      {
        bits >>= pos;
        return *this;
      }

      //! @Returns A count of the number of bits set in \c *this.
      constexpr std::size_t count() const noexcept
      {
        return __builtin_popcount(bits);
      }

      //! @Returns \c static_size.
      constexpr std::size_t size() const noexcept
      {
        return N;
      }

      //! @Requires \c pos shall be valid.
      //! @Throws std::invalid_argument if @c pos i sout of range.
      //! @Returns \c true if the bit at the associated position of \c e in \c *this has the value one,
      //! otherwise \c false.
      constexpr bool operator[](size_t pos) const
      {
        //check(pos);
        return bits & bit_ops::single<T>(pos);
      }

      //! @Requires \c pos shall be valid.
      //! @Throws std::invalid_argument if @c e does not have a valid position.
      //! @Returns An object of type \c bit_mask<N,T>::reference such that
      //! <tt>(*this)[pos] == this->test(pos)</tt>, and such that <tt>(*this)[pos] = val</tt> is
      //! equivalent to <tt>this->set(pos, val)</tt>.
      //! @Note For the purpose of determining the presence of a data race,
      //! any access or update through the resulting reference potentially
      //! accesses or modifies, respectively, the entire underlying  bitset.

      constexpr reference operator[](size_t pos)
      {
        //check(pos);
        return reference(*this, pos);
      }
      //! @Effects Sets all bits in \c *this.
      //! @Returns \c *this.
      //! @NoExcept

      constexpr bit_mask& set() noexcept
      {
        bits = T(~0) & bit_ops::up_to<N,T>();
        return *this;
      }

      //! @Throws @c std::invalid_argument if @c setting does have a invalid bit position.
      //! @Effects Stores a new value in the bit at the position associated to \c setting in \c *this.
      //! If \c value is nonzero, the stored value is one, otherwise it is zero.
      //! @Returns \c *this.
      constexpr bit_mask& set(size_t setting, bool value = true)
      {
        check(setting);
        bits &= ~bit_ops::single<T>(setting);
        bits |= (int(value)<<setting); // fixme
        return *this;
      }

      //! @Effects Resets all bits in \c *this.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& reset() noexcept
      {
        bits = T(0);
        return *this;
      }

      //! @Throws @c std::invalid_argument if \c resetting does not correspond to a valid ordinal.
      //! @Effects Resets the bit at the position associated to \c resetting in \c *this.
      //! @Returns \c *this.

      constexpr bit_mask& reset(size_t resetting)
      {
        check(resetting);
        bits &= ~bit_ops::single<T>(resetting);
        return *this;
      }

      //! @Effects Toggles all bits in \c *this.
      //! @Returns \c *this.
      //! @NoExcept
      constexpr bit_mask& flip() noexcept
      {
        bits = ~bits;
        bits &= bit_ops::up_to<N,T>();
        return *this;
      }

      //! @Throws std::invalid_argument if \c flipping does not correspond to a valid ordinal.
      //! @Effects Toggles the bit at position associated to \c pos in \c *this.
      //! @Returns \c *this.
      constexpr bit_mask& flip(size_t flipping)
      {
        check(flipping);
        bits ^= bit_ops::single<T>(flipping);

        return *this;
      }

      //! \c <tt>T</tt> conversion
      //! @Returns \c x.

      constexpr T to_integer() const noexcept
      {
        return bits;
      }

      //! \c <tt>unsigned long</tt> conversion
      //! @Throws @c std::overflow_error if the integral value \c x corresponding to the
      //! bits in \c *this cannot be represented as type <<tt>unsigned long</tt>.
      //! @Returns \c x.

      constexpr unsigned long to_ulong() const noexcept
      {
        return (unsigned long)(bits);
      }

      //! \c <tt>unsigned long long</tt> conversion
      //! @Returns \c x.
      constexpr unsigned long long to_ullong() const noexcept
      {
        return (unsigned long long)(bits);
      }
#ifdef JASEL_DOXYGEN_INVOKED

      //! @Effects Constructs a string object of the appropriate type and
      //! initializes it to a string of length \c N characters. Each character is
      //! determined by the value of its corresponding bit position in \c *this.
      //! Character position \c N-1 corresponds to bit position zero.
      //! Subsequent decreasing character positions correspond to increasing
      //! bit positions. Bit value zero becomes the character zero, bit value
      //! one becomes the character one.
      //! @Returns The created object.
      template <class charT = char,
        class traits = std::char_traits<charT>,
        class Allocator = std::allocator<charT> >
        std::basic_string<charT, traits, Allocator>
      to_string(charT zero = charT('0'), charT one = charT('1')) const;
#endif
      //! @Effects Constructs an object @c x of class @c bit_mask<N,T> and initializes it with @c *this.
      //! @Returns \c x.flip().
      //! @NoExcept
      constexpr bit_mask operator~() const noexcept
      {
        return bit_mask(*this).flip();
      }

      //! @Requires \c testing is valid
      //! @Throws out_of_range if the associated position of \c testing does not correspond to a valid bit position.
      //! @Returns \c true if the bit at position \c pos in \c *this has the value one.
      constexpr bool test(size_t testing) const
      {
        check(testing);
        return bits & bit_ops::single<T>(testing);
      }

      //! @Returns <tt>count() == size()</tt>
      //! @NoExcept
      constexpr bool all() const noexcept
      {
        return bits == bit_ops::up_to<N,T>();
      }

      //! @Returns <tt>count() != 0</tt>
      //! @NoExcept
      constexpr bool any() const noexcept
      {
        return bits != 0;
      }

      //! @Returns <tt>count() == 0</tt>
      //! @NoExcept
      constexpr bool none() const noexcept
      {
        return bits == 0;
      }

      //! @Returns <tt>bit_mask<N,T>(*this) <<= pos</tt>.
      //! @NoExcept
      constexpr bit_mask operator<<(std::size_t pos) const noexcept
      {
        bit_mask r = *this;
        r <<= pos;
        return r;
      }

      //! @Returns <tt>bit_mask<N,T>(*this) >>= pos</tt>.
      //! @NoExcept
      constexpr bit_mask operator>>(std::size_t pos) const noexcept
      {
        bit_mask r = *this;
        r >>= pos;
        return r;
      }

      //! @Returns A nonzero value if the value of each bit in \c *this equals the
      //! value of the corresponding bit in \c rhs.
      //! @NoExcept
      constexpr bool operator==(const bit_mask& rhs) const noexcept
      {
        return bits == rhs.bits;
      }

      //! @Returns A nonzero value if <tt>!(*this == rhs)</tt>.
      //! @NoExcept
      constexpr bool operator!=(const bit_mask& rhs) const noexcept
      {
        return bits != rhs.bits;
      }
    private:



    };

    // bit_mask operators:

    //! Intersection

    //! @Returns <tt>bit_mask<N,T>(lhs) &= rhs</tt>.
    //! @NoExcept
    template <size_t N, typename T>
    constexpr bit_mask<N,T> operator&(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r &= y;
      return r;
    }

    //! Union

    //! @Returns <tt>bit_mask<N,T>>(lhs) |= rhs</tt>.
    //! @NoExcept
    template <size_t N, typename T >
    constexpr bit_mask<N,T> operator|(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r |= y;
      return r;
    }

    //! Exclusive union

    //! @Returns <tt>bit_mask<N,T>(lhs) ^= rhs</tt>.
    //! @NoExcept
    template <size_t N, typename T >
    constexpr bit_mask<N,T> operator^(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r ^= y;
      return r;
    }

    //! A formatted input function.

    //! @Effects Extracts up to \c N characters from is. Stores these characters
    //! in a temporary object \c str of type <tt>basic_string<charT, traits></tt>, then
    //! evaluates the expression <tt>x = bit_mask<N,T>(str)</tt>. Characters are extracted
    //! and stored until any of the following occurs:
    //! - \c N characters have been extracted and stored;
    //! - end-of-file occurs on the input sequence;
    //! - the next input character is neither \c is.widen('0') nor \c is.widen('1')
    //! (in which case the input character is not extracted).\n
    //! If no characters are stored in \c str, calls \c is.setstate(ios_base::failbit)
    //! (which may throw <tt>ios_- base::failure</tt>).
    //! @Params
    //! @Param{is, the input stream}
    //! @Param{x, the \c bit_mask}
    //! @Returns \c is.

    template <class charT, class traits, size_t N, typename T >
    std::basic_istream<charT, traits>&
    operator>>(std::basic_istream<charT, traits>& is, bit_mask<N,T>& x)
    {
      T v;
      auto& res = is >> v;
      x = bit_mask<N,T>(mask_tag_t{}, v);
      return res;
    }

    //! A formatted output function.

    //! @Params
    //! @Param{os, the output stream}
    //! @Param{x, the \c bit_mask}
    //! @Returns the result of the following expression
    //! @code
    //! os << x.template to_string<charT,traits,allocator<charT> >(
    //!         use_facet<ctype<charT> >(os.getloc()).widen('0'),
    //!         use_facet<ctype<charT> >(os.getloc()).widen('1')
    //!       )
    //! @endcode
    template <class charT, class traits, size_t N, typename T >
    std::basic_ostream<charT, traits>&
    operator<<(std::basic_ostream<charT, traits>& os, const bit_mask<N,T>& x)
    {
      return os << x.to_integer();
    }

    template<size_t N, typename T>
    constexpr bit_mask<N,T> make_bit_mask() noexcept { return bit_mask<N,T>(); }
    template<size_t N, typename T>
    constexpr bit_mask<N,T> make_bit_mask_single(size_t pos) noexcept { return bit_mask<N,T>(pos_tag_t{}, pos); }
    template<size_t N, typename T>
    constexpr bit_mask<N,T> make_bit_mask(T msk) noexcept { return bit_mask<N,T>(mask_tag_t{}, msk); }
    template<size_t N, typename T>
    constexpr bit_mask<N,T> make_bit_mask_fill(bool val) noexcept { return bit_mask<N,T>(fill_tag_t{}, val); }

}} // experimental::fundamental_v3

  //! bit_mask hash template specialization

  template <size_t N, typename T >
  struct hash<experimental::bit_mask<N,T> >
  : public std::unary_function<experimental::bit_mask<N,T>, std::size_t>
  {
    //! \details The template specialization meets the requirements of class template \c hash.
    //! @Params
    //! @Param{es, the @c bit_mask}
    //! @Returns the hash associated to the underlying bitset.
    constexpr std::size_t operator()(const experimental::bit_mask<N,T>& es) const
    {
      return hash<T>(es.to_integer());
    }
  };

} // std

#endif // header
