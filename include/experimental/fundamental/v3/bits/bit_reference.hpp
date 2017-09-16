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
 The header \c <experimental/.../bits/bit_reference.hpp> defines a bit reference to a bit located inside a word.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf

 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BIT_REFERENCE_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BIT_REFERENCE_HPP


#include <experimental/fundamental/v3/bits/binary_digits.hpp>
#include <experimental/fundamental/v3/bits/bit_ops.hpp>

#include <experimental/fundamental/v3/contract/constexpr_assert.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
#if 0
  template <class WordType>
  class bit_reference
  {
      // Assertions
      static_assert(binary_digits<WordType>::value, "");

      // Friendship
      template <class> friend class bit_reference;
      friend class bit_pointer<WordType>;

      // Types
      public:
      using word_type = WordType;
      using index_type = bit_value::index_type;
      using size_type = bit_value::size_type;

      // Lifecycle
      public:
      template <class T
          , JASEL_REQUIRES( is_convertible<T&, WordType&>::value )
      >
      constexpr bit_reference(const bit_reference<T>& other) noexcept
      : _ptr(other._ptr)
      , _pos(other._pos)
      {
      }
      explicit constexpr bit_reference(word_type& ref) noexcept
      : _ptr(&ref)
      , _pos(0)
      {
      }
      constexpr bit_reference(word_type& ref, index_type pos)
      : _ptr((assert(pos < binary_digits<word_type>::value), &ref))
      , _pos(pos)
      {
      }


      // Assignment
      public:
      constexpr bit_reference& operator=(const bit_reference& other) noexcept
      {
          other ? set() : reset();
          return *this;
      }
      template <class T
          //, JASEL_REQUIRES( is_convertible<T&, WordType&>::value )
      >
      constexpr bit_reference& operator=(const bit_reference<T>& other) noexcept
      {
          other ? set() : reset();
          return *this;
      }
      constexpr bit_reference& operator=(bit_value val) noexcept
      {
          val ? set() : reset();
          return *this;
      }
      constexpr bit_reference& assign(word_type val) noexcept
      {
          val & 1 ? set() : reset();
          return *this;
      }
      constexpr bit_reference& assign(word_type val, index_type pos)
      {
          JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value);
          val >> pos & 1 ? set() : reset();
          return *this;
      }

      // Bitwise assignment operators
      public:
      constexpr bit_reference& operator&=(bit_value other) noexcept
      {
          *_ptr &= ~(mask() * static_cast<word_type>(!other._value));
          return *this;
      }
      constexpr bit_reference& operator|=(bit_value other) noexcept
      {
          *_ptr |= mask() * static_cast<word_type>(other._value);
          return *this;
      }
      constexpr bit_reference& operator^=(bit_value other) noexcept
      {
          *_ptr ^= mask() * static_cast<word_type>(other._value);
          return *this;
      }

      // Conversion
      public:
      explicit constexpr operator bool() const noexcept
      {
          return *_ptr & mask();
      }

      // Access
      public:
      constexpr bit_pointer<WordType> operator&() const noexcept
      {
          return bit_pointer<WordType>(_ptr, position());
      }

      // Swap members
      public:
      template <class T>
      void swap(bit_reference<T> other)
      {
          flip();
          other.flip();
      }
      void swap(bit_value& other)
      {
          flip();
          other.flip();
      }
      // Bit manipulation
      public:
      constexpr bit_reference& set(bool b) noexcept
      {
          b ? set() : reset();
          return *this;
      }
      constexpr bit_reference& set() noexcept
      {
          *_ptr |= mask();
          return *this;
      }
      constexpr bit_reference& reset() noexcept
      {
          *_ptr &= ~mask();
          return *this;
      }
      constexpr bit_reference& flip() noexcept
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
      constexpr index_type position() const noexcept
      {
          return _pos;
      }
      constexpr word_type mask() const noexcept
      {
          return static_cast<word_type>(1u) << _pos;
      }

      // Implementation details: function members
      private:
      bit_reference() noexcept = default;
      explicit constexpr bit_reference(nullptr_t) noexcept
      : _ptr(nullptr)
      , _pos(0)
      {
      }
      explicit constexpr bit_reference(word_type* ptr) noexcept
      : _ptr(ptr)
      , _pos(0)
      {
      }
      constexpr bit_reference(word_type* ptr, index_type pos)
      : _ptr((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), ptr))
      , _pos(pos)
      {
      }
      constexpr void assign_as_pointer(nullptr_t) noexcept
      {
        _ptr = nullptr;
        _pos = 0;
      }
      constexpr void assign_as_pointer(bit_reference rhs) noexcept
      {
        _ptr = rhs._ptr;
        _pos = rhs._pos;
      }
      template <class T>
      constexpr void assign_as_pointer(bit_reference<T> rhs) noexcept
      {
        _ptr = rhs._ptr;
        _pos = rhs._pos;
      }

      constexpr void inc_as_pointer()
      {
          constexpr index_type digits = binary_digits<word_type>::value;
          if (_pos + 1 < digits) {
            ++_pos;
          } else {
              ++_ptr;
              _pos = 0;
          }
      }
      constexpr void dec_as_pointer()
      {
          constexpr index_type digits = binary_digits<word_type>::value;
          if (_pos) {
              --_pos;
          } else {
              --_ptr;
              _pos = digits - 1;
          }
      }
      // Implementation details: data members
      private:
      word_type* _ptr;
      index_type _pos;
  };

  // Swap
  template <class T, class U>
  void swap(
      bit_reference<T> lhs,
      bit_reference<U> rhs
  ) noexcept
  {
      if (lhs != rhs) {
          lhs.flip();
          rhs.flip();
      }
  }
  template <class T>
  void swap(
      bit_reference<T> lhs,
      bit_value& rhs
  ) noexcept
  {
      if (lhs != rhs) {
          lhs.flip();
          rhs.flip();
      }
  }
  template <class U>
  void swap(
      bit_value& lhs,
      bit_reference<U> rhs
  ) noexcept
  {
      if (lhs != rhs) {
          lhs.flip();
          rhs.flip();
      }
  }

  // Stream functions
  template <class CharT, class Traits, class T>
  basic_istream<CharT, Traits>& operator>>(
      basic_istream<CharT, Traits>& is,
      bit_reference<T>& x
  );
  template <class CharT, class Traits, class T>
  basic_ostream<CharT, Traits>& operator<<(
      basic_ostream<CharT, Traits>& os,
      bit_reference<T> x
  )
  {
      constexpr char zero = '0';
      constexpr char one = '1';
      return os << os.widen(x ? one : zero);
  }

