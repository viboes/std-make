// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L

#include <type_traits>
#include <experimental/meta.hpp>
#include <cstddef>

// mem_usage_able/mem_usage.hpp
namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace mem_usage_able
{

  template <typename T, typename Enabler=void>
  struct traits : traits<T, meta::when<true>> {};
  template <typename R, bool B>
  struct traits<R, meta::when<B>>
  {
    template <typename T>
    static constexpr size_t apply(T&& v) = delete;
  };
  template <typename R>
  struct traits<R, meta::when<std::is_trivial<R>::value>>
  {
    template <typename T>
    static constexpr size_t apply(T&& v) noexcept { return sizeof ( remove_cv_t<remove_reference_t<T>> ); }
  };


  template <typename T>
  constexpr auto mem_usage(T&& v) noexcept -> decltype(traits<remove_cv_t<remove_reference_t<T>>>::apply(std::forward<T>(v)))
  {
    return traits<remove_cv_t<remove_reference_t<T>>>::apply(std::forward<T>(v));
  }

  template <typename R>
  struct traits<R*>
  {
    template <typename T>
    static constexpr auto apply(T* v) noexcept -> decltype( mem_usage_able::mem_usage ( *v ) )
    {
      return sizeof v + (v?mem_usage_able::mem_usage ( *v ):0);
    }
  };

}
}
}
}
#endif

#endif
