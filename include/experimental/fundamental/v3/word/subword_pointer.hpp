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

#ifndef JASEL_FUNDAMENTAL_V3_WORD_SUBWORD_POINTER_HPP
#define JASEL_FUNDAMENTAL_V3_WORD_SUBWORD_POINTER_HPP


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
  template <int Bits, class WordType, class T>
  class subword_pointer
  {
      // Assertions
      static_assert(binary_digits<WordType>::value, "");

      // Friendship
      template <int, class, class> friend class subword_pointer;

      // Types
      public:
      using word_type = WordType;
      using subword_type = T;
      static constexpr int subword_digits = Bits;
      static constexpr int word_digits = binary_digits<word_type>::value;
      using subword = subword_value<subword_digits, T>;

      using index_type = typename subword::index_type;
      using size_type = typename subword::size_type;
      using difference_type = std::ptrdiff_t;

      static_assert(is_trivially_copyable<subword_type>::value, "A subword must be trivially copyable");
      static_assert(subword_digits <= binary_digits<subword_type>::value, "");
      static_assert(word_digits % subword_digits == 0, "");
      //static_assert(alignof(T) <= alignof(WordType), ""); // it is not clear yet if this will be needed


      // Lifecycle
      public:
      constexpr subword_pointer() noexcept
      : _ref(nullptr)
      {
      }

      constexpr subword_pointer(const subword_pointer& other) noexcept
      : _ref(other._ref)
      {
      }
      template <class W>
      constexpr subword_pointer(const subword_pointer<Bits,W,T>& other) noexcept
      : _ref(other._ref)
      {
      }
      constexpr subword_pointer(std::nullptr_t) noexcept
      : _ref(nullptr)
      {
      }
      explicit constexpr subword_pointer(word_type* ptr) noexcept
      : _ref(ptr)
      {
      }
      constexpr subword_pointer(word_type* ptr, size_type pos)
      : _ref(ptr, pos)
      {
      }

      // Assignment
      public:
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator=(std::nullptr_t) noexcept
      {
          _ref.assign_as_pointer(nullptr);
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator=(const subword_pointer& other) noexcept
      {
          _ref.assign_as_pointer(other._ref);
          return *this;
      }
      template <class W>
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator=(const subword_pointer<Bits, W, T>& other) noexcept
      {
          _ref.assign_as_pointer(other._ref);
          return *this;
      }

      // Conversion
      public:
      explicit constexpr operator bool() const noexcept
      {
          return _ref._ptr;
      }

      // Access
      public:
      constexpr subword_reference<Bits, WordType, T> operator*() const noexcept
      {
          return _ref;
      }
      constexpr subword_reference<Bits, WordType, T>* operator->() const noexcept
      {
          return std::addressof(const_cast<subword_reference<Bits, WordType, T>&>(_ref));
      }
      JASEL_CXX14_CONSTEXPR subword_reference<Bits, WordType, T> operator[](difference_type n) const
      {
          //constexpr difference_type word_digits = binary_digits<word_type>::value;
        //constexpr difference_type subword_digits = binary_digits<subword_type>::value;
          const difference_type sum = subword_digits * (_ref.position() + n);
          difference_type diff = sum / word_digits;
          if (sum < 0 && diff * word_digits != sum) {
              --diff;
          }
          return subword_reference<Bits, WordType, T>(_ref._ptr + diff, sum - diff * word_digits);
      }

      // Increment and decrement operators
      public:
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator++()
      {
          _ref.inc_as_pointer();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator--()
      {
          _ref.dec_as_pointer();
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer operator++(int)
      {
          subword_pointer old = *this;
          ++(*this);
          return old;
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer operator--(int)
      {
          subword_pointer old = *this;
          --(*this);
          return old;
      }
      JASEL_CXX14_CONSTEXPR subword_pointer operator+(difference_type n) const
      {
        //constexpr difference_type word_digits = binary_digits<word_type>::value;
        //constexpr difference_type subword_digits = binary_digits<subword_type>::value;
        const difference_type sum = subword_digits * (static_cast<difference_type>(_ref.position()) + n);
        difference_type diff = sum / word_digits;
        if (sum < 0 && diff * word_digits != sum) {
              --diff;
          }
          return subword_pointer(_ref._ptr + diff, static_cast<size_type>(sum - diff * static_cast<difference_type>(word_digits)));
      }
      JASEL_CXX14_CONSTEXPR subword_pointer operator-(difference_type n) const
      {
        //constexpr difference_type word_digits = binary_digits<word_type>::value;
        //constexpr difference_type subword_digits = binary_digits<subword_type>::value;
        const difference_type sum = subword_digits * (_ref.position() - n);
        difference_type diff = sum / word_digits;
        if (sum < 0 && diff * word_digits != sum) {
              --diff;
          }
          return subword_pointer(_ref._ptr + diff, sum - diff * word_digits);
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator+=(difference_type n)
      {
          *this = *this + n;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR subword_pointer& operator-=(difference_type n)
      {
          *this = *this - n;
          return *this;
      }

      // Implementation details: data members
      private:
      subword_reference<Bits, WordType, T> _ref;

      // Non-member arithmetic operators
      template <class W>
      friend constexpr subword_pointer<Bits,W,T> operator+(
          typename subword_pointer<Bits,W,T>::difference_type n,
          subword_pointer<Bits,W,T> x
      )
      {
          return x + n;
      }
      template <int B, class W1, class W2, class U>
      friend JASEL_CXX14_CONSTEXPR typename std::common_type<
          typename subword_pointer<B, W1, U>::difference_type,
          typename subword_pointer<B, W2, U>::difference_type
      >::type operator-(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      )
      {
          using lhs_type = typename subword_pointer<B, W1, U>::difference_type;
          using rhs_type = typename subword_pointer<B, W2, U>::difference_type;
          using difference_type = typename std::common_type<lhs_type, rhs_type>::type;
          constexpr difference_type lhs_digits = binary_digits<W1>::value;
          constexpr difference_type rhs_digits = binary_digits<W2>::value;
          constexpr difference_type tword_digits = rhs_digits;
          //constexpr difference_type subword_digits = binary_digits<T>::value;
          static_assert(lhs_digits == rhs_digits, "");
          const difference_type main = lhs._ref.address() - rhs._ref.address();
          return main * (tword_digits/subword_digits) + (lhs._ref.position() - rhs._ref.position());
      }

      // Comparison operators
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator==(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() == rhs._ref.address()
              && lhs._ref.position() == rhs._ref.position();
      }
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator!=(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() != rhs._ref.address()
              || lhs._ref.position() != rhs._ref.position();
      }
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator<(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() < rhs._ref.address()
              || (lhs._ref.address() == rhs._ref.address()
                  && lhs._ref.position() < rhs._ref.position());
      }
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator<=(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() < rhs._ref.address()
              || (lhs._ref.address() == rhs._ref.address()
                  && lhs._ref.position() <= rhs._ref.position());
      }
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator>(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() > rhs._ref.address()
              || (lhs._ref.address() == rhs._ref.address()
                  && lhs._ref.position() > rhs._ref.position());
      }
      template <int B, class W1, class W2, class U>
      friend constexpr bool operator>=(
          subword_pointer<B, W1, U> lhs,
          subword_pointer<B, W2, U> rhs
      ) noexcept
      {
          return lhs._ref.address() > rhs._ref.address()
              || (lhs._ref.address() == rhs._ref.address()
                  && lhs._ref.position() >= rhs._ref.position());
      }
  };


}}
} // std

#endif // header
