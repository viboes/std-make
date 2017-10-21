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
 The header \c <experimental/.../bits/bit_value.hpp> defines a bit value with two possible values bit_off/bit_on.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf

 */

#ifndef JASEL_FUNDAMENTAL_V3_BITS_BIT_VALUE_HPP
#define JASEL_FUNDAMENTAL_V3_BITS_BIT_VALUE_HPP


#include <experimental/fundamental/v3/bits/binary_digits.hpp>

#include <experimental/contract.hpp>
#include <experimental/fundamental/v2/config.hpp>

#include <iosfwd>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  class bit_value
  {
      // Friendship
      template <class> friend class bit_reference;

      // Types
      public:
      using index_type = remove_cv<decltype(binary_digits<uintmax_t>::value)>::type;
      using size_type = index_type;

      // Lifecycle
      public:
      constexpr bit_value() noexcept
      : _value(1u) {}
      template <class T>
      constexpr bit_value(bit_reference<T> ref) noexcept
      : _value(static_cast<bool>(ref)) {}
      template <class WordType>
      explicit constexpr bit_value(WordType val) noexcept
      : _value(val & 1)
      {
        static_assert(binary_digits<WordType>::value, "");
      }
      template <class WordType>
      constexpr bit_value(WordType val, index_type pos)
      : _value((JASEL_ASSERT(pos < binary_digits<WordType>::value), (val >> pos) & 1))
      //: _value(val >> pos & 1)
      {
        static_assert(binary_digits<WordType>::value, "");
      }

      // Assignment
      public:
      template <class T>
      JASEL_MUTABLE_CONSTEXPR bit_value& operator=(bit_reference<T> ref) noexcept
      {
          _value = static_cast<bool>(ref);
          return *this;
      }
      template <class WordType>
      JASEL_MUTABLE_CONSTEXPR bit_value& assign(WordType val) noexcept
      {
        static_assert(binary_digits<WordType>::value, "");
        _value = val & 1;
        return *this;
      }

      template <class WordType>
      JASEL_MUTABLE_CONSTEXPR bit_value& assign(WordType val, index_type pos)
      {
        static_assert(binary_digits<WordType>::value, "");
        JASEL_ASSERT(pos < binary_digits<WordType>::value);
        _value = val >> pos & 1;
        return *this;
      }

      // Bitwise assignment operators
      public:
      JASEL_MUTABLE_CONSTEXPR bit_value& operator&=(bit_value other) noexcept
      {
          _value &= other._value;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR bit_value& operator|=(bit_value other) noexcept
      {
          _value |= other._value;
          return *this;
      }

      JASEL_MUTABLE_CONSTEXPR bit_value& operator^=(bit_value other) noexcept
      {
          _value ^= other._value;
          return *this;
      }

      // Conversion
      public:
      explicit constexpr operator bool() const noexcept
      {
          return _value;
      }

      // Swap members
      public:
      void swap(bit_value& other) noexcept
      {
          std::swap(*this, other);
      }
      template <class T>
      void swap(bit_reference<T> other) noexcept
      {
          flip();
          other.flip();
      }

      // Bit manipulation
      public:
      JASEL_MUTABLE_CONSTEXPR bit_value& set(bool b) noexcept
      {
          _value = b;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_value& set() noexcept
      {
          _value = true;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_value& reset() noexcept
      {
          _value = false;
          return *this;
      }
      JASEL_MUTABLE_CONSTEXPR bit_value& flip() noexcept
      {
          _value = !_value;
          return *this;
      }

      // Implementation details: data members
      private:
      using unsigned_word = unsigned;
      unsigned_word _value;

      // Bitwise operators
      public:
      friend constexpr bit_value operator~(
          bit_value rhs
      ) noexcept
      {
          using word = unsigned int;
          return bit_value(static_cast<word>(!rhs._value));
      }

      friend constexpr bit_value operator&(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return bit_value(lhs._value & rhs._value);
      }
      friend constexpr bit_value operator|(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return bit_value(lhs._value | rhs._value);
      }
      friend constexpr bit_value operator^(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return bit_value(lhs._value ^ rhs._value);
      }

      // Comparison operators
      public:
      friend constexpr bool operator==(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value == rhs._value;
      }
      friend constexpr bool operator!=(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value != rhs._value;
      }
      friend constexpr bool operator<(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value < rhs._value;
      }
      friend constexpr bool operator<=(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value <= rhs._value;
      }
      friend constexpr bool operator>(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value > rhs._value;
      }
      friend constexpr bool operator>=(
          bit_value lhs,
          bit_value rhs
      ) noexcept
      {
          return lhs._value >= rhs._value;
      }
  };

  // Stream functions
  template <class CharT, class Traits>
  basic_istream<CharT, Traits>& operator>>(
      basic_istream<CharT, Traits>& is,
      bit_value& x
  )
  {
      using stream_type = basic_istream<CharT, Traits>;
      using traits_type = typename stream_type::traits_type;
      using ios_base = typename stream_type::ios_base;
      constexpr char zero = '0';
      constexpr char one = '1';
      constexpr typename stream_type::int_type eof = traits_type::eof();
      typename ios_base::iostate state = ios_base::goodbit;
      typename stream_type::char_type char_value = 0;
      typename stream_type::int_type int_value = 0;
      typename stream_type::sentry sentry(is);
      bool ok = false;
      bit_value tmp = x;
      if (sentry) {
          try {
              int_value = is.rdbuf()->sbumpc();
              if (traits_type::eq_int_type(int_value, eof)) {
                  state |= ios_base::eofbit;
              } else {
                  char_value = traits_type::to_char_type(int_value);
                  if (traits_type::eq(char_value, is.widen(zero))) {
                      tmp.reset();
                      ok = true;
                  } else if (traits_type::eq(char_value, is.widen(one))) {
                      tmp.set();
                      ok = true;
                  } else {
                      int_value = is.rdbuf()->sputbackc(char_value);
                      if (traits_type::eq_int_type(int_value, eof)) {
                          state |= ios_base::failbit;
                      }
                  }
              }
          } catch(...) {
              is.setstate(ios_base::badbit);
          }
      }
      if (ok) {
          x = tmp;
      } else {
          state |= ios_base::failbit;
      }
      state ? is.setstate(state) : void();
      return is;
  }

  template <class CharT, class Traits>
  basic_ostream<CharT, Traits>& operator<<(
      basic_ostream<CharT, Traits>& os,
      bit_value x
  )
  {
      constexpr char zero = '0';
      constexpr char one = '1';
      return os << os.widen(x ? one : zero);
  }

  // Constant bit values
  // fixme: these should be inline variables
  constexpr bit_value bit_off(0u);
  constexpr bit_value bit_on(1u);

}}
} // std

#endif // header
