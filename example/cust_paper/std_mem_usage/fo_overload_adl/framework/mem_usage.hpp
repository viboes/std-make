// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L

#include <type_traits>
#include <cstddef>
#include <utility>
#include <experimental/meta.hpp>
#include <experimental/product_type.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  namespace adl_mem_usage
  {
    template <typename T>
    constexpr typename std::enable_if<is_trivial<T>::value, size_t>::type
    mem_usage(const T& v)
    { return sizeof v;}

#if __cplusplus > 201402L

    namespace detail {
      template <class PT, std::size_t... I>
      constexpr decltype(auto) mem_usage_impl( PT&& pt, index_sequence<I...> ) noexcept
      {
        return  sizeof(pt)
            + ( mem_usage(product_type::get<I>(forward<PT>(pt)) ) + ... )
            - ( sizeof(   product_type::get<I>(forward<PT>(pt)) ) + ... )
            ;
      }
    }
    template <typename PT>
    constexpr
//    enable_if_t<conjunction_v<
//          is_product_type<remove_cv_t<remove_reference_t<PT>>>
//      //, all_of<is_adl_mem_usage_able<product_type::element<I,PT>>
//      >
//      , size_t>
    decltype(auto) mem_usage(PT&& pt) noexcept
    {
      return detail::mem_usage_impl(forward<PT>(pt), product_type::element_sequence_for<PT>{});
    }
#endif

    struct mem_usage_fn
    {
      template <typename T>
      constexpr auto operator()(const T& v)  const noexcept -> decltype(mem_usage(v))
      { return mem_usage(v);}
    };
  } // adl
  // To avoid ODR violations:
    template <class T>  constexpr T __static_const{};
    // std::begin is a global function object
    namespace {
      constexpr auto const & mem_usage = __static_const<adl_mem_usage::mem_usage_fn>;
    }
} // funcdamental_v3
} // experimental
} // std

#endif

#endif
