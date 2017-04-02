//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2011,2015-2016.
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
 The header \c <experimental/ordinal_set.hpp> defines a class template \c ordinal_set<T> for managing sets of ordinals and several
 //! related functions for representing and manipulating sets of ordinals. We can say that
 \c ordinal_set is the counterpart of \c std::bitset when the index are ordinals.
 */

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_SET_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_SET_HPP

#include <experimental/ordinal.hpp>

// todo replace with bit_mask is the size is <= 64
#include <bitset>
#include <stdexcept>
#include <iosfwd>
#include <string>
#include <cstddef>
#include <climits>
#include <string>


namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{


    /**
      @tparam T set's element ordinal
      @par Requires:<br> @c T must be a model of <em>Ordinal</em>.

     The class template @c ordinal_set<T> describes an object that can store a sequence
     consisting of a fixed number of bits, given by @c ordinal::meta::size<T>::value.

     Each bit represents either the value zero (reset) or one (set). To toggle
     a bit is to change the value zero to one, or the value one to zero. Each
     bit has a non-negative position @c pos. When converting between an object of
     class @c ordinal_set<T> and a value of some integral type, bit position pos
     corresponds to the bit value 1<<pos. The integral value corresponding to
     two or more bits is the sum of their bit values.
     The functions described in this section can report three kinds of errors,
     each associated with a distinct exception:
     - an invalid-argument error is associated with exceptions of type @c std::invalid_argument;
     - an out-of-range error is associated with exceptions of type @c std::out_of_range;
     - an overflow error is associated with exceptions of type @c std::overflow_error.

     */
    template<typename T>
    class ordinal_set;

    template<typename T>
    class ordinal_set
    {
    public:
      //! @brief A proxy class that acts as a reference to a single bit.

      //! It contains an assignment operator, a conversion to @c bool,
      //! an @c operator~, and a member function @c flip.
      //! It exists only as a helper class for @c ordinal_set's @c operator[].
      class reference {
        friend class ordinal_set<T>;
        ordinal_set<T>& ref_;
        T pos_;
        reference() noexcept;
        reference(ordinal_set<T>& ref, T pos)
        : ref_(ref), pos_(pos)
        { }
      public:
        ~reference() noexcept
        { }

        //! assignement from bool
        reference& operator=(bool x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! assignement from another reference
        reference& operator=(const reference& x) noexcept
        {
          ref_.set(pos_,x);
          return *this;
        }

        //! flip the bit
        bool operator~() const noexcept
        {
          return ! ref_.test(pos_);
        }

        //! implicit conversion to bool
        operator bool() const noexcept
        {
          return ref_.test(pos_);
        }

        //! flip the bit
        reference flip() noexcept
        {
          ref_.flip(pos_);
          return *this;
        }
      };
      //! @par Effects:<br> Constructs an object of class \c ordinal_set<>, initializing all
      //! ordinals to zero.
      constexpr ordinal_set() noexcept
      {
      }
      // fixme: constexpr
      // Need to be refactored to be a constexpr
      // error: constexpr constructor does not have empty body
      explicit ordinal_set(T setting)
      {
        set(setting);
      }

      //! @par Effects:<br> Constructs an object of class \c ordinal_set<>, initializing the
      //! first \c M bit positions to the corresponding bit values in \c val.
      //! \c M is the smaller of \c N and the number of bits in the value
      //! representation of \c unsigned \c long \c long. If \c M<N, the remaining bit
      //! positions are initialized to zero.

      constexpr explicit ordinal_set(unsigned long long val) noexcept
        : bits(val)
      {
      }
      //#if defined(__GNUC__) &&  (__GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 ))
      // todo implement constructor from string
      #if 1
      #else

      //! @par Requires:<br> <tt>pos <= str.size()</tt>.
      //! @par Throws:<br> std::out_of_range if <tt>pos > str.size()</tt>.
      //! @par Effects:<br> Determines the effective length \c rlen of the initializing
      //! string as the smaller of \c n and <tt>str.size()-pos</tt>.
      //! The function then throws \c std::invalid_argument if any of the \c rlen
      //! characters in \c str beginning at position pos is other than zero or one.
      //! The function uses \c traits::eq() to compare the character values.
      //! Otherwise, the function constructs an object of class \c ordinal_set<T>,
      //! initializing the first \c M bit positions to values determined from the
      //! corresponding characters in the string \c str. \c M is the smaller of \c N and
      //! \c rlen.<br>
      //! An element of the constructed string has value zero if the
      //! corresponding character in \c str, beginning at position \c pos, is \c 0 zero.
      //! Otherwise, the element has the value 1. Character position \c pos+M-1
      //! corresponds to bit position zero. Subsequent decreasing character
      //! positions correspond to increasing bit positions.
      //! If \c M<ordinal::meta::size<T>::value, remaining bit positions are initialized to zero.
      template<class charT, class traits, class TAllocator>
        explicit ordinal_set(const std::basic_string<charT,traits,TAllocator>& str,
                          typename std::basic_string<charT,traits,TAllocator>::size_type pos = 0,
                          typename std::basic_string<charT,traits,TAllocator>::size_type n =
                                   std::basic_string<charT,traits,TAllocator>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str, pos, n, zero, one)
      {}

      //! @par Effects:<br> Constructs an object of class ordinal_set<N> as if by ordinal_set(string(str)).
      template <class charT>
        explicit ordinal_set(const charT* str,
                          typename std::basic_string<charT>::size_type n = std::basic_string<charT>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str,n, zero, one)
      {}
      #endif

      //! @par Effects:<br> Clears each bit in \c *this for which the corresponding bit in
      //! \c rhs is clear, and leaves all other bits unchanged.
      //! @par Returns:<br> \c *this.
      ordinal_set &operator&=(const ordinal_set &rhs) noexcept
      {
        bits &= rhs.bits;
        return *this;
      }

      //! @par Effects:<br> Sets each bit in \c *this for which the corresponding bit in
      //! \c rhs is set, and leaves all other bits unchanged.
      //! @par Returns:<br> \c *this.
      ordinal_set &operator|=(const ordinal_set &rhs) noexcept
      {
        bits |= rhs.bits;
        return *this;
      }

      //! @par Effects:<br> Toggles each bit in \c *this for which the corresponding bit in
      //! \c rhs is set, and leaves all other bits unchanged.
      //! @par Returns:<br> \c *this.
      ordinal_set &operator^=(const ordinal_set &rhs) noexcept
      {
        bits ^= rhs.bits;
        return *this;
      }

      //! @par Effects:<br> Replaces each bit at position \c I in \c *this with a value determined as follows:
      //! - If \c I<pos, the new value is zero;
      //! - If \c I>=pos, the new value is the previous value of the bit at position \c I-pos.
      //! @par Returns:<br> \c *this.
      ordinal_set &operator<<=(size_t pos) noexcept
      {
        bits <<= pos;
        return *this;
      }

      //! @par Effects:<br> Replaces each bit at position \c I in \c *this with a value determined as follows:
      //! - If \c pos>=N-I, the new value is zero;
      //! - If \c pos<N-I, the new value is the previous value of the bit at position \c I+pos.
      //! @par Returns:<br> \c *this.

      ordinal_set &operator>>=(size_t pos) noexcept
      {
        bits >>= pos;
        return *this;
      }

      //! @par Returns:<br> A count of the number of bits set in \c *this.
      std::size_t count() const noexcept
      {
        return bits.count();
      }

      //! @par Returns:<br> \c static_size.
      constexpr std::size_t size() const noexcept
      {
        return bits.size();
      }

      //! @par Requires:<br> \c pos shall be valid.
      //! @par Throws:<br> std::invalid_argument if @c e does not have a valid position.
      //! @par Returns:<br> \c true if the bit at the associated position of \c e in \c *this has the value one,
      //! otherwise \c false.
      constexpr bool operator[](T e) const noexcept
      {
        return bits.test(to_bit(e));
      }

      //! @par Requires:<br> \c pos shall be valid.
      //! @par Throws:<br> std::invalid_argument if @c e does not have a valid position.
      //! @par Returns:<br> An object of type \c ordinal_set<T>::reference such that
      //! <tt>(*this)[pos] == this->test(pos)</tt>, and such that <tt>(*this)[pos] = val</tt> is
      //! equivalent to <tt>this->set(pos, val)</tt>.
      //! @par Note:<br> For the purpose of determining the presence of a data race,
      //! any access or update through the resulting reference potentially
      //! accesses or modifies, respectively, the entire underlying  bitset.

      reference operator[](T pos) noexcept
      {
        return reference(*this,pos);
      }
      //! @par Effects:<br> Sets all bits in \c *this.
      //! @par Returns:<br> \c *this.

      ordinal_set &set() noexcept
      {
        bits.set();
        return *this;
      }

      //! @par Requires:<br> \c setting is valid
      //! @par Throws:<br> @c std::invalid_argument if @c setting does have a invalid bit position.
      //! @par Effects:<br> Stores a new value in the bit at the position associated to \c setting in \c *this.
      //! If \c value is nonzero, the stored value is one, otherwise it is zero.
      //! @par Returns:<br> \c *this.
      ordinal_set &set(T setting, bool value = true)
      {
        bits.set(to_bit(setting), value);
        return *this;
      }

      //! @par Effects:<br> Resets all bits in \c *this.
      //! @par Returns:<br> \c *this.
      ordinal_set &reset() noexcept
      {
        bits.reset();
        return *this;
      }

      //! @par Requires:<br> \c resetting is valid
      //! @par Throws:<br> @c std::invalid_argument if \c resetting does not correspond to a valid ordinal.
      //! @par Effects:<br> Resets the bit at the position associated to \c resetting in \c *this.
      //! @par Returns:<br> \c *this.

      ordinal_set &reset(T resetting)
      {
        bits.reset(to_bit(resetting));
        return *this;
      }

      //! @par Effects:<br> Toggles all bits in \c *this.
      //! @par Returns:<br> \c *this.
      ordinal_set &flip() noexcept
      {
        bits.flip();
        return *this;
      }

      //! @par Requires:<br> \c flipping is valid
      //! @par Throws:<br> std::invalid_argument if \c flipping does not correspond to a valid ordinal.
      //! @par Effects:<br> Toggles the bit at position associated to \c pos in \c *this.
      //! @par Returns:<br> \c *this.
      ordinal_set &flip(T flipping)
      {
        bits.flip(to_bit(flipping));
        return *this;
      }

      //! \c <tt>unsigned long</tt> conversion
      //! @par Throws:<br> @c std::overflow_error if the integral value \c x corresponding to the
      //! bits in \c *this cannot be represented as type <<tt>unsigned long</tt>.
      //! @par Returns:<br> \c x.

      unsigned long to_ulong() const
      {
        return bits.to_ulong();
      }

      //! \c <tt>unsigned long long</tt> conversion
      //! @par Throws:<br> @c std::overflow_error if the integral value \c x corresponding to the
      //!  bits in \c *this cannot be represented as type <tt>unsigned long long</tt>.
      //! @par Returns:<br> \c x.
      unsigned long long to_ullong() const
      {
        return bits.to_ulong();
      }
#if 0 && ! defined JASEL_DOXYGEN_INVOKED

      //! @par Effects: Constructs a string object of the appropriate type and
      //! initializes it to a string of length \c N characters. Each character is
      //! determined by the value of its corresponding bit position in \c *this.
      //! Character position \c N-1 corresponds to bit position zero.
      //! Subsequent decreasing character positions correspond to increasing
      //! bit positions. Bit value zero becomes the character zero, bit value
      //! one becomes the character one.
      //! @par Returns: The created object.
      template <class charT = char,
        class traits = std::char_traits<charT>,
        class Allocator = std::allocator<charT> >
        std::basic_string<charT, traits, Allocator>
      to_string(charT zero = charT('0'), charT one = charT('1')) const;
#endif

      //! @par Effects:<br> Constructs an object @c x of class @c ordinal_set<T> and initializes it with @c *this.
      //! @par Returns:<br> \c x.flip().
      ordinal_set operator~() const noexcept
      {
        return ordinal_set(*this).flip();
      }

      //! @par Requires:<br> \c testing is valid
      //! @par Throws:<br> out_of_range if the associated position of \c testing does not correspond to a valid bit position.
      //! @par Returns:<br> \c true if the bit at position \c pos in \c *this has the value one.
      bool test(T testing) const
      {
        return bits.test(to_bit(testing));
      }

      //! @par Returns:<br> <tt>count() == size()</tt>
      bool all() const noexcept
      {
        return bits.all();
      }

      //! @par Returns:<br> <tt>count() != 0</tt>
      bool any() const noexcept
      {
        return bits.any();
      }

      //! @par Returns:<br> <tt>count() == 0</tt>
      bool none() const noexcept
      {
        return bits.none();
      }

      //! @par Returns:<br> <tt>ordinal_set<T>(*this) <<= pos</tt>.
      ordinal_set operator<<(std::size_t pos) const noexcept
      {
        ordinal_set r = *this;
        r <<= pos;
        return r;
      }

      //! @par Returns:<br> <tt>ordinal_set<T>(*this) >>= pos</tt>.
      ordinal_set operator>>(std::size_t pos) const noexcept
      {
        ordinal_set r = *this;
        r >>= pos;
        return r;
      }

      //! @par Returns:<br> A nonzero value if the value of each bit in \c *this equals the
      //! value of the corresponding bit in \c rhs.
      bool operator==(const ordinal_set& rhs) const noexcept
      {
        return bits == rhs.bits;
      }

      //! @par Returns:<br> A nonzero value if <tt>!(*this == rhs)</tt>.
      bool operator!=(const ordinal_set& rhs) const noexcept
      {
        return bits != rhs.bits;
      }
    private:

      static std::size_t to_bit(T value) noexcept
      {
        return ordinal::pos(value);
      }

      std::bitset<ordinal::meta::size<T>::value> bits;

    public:

      std::bitset<ordinal::meta::size<T>::value> const& detail_bits() const  noexcept { return bits; }
      std::bitset<ordinal::meta::size<T>::value>& detail_bits()  noexcept { return bits; }
    };

    // ordinal_set operators:

    //! Intersection
    //! @par Returns:<br> <tt>ordinal_set<T>(lhs) &= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <typename T>
    ordinal_set<T> operator&(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r &= y;
      return r;
    }

    //! Union
    //! @par Returns:<br> <tt>ordinal_set<T>>(lhs) |= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <typename T >
    ordinal_set<T> operator|(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r |= y;
      return r;
    }

    //! Exclusive union
    //! @par Returns:<br> <tt>ordinal_set<T>(lhs) ^= rhs</tt>.
    //! @par Throws:<br> Nothing
    template <typename T >
    ordinal_set<T> operator^(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r ^= y;
      return r;
    }

    //! A formatted input function.
    //! @par Effects:<br> Extracts up to \c N characters from is. Stores these characters
    //! in a temporary object \c str of type <tt>basic_string<charT, traits></tt>, then
    //! evaluates the expression <tt>x = ordinal_set<T>(str)</tt>. Characters are extracted
    //! and stored until any of the following occurs:
    //! - \c N characters have been extracted and stored;
    //! - end-of-file occurs on the input sequence;
    //! - the next input character is neither \c is.widen('0') nor \c is.widen('1')
    //! (in which case the input character is not extracted).<br>
    //! If no characters are stored in \c str, calls \c is.setstate(ios_base::failbit)
    //! (which may throw <tt>ios_- base::failure</tt>).
    //! @param is the input stream
    //! @param x the \c ordinal_set
    //! @par Returns:<br> \c is.

    template <class charT, class traits, typename T >
    std::basic_istream<charT, traits>&
    operator>>(std::basic_istream<charT, traits>& is, ordinal_set<T>& x)
    {
      return is >> x.detail_bits();
    }

    //! A formatted output function.

    //! @param os the output stream
    //! @param x the \c ordinal_set
    //!
    //! @par Returns:<br> the result of the following expression
    //! @code
    //! os << x.template to_string<charT,traits,allocator<charT> >(
    //!         use_facet<ctype<charT> >(os.getloc()).widen('0'),
    //!         use_facet<ctype<charT> >(os.getloc()).widen('1')
    //!       )
    //! @endcode
    template <class charT, class traits, typename T >
    std::basic_ostream<charT, traits>&
    operator<<(std::basic_ostream<charT, traits>& os, const ordinal_set<T>& x)
    {
      return os << x.detail_bits();
    }

}} // experimental::fundamental_v3

  //! ordinal_set hash template specialization

  template <typename T >
  struct hash<experimental::ordinal_set<T> >
  : public std::unary_function<experimental::ordinal_set<T>, std::size_t>
  {
    //! @par Note:<br> The template specialization meets the requirements of class template \c hash.
    //! @param es the @c ordinal_set
    //! @par Returns: the hash associated to the underlying bitset.
    std::size_t operator()(const experimental::ordinal_set<T>& es) const
    {
      return hash<std::bitset<experimental::ordinal::meta::size<T>::size> >(es.detail_bits());
    }
  };

} // std

#endif // header
