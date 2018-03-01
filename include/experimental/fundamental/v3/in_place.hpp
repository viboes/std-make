//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_V3_FUNDAMENTAL_IN_PLACE_HPP
#define JASEL_V3_FUNDAMENTAL_IN_PLACE_HPP

#include <experimental/meta/v1/id.hpp>
#include <experimental/meta/v1/eval.hpp>
#include <experimental/meta/v1/void_.hpp>
#include <type_traits>

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    // 20.5.6, In-place construction
    struct in_place_t {

      explicit in_place_t() = default;
    };
    //inline
    constexpr in_place_t in_place{};

    template <class T> struct in_place_type_t {
        explicit in_place_type_t() = default;
    };
#if __cplusplus >= 201402L
    template <class T>
    //inline
    constexpr in_place_type_t<T> in_place_type{};
#endif
    template <size_t I> struct in_place_index_t {
        explicit in_place_index_t() = default;
    };
#if __cplusplus >= 201402L
    template <size_t I>
    //inline
    constexpr in_place_index_t<I> in_place_index{};
#endif
  }
  }
  using std::experimental::in_place_t;
  using std::experimental::in_place;
  using std::experimental::in_place_type_t;
  using std::experimental::in_place_index_t;
#if __cplusplus >= 201402L
  using std::experimental::in_place_type;
  using std::experimental::in_place_index;
#endif

}
#endif // header
