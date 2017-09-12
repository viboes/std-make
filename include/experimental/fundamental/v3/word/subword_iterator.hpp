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
 The header \c <experimental/.../bits/subword_value.hpp> defines a bit value with two possible values subword_off/subword_on.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf

 */

#ifndef JASEL_FUNDAMENTAL_V3_WORD_SUBWORD_ITERATOR_HPP
#define JASEL_FUNDAMENTAL_V3_WORD_SUBWORD_ITERATOR_HPP


#include <experimental/fundamental/v3/bits/binary_digits.hpp>
#include <experimental/fundamental/v3/bits/bit_ops.hpp>
#include <experimental/fundamental/v3/word/word.hpp>

#include <experimental/fundamental/v3/contract/constexpr_assert.hpp>
#include <experimental/fundamental/v2/config.hpp>

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  template <class Iterator, int Bits, class T>
  class subword_iterator
  {
      // Assertions
      private:
      using _traits_t = iterator_traits<Iterator>;

      // Types
      public:
      using iterator_type = Iterator;
      using word_type = typename _traits_t::value_type;
      using subword_type = T;

      static_assert(binary_digits<word_type>::value, "");
      using iterator_category = typename _traits_t::iterator_category;
      using value_type = subword_value<Bits, T>;
      using index_type = typename value_type::index_type;
      using size_type = typename value_type::size_type;
      using pointer = subword_pointer<Bits, word_type, T>;
      using difference_type = typename pointer::difference_type;
      using reference = subword_reference<Bits, word_type, T>;


      // Lifecycle
      public:
      constexpr subword_iterator() noexcept
      : _current()
      , _position()
      {
      }
      constexpr subword_iterator(const subword_iterator& other) = default;
      template <class It>
      constexpr subword_iterator(const subword_iterator<It, Bits, T>& other)
      : _current(other._current)
      , _position(other._position)
      {
      }
      explicit constexpr subword_iterator(iterator_type i)
      : _current(i)
      , _position(0)
      {
      }
      constexpr subword_iterator(iterator_type i, size_type pos)
      : _current(i)
      , _position((JASEL_CONSTEXPR_ASSERT(pos < binary_digits<word_type>::value), pos))
      {
      }

      // Assignment
      public:
      template <class It>
      JASEL_MUTABLE_CONSTEXPR subword_iterator& operator=(const subword_iterator<It, Bits, T>& other)
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
        constexpr difference_type word_digits = binary_digits<word_type>::value;
        constexpr difference_type subword_digits = binary_digits<subword_type>::value;
        const difference_type sum = subword_digits * (_position + n);
        difference_type diff = sum / word_digits;
        if (sum < 0 && diff * word_digits != sum) {
            --diff;
        }
        return reference(std::next(_current, diff), sum - diff * word_digits);
      }

      // Increment and decrement operators
      public:
      JASEL_MUTABLE_CONSTEXPR subword_iterator& operator++()
      {
        constexpr size_type word_digits = binary_digits<word_type>::value;
        constexpr size_type subword_digits = binary_digits<word_type>::value;
          if (_position + subword_digits < word_digits) {
              ++_position;
          } else {
              ++_current;
              _position = 0;
          }
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_iterator& operator--()
      {
          constexpr size_type word_digits = binary_digits<word_type>::value;
          if (_position) {
              --_position;
          } else {
              --_current;
              _position = word_digits - 1;
          }
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_iterator operator++(int)
      {
          subword_iterator old = *this;
          ++(*this);
          return old;
      }
      JASEL_MUTABLE_CONSTEXPR subword_iterator operator--(int)
      {
          subword_iterator old = *this;
          --(*this);
          return old;
      }
      JASEL_CXX14_CONSTEXPR subword_iterator operator+(difference_type n) const
      {
          constexpr difference_type digits = binary_digits<word_type>::value;
          const difference_type sum = _position + n;
          difference_type diff = sum / digits;
          if (sum < 0 && diff * digits != sum) {
              --diff;
          }
          return subword_iterator(std::next(_current, diff), sum - diff * digits);
      }
      JASEL_CXX14_CONSTEXPR subword_iterator operator-(difference_type n) const
      {
          constexpr difference_type digits = binary_digits<word_type>::value;
          const difference_type sum = _position - n;
          difference_type diff = sum / digits;
          if (sum < 0 && diff * digits != sum) {
              --diff;
          }
          return subword_iterator(std::next(_current, diff), sum - diff * digits);
      }
      JASEL_MUTABLE_CONSTEXPR subword_iterator& operator+=(difference_type n)
      {
          *this = *this + n;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR subword_iterator& operator-=(difference_type n)
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
      template <class It>
      friend constexpr subword_iterator<It, Bits, T> operator+(
          typename subword_iterator<It, Bits, T>::difference_type n,
          const subword_iterator<It, Bits, T>& i
      )
      {
          return i + n;
      }
      template <class It1, class It2>
      friend JASEL_CXX14_CONSTEXPR typename std::common_type<
          typename subword_iterator<It1, Bits, T>::difference_type,
          typename subword_iterator<It2, Bits, T>::difference_type
      >::type operator-(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          using lhs_utype = typename subword_iterator<It1, Bits, T>::word_type;
          using rhs_utype = typename subword_iterator<It2, Bits, T>::word_type;
          using lhs_type = typename subword_iterator<It1, Bits, T>::difference_type;
          using rhs_type = typename subword_iterator<It2, Bits, T>::difference_type;
          using difference_type = typename std::common_type<lhs_type, rhs_type>::type;
          constexpr difference_type lhs_digits = binary_digits<lhs_utype>::value;
          constexpr difference_type rhs_digits = binary_digits<rhs_utype>::value;
          constexpr difference_type digits = rhs_digits;
          static_assert(lhs_digits == rhs_digits, "");
          const difference_type main = lhs._current - rhs._current;
          return main * digits + (lhs._position - rhs._position);
      }

      // Comparison operators
      public:
      template <class It1, class It2>
      friend constexpr bool operator==(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current == rhs._current && lhs._position == rhs._position;
      }
      template <class It1, class It2>
      friend constexpr bool operator!=(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current != rhs._current || lhs._position != rhs._position;
      }
      template <class It1, class It2>
      friend constexpr bool operator<(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current < rhs._current
              || (lhs._current == rhs._current && lhs._position < rhs._position);
      }
      template <class It1, class It2>
      friend constexpr bool operator<=(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current < rhs._current
              || (lhs._current == rhs._current && lhs._position <= rhs._position);
      }
      template <class It1, class It2>
      friend constexpr bool operator>(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current > rhs._current
              || (lhs._current == rhs._current && lhs._position > rhs._position);
      }
      template <class It1, class It2>
      friend constexpr bool operator>=(
          const subword_iterator<It1, Bits, T>& lhs,
          const subword_iterator<It2, Bits, T>& rhs
      )
      {
          return lhs._current > rhs._current
              || (lhs._current == rhs._current && lhs._position >= rhs._position);
      }
  };



}}
} // std

#endif // header
