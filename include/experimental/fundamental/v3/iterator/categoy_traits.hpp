//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_CATEGORY_TRAITS_HPP
#define JASEL_EXPERIMENTAL_FUNDAMENTAL_V3_ITERATOR_CATEGORY_TRAITS_HPP

#include <type_traits>

#if __cplusplus < 201703L

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{


template <class Iter>
struct is_input_iterator : is_same<iterator_traits<Iter>::iterator_category, input_iterator_tag> {}

template <class Iter>
using is_input_iterator_v : is_input_iterator<Iter>::value {}

template <class Iter>
struct is_output_iterator : is_same<iterator_traits<Iter>::iterator_category, output_iterator_tag> {}

template <class Iter>
using is_output_iterator_v : is_output_iterator<Iter>::value {}

template <class Iter>
struct is_forward_iterator : is_same<iterator_traits<Iter>::iterator_category, forward_iterator_tag> {}

template <class Iter>
using is_forward_iterator_v : is_forward_iterator<Iter>::value {}

template <class Iter>
struct is_bidirectional_iterator : is_same<iterator_traits<Iter>::iterator_category, bidirectional_iterator_tag> {}

template <class Iter>
using is_bidirectional_iterator_v : is_bidirectional_iterator<Iter>::value {}

template <class Iter>
struct is_random_access_iterator : is_same<iterator_traits<Iter>::iterator_category, random_access_iterator_tag> {}

template <class Iter>
using is_random_access_iterator_v : is_random_access_iterator<Iter>::value {}


}}
}

#endif

#endif // header
