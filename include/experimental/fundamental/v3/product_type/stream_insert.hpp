//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_STREAM_INSERT_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_STREAM_INSERT_HPP

#include <experimental/fundamental/v3/product_type/product_type.hpp>
#include <utility>
#include <iostream>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace product_type
{
namespace detail {
  template <std::size_t I, std::size_t N>
  struct stream_insert_impl {

      template <class Stream, class PT>
      static void apply (Stream& os, const PT& value) {
          if (!!I) os << ", ";
          os << product_type::get<I>(value);
          stream_insert_impl<I + 1, N>::apply(os, value);
      }
  };
  template <std::size_t N>
  struct stream_insert_impl<N,N> {
    template <class Stream, class PT>
    static void apply (Stream& , const PT& ) {}

  };
}
  template <class Char, class Traits, class PT
  , class = enable_if_t<
      is_product_type_v<remove_cv_t<remove_reference_t<PT>>>
    >
  >
  constexpr void stream_insert(basic_ostream<Char, Traits>& os, PT const& pt)
  {
    os << '{';
    detail::stream_insert_impl<0, product_type::size<PT>::value >::apply(os, pt);
    os << '}';
  }

  namespace operators {
    template <class Char, class Traits, class PT>
    std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const PT& value)
    {
        stream_insert(os, value);
        return os;
    }
  }
}
}}
}

#endif // header
