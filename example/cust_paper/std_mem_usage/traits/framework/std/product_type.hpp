//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_MEM_USAGE_HPP
#define JASEL_FUNDAMENTAL_V3_PRODUCT_TYPE_MEM_USAGE_HPP
#if __cplusplus > 201402L

#include "../mem_usage.hpp"
#include <experimental/fundamental/v3/product_type/product_type.hpp>

namespace std
{
namespace experimental
{
inline  namespace fundamental_v3
{
namespace product_type
{

    namespace detail {
      template <class PT, std::size_t... I>
      constexpr decltype(auto) mem_usage_impl( PT&& pt, index_sequence<I...> ) noexcept
      {
        return  sizeof(pt)
            + ( mem_usage_able::mem_usage(product_type::get<I>(forward<PT>(pt)) ) + ... )
            - ( sizeof(   product_type::get<I>(forward<PT>(pt)) ) + ... )
            ;
      }
    }
    template <typename PT>
    constexpr
//    enable_if_t<
//      not is_trivial<remove_cv_t<remove_reference_t<PT>>>::value
//      and is_product_type<remove_cv_t<remove_reference_t<PT>>>::value
//      , size_t>
    //decltype(auto)
    size_t mem_usage(PT&& pt) noexcept
    {
      return detail::mem_usage_impl(forward<PT>(pt), product_type::element_sequence_for<PT>{});
    }

}

namespace mem_usage_able {
  /**
   */

  template <class PT>
  struct traits<PT, meta::when<
    not is_trivial<remove_cv_t<remove_reference_t<PT>>>::value
    and is_product_type<remove_cv_t<remove_reference_t<PT>>>::value
  >>
  {
    template <class T>
      static constexpr size_t apply(T&& pt)
      {
        return product_type::mem_usage(std::forward<T>(pt));
      }
  };
}

}}}

#endif
#endif // header
