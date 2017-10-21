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
 The header \c <experimental/.../bits/bit_iterator.hpp> defines a bit iterator to iterate over the bits located on an associated iterator.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf

 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BIT_ITERATOR_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BIT_ITERATOR_HPP


#include <experimental/fundamental/v3/bits/binary_digits.hpp>

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class Iterator>
  class bit_iterator
  {
      // Assertions
      private:
      using _traits_t = iterator_traits<Iterator>;

      // Types
      public:
      using iterator_type = Iterator;
      using word_type = typename _traits_t::value_type;
      static_assert(binary_digits<word_type>::value, "");
      using iterator_category = typename _traits_t::iterator_category;
      using value_type = bit_value;
      using difference_type = typename bit_pointer<word_type>::difference_type;
      using pointer = bit_pointer<word_type>;
      using reference = bit_reference<word_type>;
      using size_type = bit_value::size_type;
      using index_type = bit_value::index_type;

      // Lifecycle
      public:
      constexpr bit_iterator()
      : _current()
      , _position()
      {
      }
      template <class T
      , JASEL_REQUIRES( is_constructible<iterator_type, T>::value )
      >
      constexpr bit_iterator(const bit_iterator<T>& other)
      : _current(other._current)
      , _position(other._position)
      {
      }
      explicit constexpr bit_iterator(iterator_type i)
      : _current(i)
      , _position(0)
      {
      }
      constexpr bit_iterator(iterator_type i, size_type pos)
      : _current(i)
      , _position((JASEL_ASSERT(pos < binary_digits<word_type>::value), pos))
      {
      }

      // Assignment
      public:
      template <class T
      , JASEL_REQUIRES( is_assignable<iterator_type, T>::value )
      >
      JASEL_MUTABLE_CONSTEXPR bit_iterator& operator=(const bit_iterator<T>& other)
      {
          _current = other._current;
          _position = other._position;
          return *this;
      }

      // Access
      public:
      constexpr reference operator*() const noexcept
      {
          return reference(*_current, _position);
      }
      constexpr pointer operator->() const noexcept
      {
          return pointer(&*_current, _position);
      }
      JASEL_CXX14_CONSTEXPR reference operator[](difference_type n) const
      {
          constexpr difference_type digits = binary_digits<word_type>::value;
          const difference_type sum = _position + n;
          difference_type diff = sum / digits;
          if (sum < 0 && diff * digits != sum) {
              --diff;
          }
          return reference(*next(_current, diff), sum - diff * digits);
      }

      // Increment and decrement operators
      public:
      JASEL_MUTABLE_CONSTEXPR bit_iterator& operator++()
      {
          constexpr size_type digits = binary_digits<word_type>::value;
          if (_position + 1 < digits) {
              ++_position;
          } else {
              ++_current;
              _position = 0;
          }
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_iterator& operator--()
      {
          constexpr size_type digits = binary_digits<word_type>::value;
          if (_position) {
              --_position;
          } else {
              --_current;
              _position = digits - 1;
          }
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_iterator operator++(int)
      {
          bit_iterator old = *this;
          ++(*this);
          return old;
      }
      JASEL_MUTABLE_CONSTEXPR bit_iterator operator--(int)
      {
          bit_iterator old = *this;
          --(*this);
          return old;
      }
      JASEL_CXX14_CONSTEXPR bit_iterator operator+(difference_type n) const
      {
          constexpr difference_type digits = binary_digits<word_type>::value;
          const difference_type sum = _position + n;
          difference_type diff = sum / digits;
          if (sum < 0 && diff * digits != sum) {
              --diff;
          }
          return bit_iterator(next(_current, diff), sum - diff * digits);
      }
      JASEL_CXX14_CONSTEXPR bit_iterator operator-(difference_type n) const
      {
          constexpr difference_type digits = binary_digits<word_type>::value;
          const difference_type sum = _position - n;
          difference_type diff = sum / digits;
          if (sum < 0 && diff * digits != sum) {
              --diff;
          }
          return bit_iterator(next(_current, diff), sum - diff * digits);
      }
      JASEL_MUTABLE_CONSTEXPR bit_iterator& operator+=(difference_type n)
      {
          *this = *this + n;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR bit_iterator& operator-=(difference_type n)
      {
          *this = *this - n;
          return *this;
      }

      // Underlying details
      public:
      constexpr iterator_type base() const
      {
          return _current;
      }
      constexpr size_type position() const noexcept
      {
          return _position;
      }
      constexpr word_type mask() const noexcept
      {
          return static_cast<word_type>(1) << _position;
      }

      // Implementation details: data members
      private:
      iterator_type _current;
      size_type _position;

      // Non-member arithmetic operators
      template <class T>
      friend constexpr bit_iterator<T> operator+(
          typename bit_iterator<T>::difference_type n,
          const bit_iterator<T>& i
      )
      {
          return i + n;
      }
      template <class T, class U>
      friend JASEL_CXX14_CONSTEXPR typename common_type<
          typename bit_iterator<T>::difference_type,
          typename bit_iterator<U>::difference_type
      >::type operator-(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          using lhs_utype = typename bit_iterator<T>::word_type;
          using rhs_utype = typename bit_iterator<U>::word_type;
          using lhs_type = typename bit_iterator<T>::difference_type;
          using rhs_type = typename bit_iterator<U>::difference_type;
          using difference_type = typename common_type<lhs_type, rhs_type>::type;
          constexpr difference_type lhs_digits = binary_digits<lhs_utype>::value;
          constexpr difference_type rhs_digits = binary_digits<rhs_utype>::value;
          constexpr difference_type digits = rhs_digits;
          static_assert(lhs_digits == rhs_digits, "");
          const difference_type main = lhs._current - rhs._current;
          return main * digits + (lhs._position - rhs._position);
      }

      // Comparison operators
      public:
      template <class T, class U>
      friend constexpr bool operator==(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current == rhs._current && lhs._position == rhs._position;
      }
      template <class T, class U>
      friend constexpr bool operator!=(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current != rhs._current || lhs._position != rhs._position;
      }
      template <class T, class U>
      friend constexpr bool operator<(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current < rhs._current
              || (lhs._current == rhs._current && lhs._position < rhs._position);
      }
      template <class T, class U>
      friend constexpr bool operator<=(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current < rhs._current
              || (lhs._current == rhs._current && lhs._position <= rhs._position);
      }
      template <class T, class U>
      friend constexpr bool operator>(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current > rhs._current
              || (lhs._current == rhs._current && lhs._position > rhs._position);
      }
      template <class T, class U>
      friend constexpr bool operator>=(
          const bit_iterator<T>& lhs,
          const bit_iterator<U>& rhs
      )
      {
          return lhs._current > rhs._current
              || (lhs._current == rhs._current && lhs._position >= rhs._position);
      }
  };

}}
} // std

#endif // header
