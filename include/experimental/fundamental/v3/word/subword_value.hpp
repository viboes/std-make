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
 The header \c <experimental/.../bits/subword_value.hpp> defines a subword value with 2^N possible values.

 */

#ifndef JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_VALUE_HPP
#define JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_VALUE_HPP


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
  template <int Bits, class WT>
  class subword_value
  {

      // Friendship
      template <int B, class W, class T>
      friend class subword_reference;

      // Types
  public:
      using index_type = size_t;
      using size_type = size_t;
      using word_type = WT;
      static constexpr int word_digits = binary_digits<word_type>::value;
      static constexpr int subword_digits = Bits;
      static_assert(subword_digits <= word_digits, "");

      static constexpr word_type mask() { return bit_ops::up_to<subword_digits, word_type>(); }

      // Lifecycle
  public:
      constexpr subword_value() noexcept = default;

      template <class WordType>
      explicit constexpr subword_value(WordType val) noexcept
      : _value(val & mask())
      {
        static_assert(subword_digits <= binary_digits<WordType>::value, "");
        static_assert(binary_digits<WordType>::value, "");
      }
      template <class WordType>
      constexpr subword_value(WordType const& val, index_type pos)
      : _value((JASEL_CONSTEXPR_ASSERT(pos*subword_digits < binary_digits<WordType>::value), static_cast<word_type>(bit_ops::from_up_to<subword_digits>(val, pos*subword_digits))))
      {
        static_assert(binary_digits<WordType>::value, "");
      }

      // Observers
      public:
      word_type value() const noexcept
      {
          return _value;
      }

      // Assignment
      public:
      template <class W, class T>
      JASEL_MUTABLE_CONSTEXPR subword_value& operator=(subword_reference<Bits, W, T> const& ref) noexcept
      {
          *this = static_cast<subword_value>(ref);
          return *this;
      }
      template <class WordType>
      JASEL_MUTABLE_CONSTEXPR subword_value& assign(WordType const& val) noexcept
      {
        static_assert(binary_digits<WordType>::value, "");
        _value = val & mask();
        return *this;
      }

      template <class WordType>
      JASEL_MUTABLE_CONSTEXPR subword_value& assign(WordType const& val, index_type pos)
      {
        static_assert(binary_digits<WordType>::value, "");
        JASEL_CONSTEXPR_ASSERT(pos < binary_digits<WordType>::value);
        _value = val >> pos & mask();
        return *this;
      }

      // Bitwise assignment operators
      public:
      JASEL_MUTABLE_CONSTEXPR subword_value& operator&=(subword_value const& other) noexcept
      {
          _value &= other._value;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR subword_value& operator|=(subword_value const& other) noexcept
      {
          _value |= other._value;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR subword_value& operator^=(subword_value const& other) noexcept
      {
          _value ^= other._value;
          return *this;
      }

      // Conversion
      public:

      // Swap members
      public:
      void swap(subword_value& other) noexcept
      {
        std::swap(*this, other);
      }
      template <class W, class T>
      void swap(subword_reference<Bits, W, T> other) noexcept
      {
        subword_value tmp{other};
        other = *this;
        *this = tmp;
      }

      // Bits manipulation
      public:

      // Implementation details: data members
      private:
      word_type _value;

      // Bitwise operators
      public:
      friend constexpr subword_value operator~(
          subword_value rhs
      ) noexcept
      {
          using word = unsigned int;
          return subword_value(static_cast<word>(~rhs._value));
      }

      friend constexpr subword_value operator&(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return subword_value(lhs._value & rhs._value);
      }
      friend constexpr subword_value operator|(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return subword_value(lhs._value | rhs._value);
      }
      friend constexpr subword_value operator^(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return subword_value(lhs._value ^ rhs._value);
      }

      // Comparison operators
      public:
      friend constexpr bool operator==(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value == rhs._value;
      }
      friend constexpr bool operator!=(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value != rhs._value;
      }
      friend constexpr bool operator<(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value < rhs._value;
      }
      friend constexpr bool operator<=(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value <= rhs._value;
      }
      friend constexpr bool operator>(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value > rhs._value;
      }
      friend constexpr bool operator>=(
          subword_value lhs,
          subword_value rhs
      ) noexcept
      {
          return lhs._value >= rhs._value;
      }

      // Stream functions
      template <class CharT, class Traits>
      friend basic_istream<CharT, Traits>& operator>>(
          basic_istream<CharT, Traits>& is,
          subword_value& x
      )
      {
          return is >> x.value;
          x.value &= mask();
          return is;
      }

      template <class CharT, class Traits>
      friend basic_ostream<CharT, Traits>& operator<<(
          basic_ostream<CharT, Traits>& os,
          subword_value const& x
      )
      {
          return os << x.value();
      }
  };





  // Constant subwords values

}}
} // std

#endif // header
