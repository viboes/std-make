//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2017.
// Distributed under the Boost
// Software License, Version 1.0.
// (See accompanying file LICENSE_1_0.txt or
// copy at http://www.boost.org/LICENSE_1_0.txt)
//
//
//////////////////////////////////////////////////////////////////////////////

/*
 \file
 \brief
 The header \c <experimental/.../subword/subword_reference.hpp> defines a subword reference to a subword located inside a word.

 */

#ifndef JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_REFERENCE_HPP
#define JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_REFERENCE_HPP


#include <experimental/fundamental/v3/bits/binary_digits.hpp>
#include <experimental/fundamental/v3/bits/bit_ops.hpp>
#include <experimental/fundamental/v3/word/word.hpp>

#include <experimental/fundamental/v3/contract/constexpr_assert.hpp>
#include <experimental/fundamental/v2/config.hpp>

#include <iosfwd>
#include <type_traits>
#include <cstring>

// fixme: which operation should a subword_reference provide?
// If we see them just as a reference to a set of bits it should provide the bitwise operations
// Should it provide the bit_set operations?
// However, subword_reference is a T proxy

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <int Bits, class WordType, class T>
  class subword_reference
  {
  public:
      // Types and Constants
      using word_type = WordType;
      using subword_type = T;

      static_assert(is_unsigned<word_type>::value, "A WordType must be unsigned");
      static_assert(is_unsigned<subword_type>::value, "A T must be unsigned");

      static constexpr int subword_digits = Bits;
      static constexpr int word_digits = binary_digits<word_type>::value;

      // fixme: this works only for T that are unsigned integral types.
      // But we want to store here any trivial type that has Bits significant bits
      // We need an unsigned type associated to T, so that we can memcpy from T to subword and vice-versa
      // Note that this forbids to store signed types.
      // An alternative is change the meaning of the T parameter of subword_value<Bits, T> so that the storage is an unsigned,
      // but the conversion from from subword to T add the sign if the type is an signed type
      using subword = subword_value<subword_digits, T>;

      using index_type = typename subword::index_type;
      using size_type = typename subword::size_type;

      //static_assert(is_multiword<word_type>::value, "");
      static_assert(is_trivially_copyable<subword_type>::value, "A subword must be trivially copyable");
      static_assert(subword_digits <= binary_digits<T>::value, "");
      static_assert(word_digits % subword_digits == 0, "");

      // Friendship
  private:
      template <int, class, class> friend class subword_reference;
      friend class subword_pointer<Bits, WordType, T>;

      // Lifecycle
  public:
      constexpr subword_reference(const subword_reference& other) noexcept = default;
      template <class W>
      constexpr subword_reference(const subword_reference<Bits, W, T>& other) noexcept
      : _ptr(other._ptr)
      , _pos(other._pos)
      {
      }
      explicit constexpr subword_reference(word_type& ref) noexcept
      : _ptr(&ref)
      , _pos(0)
      {
      }
      constexpr subword_reference(word_type& ref, index_type pos)
      : _ptr((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), &ref))
      , _pos(pos)
      {
      }

      // Assignment
  public:
      JASEL_CXX14_CONSTEXPR subword_reference& operator=(subword_reference const& other) noexcept
      {
          set(other.value());
          return *this;
      }
      template <class W>
      JASEL_CXX14_CONSTEXPR subword_reference& operator=(subword_reference<Bits, W, T> const& other) noexcept
      {
          set(other.value());
          return *this;
      }
      // fixme: We can not define a assignment from word_type and value_type if both are the same.
      //
