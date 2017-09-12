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
 The header \c <experimental/.../bits/bit_value.hpp> defines a bit reference.
 Most of them are based on "Wording for fundamental bit manipulation utilities" http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0237r8.pdf
 */

#ifndef JASEL_FUNDAMENTAL_V3_WORD_BINARY_DIGGITS_HPP
#define JASEL_FUNDAMENTAL_V3_WORD_BINARY_DIGGITS_HPP

#include <limits>
#include <type_traits>

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/bits/binary_digits.hpp>

#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  // forward declarations
  using single_uword = unsigned int;
  using word = single_uword;
  template <int Words, class WordType=single_uword>
  class multiword;

  template <int Bits, class WordType=single_uword>
  class subword_value;
  template <int Bits, class WordType=single_uword, class T=remove_cv_t<WordType>> class subword_reference;
  template <int Bits, class WordType=single_uword, class T=WordType> class subword_pointer;
  template <class Iterator, int Bits, class T=typename Iterator::value_type> class subword_iterator;

//  template <class UIntType>
//  struct is_word_type : false_type {};
//
//  template <class UIntType>
//  struct is_word_type<const UIntType> : is_word_type<UIntType> {};
//  template <class UIntType>
//  struct is_word_type<volatile UIntType> : is_word_type<UIntType> {};
//  template <class UIntType>
//  struct is_word_type<const volatile UIntType> : is_word_type<UIntType> {};
//
//#if __cplusplus >= 201402L
//  template <class T>
//  constexpr bool is_word_type_v = is_word_type<T>::value;
//#endif
//
//  template <>
//  struct is_word_type<unsigned char> : true_type {};
//  template <>
//  struct is_word_type<unsigned short> : true_type {};
//  template <>
//  struct is_word_type<unsigned int> : true_type {};
//  template <>
//  struct is_word_type<unsigned long> : true_type {};
//  template <>
//  struct is_word_type<unsigned long long> : true_type {};
//  template <int N, class T>
//  struct is_word_type<multiword<N,W>> : is_word_type<W> {};
//
//  static_assert(! is_word_type<bool>::value, "");
//  static_assert(! is_word_type<char>::value, "");
//  static_assert(! is_word_type<signed char>::value, "");
//  static_assert(! is_word_type<signed short>::value, "");
//  static_assert(! is_word_type<signed int>::value, "");
//  static_assert(! is_word_type<signed long>::value, "");
//  static_assert(! is_word_type<signed long long>::value, "");
//
//  template <class UIntType>
//  struct binary_digits
//  : integral_constant<std::size_t, numeric_limits<UIntType>::digits>
//  {
//      // Assertions
//      static_assert(is_word_type<UIntType>::value, "");
//  };
//  template <class Words, class WordType>
//  struct binary_digits<multiword<Words,WordType>>
//  : integral_constant<std::size_t, Words*binary_digits<WordType>::digits>
//  {
//      // Assertions
//      static_assert(is_word_type<UIntType>::value, "");
//  };
//
//  // Binary digits value
//#if __cplusplus >= 201402L
//  template <class T>
//  constexpr std::size_t binary_digits_v = binary_digits<T>::value;
//#endif

}
}
} // std

#endif // header
