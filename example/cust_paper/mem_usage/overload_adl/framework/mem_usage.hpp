// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L
#include <type_traits>
#include <experimental/optional>
#include <utility>
#include <vector>
#include <experimental/meta.hpp>

namespace framework {
  using std::experimental::meta::when;
  using std::experimental::meta::is_valid;
namespace adl_mem_usage {

  template< class C >
    constexpr auto mem_usage_impl( C& c ) -> decltype(c.mem_usage()) { return c.mem_usage(); }
  template <typename T>
  std::enable_if_t<std::is_trivial<T>::value, size_t>
     mem_usage_impl(const T& v) { return sizeof v; }

  template <typename T>
#if 1
    size_t mem_usage_impl(const std::vector<T>& v);
#else
  // unable to forward declare SFINAE interface
    auto mem_usage_impl(const std::vector<T>& v) -> decltype(mem_usage_impl(v[0]));
#endif

  // overload for std::optional
  template <typename T>
  auto mem_usage_impl(const std::experimental::optional<T>& v) -> decltype(mem_usage_impl(*v))
  {
      size_t ans = sizeof(v);
      if (v) ans += mem_usage_impl(*v) - sizeof(*v);
      return ans;
  }

  // overload for std::pair
  template <typename T, typename U>
  auto mem_usage_impl(const std::pair<T,U>& v) -> decltype(mem_usage_impl(v.first)
      + mem_usage_impl(v.second))
  {
    return mem_usage_impl(v.first)
    + mem_usage_impl(v.second)
    + sizeof(v)
    - sizeof(v.first)
    - sizeof(v.second);
  }

  // overload for std::vector
  template <typename T>
#if 1
    size_t mem_usage_impl(const std::vector<T>& v)
#else
    auto mem_usage_impl(const std::vector<T>& v) -> decltype(mem_usage_impl(v[0]))
#endif
  {
      size_t ans = sizeof(v);
      for (const T& e : v) ans += mem_usage_impl(e);
      ans += (v.capacity() - v.size()) * sizeof(T);
      return ans;    }

  template <class T, typename Enabler=void>
    struct is_adl_mem_usage : std::false_type {};
  template <class T>
    struct is_adl_mem_usage<T,std::void_t<
        decltype(mem_usage_impl(std::declval<T>))
        >
        > : std::true_type {};

  template< class C >
    constexpr auto apply( C& c ) -> decltype(mem_usage_impl(c)) { return mem_usage_impl(c); }
}
  template <typename T>
#if 0
    size_t mem_usage(const T& v)
#else
  // this is possible only of the name of the user and the customization interface are different.
  auto mem_usage(const T& v)
    -> decltype(adl_mem_usage::apply(v))
#endif
    { return adl_mem_usage::apply(v); }
}

#endif

#endif