//      constexpr subword_reference& operator=(value_type const& val) noexcept
//      {
//          set(val);
//          return *this;
//      }
      JASEL_CXX14_CONSTEXPR subword_reference& operator=(subword const& val) noexcept
      {
          set(val);
          return *this;
      }
      JASEL_CXX14_CONSTEXPR subword_reference& assign(word_type const& val) noexcept
      {
          return *this=subword(val);
      }
      JASEL_CXX14_CONSTEXPR subword_reference& assign(word_type const& val, index_type pos)
      {
          JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value);
          return *this=subword(val, pos);
      }

      // Bitwise assignment operators
  public:
      JASEL_CXX14_CONSTEXPR subword_reference& operator&=(subword const& other) noexcept
      {
          *_ptr &= ~mask() | mask(other,_pos);
          return *this;
      }
      JASEL_CXX14_CONSTEXPR subword_reference& operator|=(subword const& other) noexcept
      {
          *_ptr |= ~mask() | mask(other,_pos);
          return *this;
      }
      JASEL_CXX14_CONSTEXPR subword_reference& operator^=(subword const& other) noexcept
      {
          *_ptr ^= mask(other,_pos);
          return *this;
      }

      // Conversion
  public:
      constexpr operator subword() const noexcept
      {
        return subword(*_ptr , _pos);
      }
      // fixme:: should value return word_type or value_type?
      // what name for retrieving the word_type?
      word_type value() const noexcept
      {
          return subword(*this).value();
      }
      public:
      // fixme: cannot be constexpr because the function is declaring a variable uninitialized and using memcpy?
      explicit operator T() const noexcept
      {
        //return T(subword(*this)); // if explicitly convertible
        subword s(*_ptr , _pos); // if trivially_copiable
        T t;
        memcpy(&t, &s, sizeof(T));
        return t;
      }

      // Access
      public:
      constexpr subword_pointer<Bits, WordType> operator&() const noexcept
      {
          return subword_pointer<Bits, WordType>(_ptr, position());
      }

      // Swap members
      public:
      template <class W>
      void swap(subword_reference<Bits, W, T> other)
      {
          if (other != *this) {
              subword s(*this);
              assign(other);
              other.assign(s);
          }
      }
      void swap(subword& other)
      {
            subword s(*this);
            assign(other);
            other.assign(s);
      }
      // Bit manipulation
      public:
      JASEL_CXX14_CONSTEXPR subword_reference& set(subword other) noexcept
      {
          reset();
          *_ptr |= ~mask() | mask(other, _pos);
          return *this;
      }
      JASEL_CXX14_CONSTEXPR subword_reference& set(T other) noexcept
      {
          return set(subword(other));
      }
      JASEL_CXX14_CONSTEXPR subword_reference& reset() noexcept
      {
          *_ptr &= ~mask();
          return *this;
      }
      JASEL_CXX14_CONSTEXPR subword_reference& flip() noexcept
      {
          *_ptr ^= mask();
          return *this;
      }
      // Underlying details
      public:
      constexpr word_type* address() const noexcept
      {
          return _ptr;
      }
      constexpr size_type position() const noexcept
      {
          return _pos;
      }
      static constexpr word_type static_mask = bit_ops::up_to<subword_digits, word_type>();
      constexpr word_type mask() const noexcept
      {
          return static_mask << (subword_digits * _pos);
      }
      constexpr word_type mask(subword sw, size_type pos) const noexcept
      {
          return word_type(sw.value()) << (subword_digits * pos);
      }

      // Implementation details: function members
      private:
      subword_reference() noexcept = default;
      explicit constexpr subword_reference(std::nullptr_t) noexcept
      : _ptr(nullptr)
      , _pos(0)
      {
      }
      explicit constexpr subword_reference(word_type* ptr) noexcept
      : _ptr(ptr)
      , _pos(0)
      {
      }
      constexpr subword_reference(word_type* ptr, index_type pos)
      : _ptr((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), ptr))
      , _pos(pos)
      {
      }
      JASEL_CXX14_CONSTEXPR void assign_as_pointer(std::nullptr_t) noexcept
      {
        _ptr = nullptr;
        _pos = 0;
      }
      JASEL_CXX14_CONSTEXPR void assign_as_pointer(subword_reference rhs) noexcept
      {
        _ptr = rhs._ptr;
        _pos = rhs._pos;
      }
      template <class W>
      JASEL_CXX14_CONSTEXPR void assign_as_pointer(subword_reference<Bits, W, T> rhs) noexcept
      {
        _ptr = rhs._ptr;
        _pos = rhs._pos;
      }

      JASEL_CXX14_CONSTEXPR void inc_as_pointer()
      {
          //constexpr size_type word_digits = binary_digits<word_type>::value;
          if ((_pos*subword_digits) + subword_digits < word_digits) {
              ++_pos;
          } else {
              ++_ptr;
              _pos = 0;
          }
          //return *this;
      }
      JASEL_CXX14_CONSTEXPR void dec_as_pointer()
      {
          //constexpr size_type word_digits = binary_digits<word_type>::value;
          if ((_pos*subword_digits) > subword_digits) {
              --_pos ;
          } else {
              --_ptr;
              _pos = (word_digits / subword_digits)-1;
          }
          //return *this;
      }
      // Implementation details: data members
      private:
      word_type* _ptr;
      index_type _pos;
  };

  // Swap
  template <int B, class W1, class W2, class T>
  void swap(
      subword_reference<B, W1, T> lhs,
      subword_reference<B, W2, T> rhs
  ) noexcept
  {
      lhs.swap(rhs);
  }
  template <int B, class W, class T >
  void swap(
      subword_reference<B, W, T> lhs,
      subword_value<B, T>& rhs
  ) noexcept
  {
    lhs.swap(rhs);
  }
  template <int B, class W, class T>
  void swap(
      subword_value<B, T>& lhs,
      subword_reference<B, W, T> rhs
  ) noexcept
  {
    rhs.swap(lhs);
  }

  // Stream functions
  template <class CharT, class Traits, int B, class W, class T>
  std::basic_istream<CharT, Traits>& operator>>(
      std::basic_istream<CharT, Traits>& is,
      subword_reference<B, W, T>& x
  )
  {
    T t;
    is >> t;
    x.assign(t);
    return is;
  }
  template <class CharT, class Traits, int B, class W, class T>
  std::basic_ostream<CharT, Traits>& operator<<(
      std::basic_ostream<CharT, Traits>& os,
      subword_reference<B, W, T> x
  )
  {
      return os << T(x);
  }

}}
} // std

#endif // header
