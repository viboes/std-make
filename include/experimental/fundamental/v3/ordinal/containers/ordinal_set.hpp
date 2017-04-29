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
 The header `<experimental/ordinal_set.hpp>` defines a class template `ordinal_set<T>` for managing sets of ordinals and several
 //! related functions for representing and manipulating sets of ordinals. We can say that
 `ordinal_set` is the counterpart of `std::bitset` when the index are ordinals.
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

      <b>Requires:</b> `T` must be a model of <em>Ordinal</em>.

     The class template `ordinal_set<T>` describes an object that can store a sequence
     consisting of a fixed number of bits, given by `ordinal::meta::size<T>::value`.

     Each bit represents either the value zero (reset) or one (set). To toggle
     a bit is to change the value zero to one, or the value one to zero. Each
     bit has a non-negative position `pos`. When converting between an object of
     class `ordinal_set<T>` and a value of some integral type, bit position `pos`
     corresponds to the bit value 1<<pos. The integral value corresponding to
     two or more bits is the sum of their bit values.
     The functions described in this section can report three kinds of errors,
     each associated with a distinct exception:
     - an invalid-argument error is associated with exceptions of type `std::invalid_argument`;
     - an out-of-range error is associated with exceptions of type `std::out_of_range`;
     - an overflow error is associated with exceptions of type `std::overflow_error`.

     */
    template<typename T>
    class ordinal_set;

    template<typename T>
    class ordinal_set
    {
    public:
      //! @brief A proxy class that acts as a reference to a single bit.

      //! It contains an assignment operator, a conversion to `bool`,
      //! an `operator~`, and a member function `flip`.
      //! It exists only as a helper class for `ordinal_set`'s `operator[]`.
      class reference {
        friend class ordinal_set<T>;
        ordinal_set<T>* ref_;
        T pos_;
        reference() noexcept;
        reference(ordinal_set<T>& ref, T pos)
        : ref_(&ref), pos_(pos)
        { }
      public:
        ~reference() noexcept = default;
        reference(reference const& x) noexcept  = default;
        reference& operator=(const reference& x) noexcept = default;

        //! assignement from bool
        reference& operator=(bool x) noexcept
        {
          ref_->set(pos_,x);
          return *this;
        }

        //! flip the bit
        bool operator~() const noexcept
        {
          return ! ref_->test(pos_);
        }

        //! implicit conversion to bool
        operator bool() const noexcept
        {
          return ref_->test(pos_);
        }

        //! flip the bit
        reference flip() noexcept
        {
          ref_->flip(pos_);
          return *this;
        }
      };
      //! \n<b>Effects:</b> Constructs an object of class `ordinal_set<>`, initializing all
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

      //! \n<b>Effects:</b> Constructs an object of class `ordinal_set<>`, initializing the
      //! first `M` bit positions to the corresponding bit values in `val`.
      //! `M` is the smaller of `N` and the number of bits in the value
      //! representation of `unsigned long long`. If `M < N`, the remaining bit
      //! positions are initialized to zero.

      constexpr explicit ordinal_set(unsigned long long val) noexcept
        : bits(val)
      {
      }
      //#if defined(__GNUC__) &&  (__GNUC__ < 4 || ( __GNUC__ == 4 && __GNUC_MINOR__ < 4 ))
      // todo implement constructor from string
      #if 1
      #else

      //! \n<b>Requires:</b> `pos <= str.size()`.
      //! \n<b>Throws:</b> std::out_of_range if `pos > str.size()`.
      //! \n<b>Effects:</b> Determines the effective length `rlen` of the initializing
      //! string as the smaller of `n` and `str.size()-pos`.
      //! The function then throws `std::invalid_argument` if any of the `rlen`
      //! characters in `str` beginning at position pos is other than zero or one.
      //! The function uses `traits::eq()` to compare the character values.
      //! Otherwise, the function constructs an object of class `ordinal_set<T>`,
      //! initializing the first `M` bit positions to values determined from the
      //! corresponding characters in the string `str`. `M` is the smaller of `N` and
      //! `rlen`.<br>
      //! An element of the constructed string has value zero if the
      //! corresponding character in `str`, beginning at position `pos`, is `0` zero.
      //! Otherwise, the element has the value `1`. Character position `pos + M - 1`
      //! corresponds to bit position zero. Subsequent decreasing character
      //! positions correspond to increasing bit positions.
      //! If `M<ordinal::meta::size<T>::value`, remaining bit positions are initialized to zero.
      template<class charT, class traits, class TAllocator>
        explicit ordinal_set(const std::basic_string<charT,traits,TAllocator>& str,
                          typename std::basic_string<charT,traits,TAllocator>::size_type pos = 0,
                          typename std::basic_string<charT,traits,TAllocator>::size_type n =
                                   std::basic_string<charT,traits,TAllocator>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str, pos, n, zero, one)
      {}

      //! \n<b>Effects:</b> Constructs an object of class ordinal_set<N> as if by ordinal_set(string(str)).
      template <class charT>
        explicit ordinal_set(const charT* str,
                          typename std::basic_string<charT>::size_type n = std::basic_string<charT>::npos,
                          charT zero = charT('0'), charT one = charT('1'))
        : bits(str,n, zero, one)
      {}
      #endif

      //! \n<b>Effects:</b> Clears each bit in `*this` for which the corresponding bit in
      //! `rhs` is clear, and leaves all other bits unchanged.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &operator&=(const ordinal_set &rhs) noexcept
      {
        bits &= rhs.bits;
        return *this;
      }

      //! \n<b>Effects:</b> Sets each bit in `*this` for which the corresponding bit in
      //! `rhs` is set, and leaves all other bits unchanged.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &operator|=(const ordinal_set &rhs) noexcept
      {
        bits |= rhs.bits;
        return *this;
      }

      //! \n<b>Effects:</b> Toggles each bit in `*this` for which the corresponding bit in
      //! `rhs` is set, and leaves all other bits unchanged.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &operator^=(const ordinal_set &rhs) noexcept
      {
        bits ^= rhs.bits;
        return *this;
      }

      //! \n<b>Effects:</b> Replaces each bit at position `I` in `*this` with a value determined as follows:
      //! - If `I < pos`, the new value is zero;
      //! - If `I >= pos`, the new value is the previous value of the bit at position `I - pos`.
      //!
      //! <b>Returns:</b> `*this`.
      ordinal_set &operator<<=(size_t pos) noexcept
      {
        bits <<= pos;
        return *this;
      }

      //! \n<b>Effects:</b> Replaces each bit at position `I` in `*this` with a value determined as follows:
      //! - If `pos >= N-I`, the new value is zero;
      //! - If `pos < N-I`, the new value is the previous value of the bit at position `I+pos`.
      //!
      //! <b>Returns:</b> `*this`.

      ordinal_set &operator>>=(size_t pos) noexcept
      {
        bits >>= pos;
        return *this;
      }

      //! \n<b>Returns:</b> A count of the number of bits set in `*this`.
      std::size_t count() const noexcept
      {
        return bits.count();
      }

      //! \n<b>Returns:</b> `static_size`.
      constexpr std::size_t size() const noexcept
      {
        return bits.size();
      }

      //! \n<b>Requires:</b> `pos` shall be valid.
      //! \n<b>Throws:</b> `std::invalid_argument` if `e` does not have a valid position.
      //! \n<b>Returns:</b> `true` if the bit at the associated position of `e` in `*this` has the value one,
      //! otherwise `false`.
      constexpr bool operator[](T e) const noexcept
      {
        return bits.test(to_bit(e));
      }

      //! \n<b>Requires:</b> `pos` shall be valid.
      //! \n<b>Throws:</b> `std::invalid_argument` if `e` does not have a valid position.
      //! \n<b>Returns:</b> An object of type `ordinal_set<T>::reference` such that
      //! `(*this)[pos] == this->test(pos)`, and such that `(*this)[pos] = val` is
      //! equivalent to `this->set(pos, val)`.
      //! \n<b>Note:</b> For the purpose of determining the presence of a data race,
      //! any access or update through the resulting reference potentially
      //! accesses or modifies, respectively, the entire underlying  bitset.

      reference operator[](T pos) noexcept
      {
        return reference(*this,pos);
      }
      //! \n<b>Effects:</b> Sets all bits in `*this`.
      //! \n<b>Returns:</b> `*this`.

      ordinal_set &set() noexcept
      {
        bits.set();
        return *this;
      }

      //! \n<b>Requires:</b> `setting` is valid
      //! \n<b>Throws:</b> `std::invalid_argument` if `setting` does have a invalid bit position.
      //! \n<b>Effects:</b> Stores a new value in the bit at the position associated to `setting` in `*this`.
      //! If `value` is non-zero, the stored value is one, otherwise it is zero.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &set(T setting, bool value = true)
      {
        bits.set(to_bit(setting), value);
        return *this;
      }

      //! \n<b>Effects:</b> Resets all bits in `*this`.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &reset() noexcept
      {
        bits.reset();
        return *this;
      }

      //! \n<b>Requires:</b> `resetting` is valid
      //! \n<b>Throws:</b> `std::invalid_argument` if `resetting` does not correspond to a valid ordinal.
      //! \n<b>Effects:</b> Resets the bit at the position associated to `resetting` in `*this`.
      //! \n<b>Returns:</b> `*this`.

      ordinal_set &reset(T resetting)
      {
        bits.reset(to_bit(resetting));
        return *this;
      }

      //! \n<b>Effects:</b> Toggles all bits in `*this`.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &flip() noexcept
      {
        bits.flip();
        return *this;
      }

      //! \n<b>Requires:</b> `flipping` is valid
      //! \n<b>Throws:</b> `std::invalid_argument` if `flipping` does not correspond to a valid ordinal.
      //! \n<b>Effects:</b> Toggles the bit at position associated to `pos` in `*this`.
      //! \n<b>Returns:</b> `*this`.
      ordinal_set &flip(T flipping)
      {
        bits.flip(to_bit(flipping));
        return *this;
      }

      //! `unsigned long` conversion
      //! \n<b>Throws:</b> `std::overflow_error` if the integral value `x` corresponding to the
      //! bits in `*this` cannot be represented as type `unsigned long`.
      //! \n<b>Returns:</b> `x`.

      unsigned long to_ulong() const
      {
        return bits.to_ulong();
      }

      //! `unsigned long long` conversion
      //! \n<b>Throws:</b> `std::overflow_error` if the integral value `x` corresponding to the
      //!  bits in `*this` cannot be represented as type `unsigned long long`.
      //! \n<b>Returns:</b> `x`.
      unsigned long long to_ullong() const
      {
        return bits.to_ulong();
      }
#if 0 && ! defined JASEL_DOXYGEN_INVOKED

      //! \n<b>Effects:</b> Constructs a string object of the appropriate type and
      //! initializes it to a string of length `N` characters. Each character is
      //! determined by the value of its corresponding bit position in `*this`.
      //! Character position `N-1` corresponds to bit position zero.
      //! Subsequent decreasing character positions correspond to increasing
      //! bit positions. Bit value zero becomes the character zero, bit value
      //! one becomes the character one.
      //! \n<b>Returns:</b> The created object.
      template <class charT = char,
        class traits = std::char_traits<charT>,
        class Allocator = std::allocator<charT> >
        std::basic_string<charT, traits, Allocator>
      to_string(charT zero = charT('0'), charT one = charT('1')) const;
#endif

      //! \n<b>Effects:</b> Constructs an object `x` of class `ordinal_set<T>` and initializes it with `*this`.
      //! \n<b>Returns:</b> `x.flip()`.
      ordinal_set operator~() const noexcept
      {
        return ordinal_set(*this).flip();
      }

      //! \n<b>Requires:</b> `testing` is valid
      //! \n<b>Throws:</b> `std::out_of_range` if the associated position of `testing` does not correspond to a valid bit position.
      //! \n<b>Returns:</b> `true` if the bit at position `testing` in `*this` has the value one.
      bool test(T testing) const
      {
        return bits.test(to_bit(testing));
      }

      //! \n<b>Returns:</b> `count() == size()`
      bool all() const noexcept
      {
        return bits.all();
      }

      //! \n<b>Returns:</b> `count() != 0`
      bool any() const noexcept
      {
        return bits.any();
      }

      //! \n<b>Returns:</b> `count() == 0`
      bool none() const noexcept
      {
        return bits.none();
      }

      //! \n<b>Equivalent to:</b> `ordinal_set<T>(*this) <<= pos`.
      ordinal_set operator<<(std::size_t pos) const noexcept
      {
        ordinal_set r = *this;
        r <<= pos;
        return r;
      }

      //! \n<b>Equivalent to:</b> `ordinal_set<T>(*this) >>= pos`.
      ordinal_set operator>>(std::size_t pos) const noexcept
      {
        ordinal_set r = *this;
        r >>= pos;
        return r;
      }

      //! \n<b>Returns:</b> A nonzero value if the value of each bit in `*this` equals the
      //! value of the corresponding bit in `rhs`.
      bool operator==(const ordinal_set& rhs) const noexcept
      {
        return bits == rhs.bits;
      }

      //! \n<b>Returns:</b> A nonzero value if `!(*this == rhs)`.
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
    //! \n<b>Returns:</b> `ordinal_set<T>(lhs) &= rhs`.
    //! \n<b>Throws:</b> Nothing
    template <typename T>
    ordinal_set<T> operator&(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r &= y;
      return r;
    }

    //! Union
    //! \n<b>Returns:</b> `ordinal_set<T>>(lhs) |= rhs`.
    //! \n<b>Throws:</b> Nothing
    template <typename T >
    ordinal_set<T> operator|(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r |= y;
      return r;
    }

    //! Exclusive union
    //! \n<b>Returns:</b> `ordinal_set<T>(lhs) ^= rhs`.
    //! \n<b>Throws:</b> Nothing
    template <typename T >
    ordinal_set<T> operator^(const ordinal_set<T>& x, const ordinal_set<T>& y)
    {
      ordinal_set<T> r = x;
      r ^= y;
      return r;
    }

    //! A formatted input function.
    //! \n<b>Effects:</b> Extracts up to `N` characters from is. Stores these characters
    //! in a temporary object `str` of type `basic_string<charT, traits>`, then
    //! evaluates the expression `x = ordinal_set<T>(str)`. Characters are extracted
    //! and stored until any of the following occurs:
    //! - `N` characters have been extracted and stored;
    //! - end-of-file occurs on the input sequence;
    //! - the next input character is neither `is.widen('0')` nor `is.widen('1')`
    //! (in which case the input character is not extracted).<br>
    //! If no characters are stored in `str`, calls `is.setstate(ios_base::failbit)`
    //! (which may throw `ios_base::failure`).
    //! @param is the input stream
    //! @param x the `ordinal_set`
    //! \n<b>Returns:</b> `is`.

    template <class charT, class traits, typename T >
    std::basic_istream<charT, traits>&
    operator>>(std::basic_istream<charT, traits>& is, ordinal_set<T>& x)
    {
      return is >> x.detail_bits();
    }

    //! A formatted output function.
    //! @param os the output stream
    //! @param x the `ordinal_set`
    //!
    //! \n<b>Returns:</b> the result of the following expression
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
    //! \n<b>Note:</b> The template specialization meets the requirements of class template `hash`.
    //! @param es the `ordinal_set`
    //! \n<b>Returns:</b> the hash associated to the underlying bitset.
    std::size_t operator()(const experimental::ordinal_set<T>& es) const
    {
      return hash<std::bitset<experimental::ordinal::meta::size<T>::size> >(es.detail_bits());
    }
  };

} // std

#endif // header
