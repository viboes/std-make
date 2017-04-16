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

/*
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

#include <experimental/fundamental/v2/config.hpp>


// todo: Extract bits algorithms in an explicit file

// todo: Add explicit conversions from bit_mask of different sizes

// todo: mixed operations between masks of different sizes
// The bits not represented are as if they were set to 0

// todo see bitset2::bitset2 (https://github.com/ClaasBontus/bitset2) as a generalization of bit_mask a std::bitset.

// todo: Add P0125R0: is_subset_of and is_superset_of (is_proper_subset_of and is_proper_superset_of)

// todo: Add n2050 iteration functions:
// size_type find_first() const;
// size_type find_next(size_type pos) const;
// size_type find_last() const;
// size_type find_prev(size_type pos) const;
// size_type find_first_not() const;
// size_type find_next_not(size_type pos) const;
// size_type find_last_not() const;
// size_type find_prev_not(size_type pos) const;

// todo: Quiz of an interface as if it was a map<size_t, bool> at least for const iteration.

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  struct pos_tag_t {}; //! tag used when passing the position to set in the constructor
  struct fill_tag_t {}; //! tag used when passing a value to fill in the constructor
  struct mask_tag_t {}; //! tag used when passing a mask to set in the constructor

  namespace bit_ops
  {
    //! @par Returns
    //!   The bits in x rotated to the left s times
    template <class T>
    JASEL_MUTABLE_CONSTEXPR T rotl(T x, unsigned int s) noexcept
    {
      constexpr unsigned int N = std::numeric_limits<T>::digits;
      constexpr unsigned int m = s % N;
      return m==0
           ? x
           : (x << m) | (x >> (N - m))
           ;
    }

    //! @par Returns
    //!   The number of bits set to 1 in x
    template <class T>
    constexpr int popcount(T x) noexcept
    {
      return __builtin_popcount(x);
    }

    //! @par Requires:
    //!   N <= sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit N
    template <size_t N, class T=unsigned>
    constexpr T up_to() noexcept
    {
      return (N >= sizeof(T) * CHAR_BIT) ?
          T(-1) : (T(1) << N) - 1;
    }

    //! @par Pre-condition:
    //!   n <= sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with all the bits set to 1 up to the bit n
    template <class T=unsigned>
    constexpr T up_to(size_t n) noexcept
    {
      return (n >= sizeof(T) * CHAR_BIT) ?
          T(-1) : (T(1) << n) - 1;
    }

    //! @par Requires:
    //!   S < sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with the bit S set to 1
    template <size_t S, class T=unsigned>
    constexpr T single() noexcept
    {
      //constexpr unsigned int N = std::numeric_limits<T>::digits;
      //constexpr unsigned int m = S % std::numeric_limits<T>::digits;
      return T(1) << (S % std::numeric_limits<T>::digits);
    }
    //! @par Pre-condition:
    //!   s < sizeof(T) * CHAR_BIT
    //! @par Returns
    //!   A mask with the bit s set to 1
    template <class T>
    constexpr T single(size_t s) noexcept
    {
      //constexpr unsigned int N = std::numeric_limits<T>::digits;
      //const unsigned int m = s % std::numeric_limits<T>::digits;
      return T(1) << (s % std::numeric_limits<T>::digits);
    }
  }
    /**
      @tparam N the number of significant bits on the storage
      @tparam T the storage type
      @par Requires:
        @c N must be @c T must be a model of <em>unsigned Integral</em> type.

     The class template @c bit_mask<N,T> describes an object that can store a sequence
     consisting of a fixed number of bits, given by @c N.

     Each bit represents either the value zero (reset) or one (set). To toggle
     a bit is to change the value zero to one, or the value one to zero. Each
     bit has a non-negative position @c pos. When converting between an object of
     class @c bit_mask<N,T> and a value of some integral type, bit position @c pos
     corresponds to the bit value @c 1<<pos. The integral value corresponding to
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
      // fixme: shouldn't we provide a function that checks the validity of a position?
      JASEL_MUTABLE_CONSTEXPR void check(size_t testing) const
      {
        // fixme: use assert here
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
        constexpr reference(bit_mask<N,T>& ref, T pos) noexcept
          : ref_(ref), pos_(pos)
        { }
      public:
        constexpr reference() noexcept = delete;

        //! assignment from bool
        JASEL_MUTABLE_CONSTEXPR reference& operator=(bool x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! assignment from another reference
        JASEL_MUTABLE_CONSTEXPR reference& operator=(const reference& x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! flip the bit
        constexpr bool operator~() noexcept
        {
          return ! ref_.test(pos_);
        }

        //! implicit conversion to bool
        constexpr operator bool() const noexcept
        {
          return ref_.test(pos_);
        }

        //! flip the bit
        JASEL_MUTABLE_CONSTEXPR reference flip()  noexcept
        {
          ref_.flip(pos_);
          return *this;
        }
      };

      //fixme: If we want bit_mask to be a POD this constructor should be default
      // The user will need to use explicit zero-initialization.
      // bit_mask<5> bm {}
      // This has the advantage that it behaves like the underlying unsigned integer type
      // However std::bitset provides it.
#if 0
      //! @par Effects:<br> Constructs an object of class \c bit_mask<>, initializing all
      //! bits to zero.
      //! @par Throws:<br> Nothing
      constexpr bit_mask() noexcept
          : bits(0)
      {
      }
#endif
      //! @par Effects:
      //!   Construct an uninitialized bit_mask
      //! @par Remark:
      //!   Unlike std::bitset, bit_mask is a POD
      constexpr bit_mask() noexcept = default;

      //! @par Effects:
      //!   Constructs an object of class \c bit_mask<>, initializing the
      //!   \c pos bit position to \c 1 and the others to \c 0.
      //! @par Pre-condition:
      //!   @code pos <= N @endcode
      explicit constexpr bit_mask(pos_tag_t, size_t pos)
        : bits(bit_ops::single<T>(pos) & bit_ops::up_to<N,T>())
      {
        //JASEL_EXPECTS(valid_position(pos));
      }

      //! @par Effects:
      //!   Constructs an object of class \c bit_mask<>, initializing the
      //!   all bit position to \c val.
      //! @par Throws:
      //!   Nothing
      explicit constexpr bit_mask(fill_tag_t, bool val) noexcept
        : bits(val?bit_ops::up_to<N,T>():0)
      {
      }

      //! @par Effects:
      //!   Constructs an object of class \c bit_mask<>, initializing the
      //!   first \c M bit positions to the corresponding bit values in \c val.
      //!   \c M is the smaller of \c N and the number of bits in the value
      //!   representation of \c unsigned \c long \c long. If \c M<N, the remaining bit
      //!   positions are initialized to zero.
      //! @par Throws:
      //!   Nothing

      explicit constexpr bit_mask(mask_tag_t, T val) noexcept
        : bits(val & bit_ops::up_to<N,T>())
      {
      }

      // todo implement this
      //#if defined(__GNUC__) &&  (__GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 ))
      #if 1
      #else

      //! @par Requires:<br>  <tt>pos <= str.size()</tt>.
      //! @par Throws:<br>  std::out_of_range if <tt>pos > str.size()</tt>.
      //! @par Effects:<br> Determines the effective length \c rlen of the initializing
      //! string as the smaller of \c n and <tt>str.size()-pos</tt>.
      //! The function then throws \c std::invalid_argument if any of the \c rlen
      //! characters in \c str beginning at position pos is other than zero or one.
      //! The function uses \c traits::eq() to compare the character values.
      //! Otherwise, the function constructs an object of class \c bit_mask<N,T>,
      //! initializing the first \c M bit positions to values determined from the
      //! corresponding characters in the string \c str. \c M is the smaller of \c N and
      //! \c rlen.<br>
      //! An element of the constructed string has value zero if the
      //! corresponding character in \c str, beginning at position \c pos, is \c 0 zero.
      //! Otherwise, the element has the value 1. Character position \c pos+M-1
      //! corresponds to bit position zero. Subsequent decreasing character
      //! positions correspond to increasing bit positions.
      //! If \c M < numeric_limits<T>::digits, remaining bit positions are initialized to zero.
      template<class charT, class traits, class TAllocator>
        explicit bit_mask(const std::basic_string<charT,traits,TAllocator>& str,
                          typename std::basic_string<charT,traits,TAllocator>::size_type pos = 0,
                          typename std::basic_string<charT,traits,TAllocator>::size_type n =
                                   std::basic_string<charT,traits,TAllocator>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str, pos, n, zero, one)
      {}

      //! @par Effects:<br> Constructs an object of class bit_mask<N> as if by bit_mask(string(str)).
      template <class charT>
        explicit bit_mask(const charT* str,
                          typename std::basic_string<charT>::size_type n = std::basic_string<charT>::npos,
                          charT zero = charT('0'), charT one = charT('1'));
      #endif

      //! @par Effects:
      //!   Clears each bit in \c *this for which the corresponding bit in
      //!   \c rhs is clear, and leaves all other bits unchanged.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& operator&=(const bit_mask& rhs) noexcept
      {
        bits &= rhs.bits;
        return *this;
      }

      //! @par Effects:
      //!   Sets each bit in \c *this for which the corresponding bit in
      //!   \c rhs is set, and leaves all other bits unchanged.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& operator|=(const bit_mask& rhs) noexcept
      {
        bits |= rhs.bits;
        return *this;
      }

      //! @par Effects:
      //!   Toggles each bit in \c *this for which the corresponding bit in
      //!   \c rhs is set, and leaves all other bits unchanged.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& operator^=(const bit_mask& rhs) noexcept
      {
        bits ^= rhs.bits;
        bits &= bit_ops::up_to<N,T>();
        return *this;
      }

      //! @par Effects:
      //!   Replaces each bit at position \c I in \c *this with a value determined as follows:
      //!   - If \c I<pos, the new value is zero;
      //!   - If \c I>=pos, the new value is the previous value of the bit at position \c I-pos.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& operator<<=(size_t pos) noexcept
      {
        bits <<= pos;
        bits &= bit_ops::up_to<N,T>();
        return *this;
      }

      //! @par Effects:
      //!   Replaces each bit at position \c I in \c *this with a value determined as follows:
      //!   - If \c pos>=N-I, the new value is zero;
      //!   - If \c pos<N-I, the new value is the previous value of the bit at position \c I+pos.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing

      JASEL_MUTABLE_CONSTEXPR bit_mask& operator>>=(size_t pos) noexcept
      {
        bits >>= pos;
        bits &= bit_ops::up_to<N,T>();
        return *this;
      }

      //! @par Returns:
      //!   A count of the number of bits set in \c *this.
      constexpr std::size_t count() const noexcept
      {
        return __builtin_popcount(bits);
      }

      //! @par Returns:
      //!   \c static_size.
      constexpr std::size_t size() const noexcept
      {
        return N;
      }

      //! @par Pre-condition:
      //!   \c pos shall be valid.
      //! @par Throws:
      //!   std::out_of_range if @c pos i sout of range.
      //! @par Returns:
      //!   \c true if the bit at the associated position of \c e in \c *this has the value one,
      //! otherwise \c false.
      JASEL_CXX14_CONSTEXPR bool operator[](size_t pos) const
      {
        //JASEL_EXPECTS(valid_position(pos));
        return bits & bit_ops::single<T>(pos);
      }

      //! @par Pre-condition:
      //!   \c pos shall be valid.
      //! @par Throws:
      //!   std::out_of_range if @c e does not have a valid position.
      //! @par Returns:
      //!   An object of type \c bit_mask<N,T>::reference such that
      //!   <tt>(*this)[pos] == this->test(pos)</tt>, and such that <tt>(*this)[pos] = val</tt> is
      //! equivalent to <tt>this->set(pos, val)</tt>.
      //! @par Note:
      //!   For the purpose of determining the presence of a data race,
      //!   any access or update through the resulting reference potentially
      //!   accesses or modifies, respectively, the entire underlying  bitset.

      JASEL_CXX14_CONSTEXPR reference operator[](size_t pos)
      {
        //JASEL_EXPECTS(valid_position(pos));
        return reference(*this, pos);
      }
      //! @par Effects:
      //!   Sets all bits in \c *this.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing

      JASEL_MUTABLE_CONSTEXPR bit_mask& set() noexcept
      {
        bits = T(~0) & bit_ops::up_to<N,T>();
        return *this;
      }

      //! @par Effects:
      //!   Stores a new value in the bit at the position associated to \c setting in \c *this.
      //!   If \c value is nonzero, the stored value is one, otherwise it is zero.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   @c std::out_of_range if @c setting does have a invalid bit position.
      JASEL_MUTABLE_CONSTEXPR bit_mask& set(size_t setting, bool value = true)
      {
        check(setting);
        bits &= ~bit_ops::single<T>(setting);
        bits |= (int(value)<<setting); // fixme
        return *this;
      }

      //! @par Effects:<br>
      //!   Resets all bits in \c *this.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& reset() noexcept
      {
        bits = T(0);
        return *this;
      }

      //! @par Throws:
      //!   @c std::out_of_range if \c resetting does not correspond to a valid position.
      //! @par Effects:
      //!   Resets the bit at the position associated to \c resetting in \c *this.
      //! @par Returns:
      //!   \c *this.

      JASEL_MUTABLE_CONSTEXPR bit_mask& reset(size_t resetting)
      {
        check(resetting);
        bits &= ~bit_ops::single<T>(resetting);
        return *this;
      }

      //! @par Effects:
      //!   Toggles all bits in \c *this.
      //! @par Returns:
      //!   \c *this.
      //! @par Throws:
      //!   Nothing
      JASEL_MUTABLE_CONSTEXPR bit_mask& flip() noexcept
      {
        bits = ~bits;
        bits &= bit_ops::up_to<N,T>();
        return *this;
      }

      //! @par Throws:
      //!   std::out_of_range if \c flipping does not correspond to a valid position.
      //! @par Effects:
      //!   Toggles the bit at position associated to \c pos in \c *this.
      //! @par Returns:
      //!   \c *this.
      JASEL_MUTABLE_CONSTEXPR bit_mask& flip(size_t flipping)
      {
        check(flipping);
        bits ^= bit_ops::single<T>(flipping);
        bits &= bit_ops::up_to<N,T>();

        return *this;
      }

      //! \c <tt>T</tt> conversion
      //! @par Returns:
      //!   \c x.
      constexpr T to_integer() const noexcept
      {
        return bits;
      }

      //! \c <tt>unsigned long</tt> conversion
      //! @par Returns:
      //!   \c x.
      //! @par Throws:
      //!   @c std::overflow_error if the integral value \c x corresponding to the
      //!   bits in \c *this cannot be represented as type <<tt>unsigned long</tt>.

      constexpr unsigned long to_ulong() const noexcept
      {
        return (unsigned long)(bits);
      }

      //! \c <tt>unsigned long long</tt> conversion
      //! @par Returns:
      //! \c x.
      constexpr unsigned long long to_ullong() const noexcept
      {
        return (unsigned long long)(bits);
      }
#if 0

      //! @par Effects:<br> Constructs a string object of the appropriate type and
      //! initializes it to a string of length \c N characters. Each character is
      //! determined by the value of its corresponding bit position in \c *this.
      //! Character position \c N-1 corresponds to bit position zero.
      //! Subsequent decreasing character positions correspond to increasing
      //! bit positions. Bit value zero becomes the character zero, bit value
      //! one becomes the character one.
      //! @par Returns:<br>   The created object.
      template <class charT = char,
        class traits = std::char_traits<charT>,
        class Allocator = std::allocator<charT> >
        std::basic_string<charT, traits, Allocator>
      to_string(charT zero = charT('0'), charT one = charT('1')) const;
#endif

      //! @par Effects:
      //!   Constructs an object @c x of class @c bit_mask<N,T> and initializes it with @c *this.
      //! @par Returns:
      //!   \c x.flip().
      constexpr bit_mask operator~() const noexcept
      {
        return bit_mask(*this).flip();
      }

      //! @par Returns:
      //!   \c true if the bit at position \c pos in \c *this has the value one.
      //! @par Throws:
      //!   std::out_of_range if the associated position of \c testing does not correspond to a valid bit position.
      JASEL_MUTABLE_CONSTEXPR bool test(size_t testing) const
      {
        check(testing);
        return bits & bit_ops::single<T>(testing);
      }

      //! @par Returns:
      //!   <tt>count() == size()</tt>
      constexpr bool all() const noexcept
      {
        return bits == bit_ops::up_to<N,T>();
      }

      //! @par Returns:
      //!   <tt>count() != 0</tt>
      constexpr bool any() const noexcept
      {
        return bits != 0;
      }

      //! @par Returns:
      //!   <tt>count() == 0</tt>
      constexpr bool none() const noexcept
      {
        return bits == 0;
      }

      //! @par Returns:
      //!   <tt>bit_mask<N,T>(*this) <<= pos</tt>.
      //! @par Throws:<br> Nothing
      JASEL_CXX14_CONSTEXPR bit_mask operator<<(std::size_t pos) const noexcept
      {
        bit_mask r = *this;
        r <<= pos;
        return r;
      }

      //! @par Returns:
      //!   <tt>bit_mask<N,T>(*this) >>= pos</tt>.
      //! @par Throws:<br> Nothing
      JASEL_CXX14_CONSTEXPR bit_mask operator>>(std::size_t pos) const noexcept
      {
        bit_mask r = *this;
        r >>= pos;
        return r;
      }

      //! @par Returns:
      //!   A nonzero value if the value of each bit in \c *this equals the
      //!   value of the corresponding bit in \c rhs.
      constexpr bool operator==(const bit_mask& rhs) const noexcept
      {
        return bits == rhs.bits;
      }

      //! @par Returns:
      //!   A nonzero value if <tt>!(*this == rhs)</tt>.
      constexpr bool operator!=(const bit_mask& rhs) const noexcept
      {
        return bits != rhs.bits;
      }
    private:

    };

    // bit_mask operators:

    //! Intersection
    //! @par Returns:<br>   <tt>bit_mask<N,T>(lhs) &= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <size_t N, typename T>
    JASEL_CXX14_CONSTEXPR bit_mask<N,T> operator&(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r &= y;
      return r;
    }

    //! Union
    //! @par Returns:<br>   <tt>bit_mask<N,T>>(lhs) |= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <size_t N, typename T >
    JASEL_CXX14_CONSTEXPR bit_mask<N,T> operator|(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r |= y;
      return r;
    }

    //! Exclusive union
    //! @par Returns:<br>   <tt>bit_mask<N,T>(lhs) ^= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <size_t N, typename T >
    JASEL_CXX14_CONSTEXPR bit_mask<N,T> operator^(const bit_mask<N,T>& x, const bit_mask<N,T>& y) noexcept
    {
      bit_mask<N,T> r = x;
      r ^= y;
      return r;
    }

    //! A formatted input function.
    //! @par Effects:<br> Extracts up to \c N characters from is. Stores these characters
    //! in a temporary object \c str of type <tt>basic_string<charT, traits></tt>, then
    //! evaluates the expression <tt>x = bit_mask<N,T>(str)</tt>. Characters are extracted
    //! and stored until any of the following occurs:
    //! - \c N characters have been extracted and stored;
    //! - end-of-file occurs on the input sequence;
    //! - the next input character is neither \c is.widen('0') nor \c is.widen('1')
    //! (in which case the input character is not extracted).<br>
    //! If no characters are stored in \c str, calls \c is.setstate(ios_base::failbit)
    //! (which may throw <tt>ios_- base::failure</tt>).
    //!
    //! @param is the input stream
    //! @param x the \c bit_mask
    //! @par Returns:<br>   \c is.

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
    //!
    //! @param os the output stream
    //! @param x the \c bit_mask
    //! @par Returns:<br>   the result of the following expression
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
    //!
    //! @param es the @c bit_mask
    //! @par Returns:<br>   the hash associated to the underlying bitset.
    constexpr std::size_t operator()(const experimental::bit_mask<N,T>& es) const
    {
      return hash<T>(es.to_integer());
    }
  };

} // std

#endif // header