#else

  template <class WordType>
  class bit_reference
  {
      // Assertions
      static_assert(binary_digits<WordType>::value, "");

      // Friendship
      template <class> friend class bit_reference;
      friend class bit_pointer<WordType>;

      // Types
      public:
      using word_type = WordType;
      using index_type = bit_value::index_type;
      using size_type = bit_value::size_type;

      // Lifecycle
      public:
      constexpr bit_reference(const bit_reference& ) noexcept = default;
      template <class T
      , JASEL_REQUIRES( is_convertible<T&, WordType&>::value )
      >
      constexpr bit_reference(const bit_reference<T>& other) noexcept
      : _ptr(other._ptr)
      , _mask(other._mask)
      {
      }
      explicit constexpr bit_reference(word_type& ref) noexcept
      : _ptr(&ref)
      , _mask(1)
      {
      }
      constexpr bit_reference(word_type& ref, index_type pos)
      : _ptr((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), &ref))
      , _mask(static_cast<word_type>(1u << pos))
      {
      }

      // Assignment
      public:
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator=(const bit_reference& other) noexcept
      {
          other ? set() : reset();
          return *this;
      }
      template <class T
      //, JASEL_REQUIRES( is_convertible<T&, WordType&>::value )
      >
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator=(const bit_reference<T>& other) noexcept
      {
          other ? set() : reset();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator=(bit_value val) noexcept
      {
          val ? set() : reset();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& assign(word_type val) noexcept
      {
          val & 1 ? set() : reset();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& assign(word_type val, index_type pos)
      {
          JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value);
          val >> pos & 1 ? set() : reset();
          return *this;
      }

      // Bitwise assignment operators
      public:
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator&=(bit_value other) noexcept
      {
          *_ptr &= ~(_mask * static_cast<word_type>(!other._value));
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator|=(bit_value other) noexcept
      {
          *_ptr |= _mask * static_cast<word_type>(other._value);
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& operator^=(bit_value other) noexcept
      {
          *_ptr ^= _mask * static_cast<word_type>(other._value);
          return *this;
      }

      // Conversion
      public:
      explicit constexpr operator bool() const noexcept
      {
          return *_ptr & _mask;
      }

      // Access
      public:
      constexpr bit_pointer<WordType> operator&() const noexcept
      {
          return bit_pointer<WordType>(_ptr, position());
      }

      // Swap members
      public:
      template <class T>
      void swap(bit_reference<T> other)
      {
          if (other != *this) {
              flip();
              other.flip();
          }
      }
      void swap(bit_value& other)
      {
          if (other != *this) {
              flip();
              other.flip();
          }
      }
      // Bit manipulation
      public:
      JASEL_MUTABLE_CONSTEXPR bit_reference& set(bool b) noexcept
      {
          b ? set() : reset();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& set() noexcept
      {
          *_ptr |= _mask;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& reset() noexcept
      {
          *_ptr &= ~_mask;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_reference& flip() noexcept
      {
          *_ptr ^= _mask;
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
          return static_cast<size_type>(bit_ops::countr_zero(_mask));
      }
      constexpr word_type mask() const noexcept
      {
          return _mask;
      }

      // Implementation details: function members
      private:
      bit_reference() noexcept = default;
      explicit constexpr bit_reference(nullptr_t) noexcept
      : _ptr(nullptr)
      , _mask()
      {
      }
      explicit constexpr bit_reference(word_type* ptr) noexcept
      : _ptr(ptr)
      , _mask(1)
      {
      }
      constexpr bit_reference(word_type* ptr, index_type pos)
      : _ptr((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), ptr))
      , _mask(static_cast<word_type>(1) << pos)
      {
      }

      JASEL_MUTABLE_CONSTEXPR void assign_as_pointer(nullptr_t) noexcept
      {
        _ptr = nullptr;
        _mask = 0;
      }
      JASEL_MUTABLE_CONSTEXPR void assign_as_pointer(bit_reference rhs) noexcept
      {
        _ptr = rhs._ptr;
        _mask = rhs._mask;
      }
      template <class T>
      JASEL_MUTABLE_CONSTEXPR void assign_as_pointer(bit_reference<T> rhs) noexcept
      {
        _ptr = rhs._ptr;
        _mask = rhs._mask;
      }

      JASEL_MUTABLE_CONSTEXPR void inc_as_pointer()
      {
          using type = typename remove_cv<word_type>::type;
          constexpr size_type digits = binary_digits<word_type>::value;
          constexpr type one = 1;
          constexpr type mask = one;
          const size_type pos = position();
          if (pos + 1 < digits) {
              _mask <<= 1;
          } else {
              ++_ptr;
              _mask = mask;
          }
      }
      JASEL_MUTABLE_CONSTEXPR void dec_as_pointer()
      {
          using type = typename remove_cv<word_type>::type;
          constexpr size_type digits = binary_digits<word_type>::value;
          constexpr type one = 1;
          constexpr type mask = static_cast<type>(one << (digits - 1));
          const size_type pos = position();
          if (pos) {
              _mask >>= 1;
          } else {
              --_ptr;
              _mask = mask;
          }
      }
      // Implementation details: data members
      private:
      word_type* _ptr;
      typename remove_cv<word_type>::type _mask;
  };

  // Swap
  template <class T, class U>
  void swap(
      bit_reference<T> lhs,
      bit_reference<U> rhs
  ) noexcept
  {
      if (bool(lhs) != bool(rhs)) {
          lhs.flip();
          rhs.flip();
      }
  }
  template <class T>
  void swap(
      bit_reference<T> lhs,
      bit_value& rhs
  ) noexcept
  {
      if (lhs != rhs) {
          lhs.flip();
          rhs.flip();
      }
  }
  template <class U>
  void swap(
      bit_value& lhs,
      bit_reference<U> rhs
  ) noexcept
  {
      if (lhs != rhs) {
          lhs.flip();
          rhs.flip();
      }
  }

  // Stream functions
  template <class CharT, class Traits, class T>
  basic_istream<CharT, Traits>& operator>>(
      basic_istream<CharT, Traits>& is,
      bit_reference<T>& x
  );
  template <class CharT, class Traits, class T>
  basic_ostream<CharT, Traits>& operator<<(
      basic_ostream<CharT, Traits>& os,
      bit_reference<T> x
  )
  {
      constexpr char zero = '0';
      constexpr char one = '1';
      return os << os.widen(x ? one : zero);
  }
#endif
}}
} // std

#endif // header
