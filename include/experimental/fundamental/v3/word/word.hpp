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
 The header \c <experimental/.../subword/subword.hpp> defines forward declarations to the subword classes.
 */

#ifndef JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_HPP
#define JASEL_FUNDAMENTAL_V3_SUBWORD_SUBWORD_HPP

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
  //template <int Words, class WordType=single_uword>
  //class multi_word;

  template <int Bits, class WordType=single_uword>
  class subword_value;
  template <int Bits, class WordType=single_uword, class T=remove_cv_t<WordType>> class subword_reference;
  template <int Bits, class WordType=single_uword, class T=WordType> class subword_pointer;
  template <class Iterator, int Bits, class T=typename Iterator::value_type> class subword_iterator;

}
}
} // std

#endif // header
