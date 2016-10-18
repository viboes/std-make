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
  template <std::size_t FieldIndex, std::size_t FieldsCount>
  struct stream_insert_impl {

      template <class Stream, class T>
      static void apply (Stream& out, const T& value) {
          if (!!FieldIndex) out << ", ";
          out << product_type::get<FieldIndex>(value);           // std::get<FieldIndex>(value)
          stream_insert_impl<FieldIndex + 1, FieldsCount>::apply(out, value);
      }
  };
  template <std::size_t FieldsCount>
  struct stream_insert_impl<FieldsCount,FieldsCount> {
    template <class Stream, class T>
    static void apply (Stream& out, const T& value) {}

  };
}
  template <class Char, class Traits, class ProductType
  , class = enable_if_t<
      is_product_type_v<remove_cv_t<remove_reference_t<ProductType>>>
    >
  >
  constexpr void stream_insert(basic_ostream<Char, Traits>& os, ProductType const& pt)
  {
    os << '{';
    detail::stream_insert_impl<0, product_type::size<ProductType>::value >::apply(os, pt);
    os << '}';
  }

  namespace operators {
    template <class Char, class Traits, class T>
    std::basic_ostream<Char, Traits>&
        operator<<(std::basic_ostream<Char, Traits>& os, const T& value)
    {
        stream_insert(os, value);
        return os;
    }
  }
}
}}
}

#endif // header
