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

//#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_DIFF

namespace framework {
  using std::experimental::meta::when;
  using std::experimental::meta::is_valid;
  using std::experimental::meta::void_t;

namespace adl_mem_usage {

  template< class C >
    constexpr auto mem_usage( C& c ) -> decltype(c.mem_usage()) { return c.mem_usage(); }
  template <typename T>
  std::enable_if_t<std::is_trivial<T>::value, size_t>
     mem_usage(const T& v) { return sizeof v; }

  template <typename T>
#if 1
    size_t mem_usage(const std::vector<T>& v);
#else
  // unable to forward declare SFINAE interface
    auto mem_usage(const std::vector<T>& v) -> decltype(mem_usage(v[0]));
#endif

  // overload for std::optional
  template <typename T>
  auto mem_usage(const std::experimental::optional<T>& v) -> decltype(mem_usage(*v))
  {
      size_t ans = sizeof(v);
      if (v) ans += mem_usage(*v) - sizeof(*v);
      return ans;
  }

  // overload for std::pair
  template <typename T, typename U>
  auto mem_usage(const std::pair<T,U>& v) -> decltype(mem_usage(v.first)
      + mem_usage(v.second))
  {
    return mem_usage(v.first)
    + mem_usage(v.second)
    + sizeof(v)
    - sizeof(v.first)
    - sizeof(v.second);
  }

  // overload for std::vector
  template <typename T>
#if 1
    size_t mem_usage(const std::vector<T>& v)
#else
    // unable to forward declare SFINAE interface
    auto mem_usage(const std::vector<T>& v) -> decltype(mem_usage(v[0]))
#endif
  {
      size_t ans = sizeof(v);
      for (const T& e : v) ans += mem_usage(e);
      ans += (v.capacity() - v.size()) * sizeof(T);
      return ans;    }

  template <class T, typename Enabler=void>
    struct is_adl_mem_usage : std::false_type {};
  template <class T>
    struct is_adl_mem_usage<T,void_t<
        decltype(mem_usage(std::declval<T>))
        >
        > : std::true_type {};

  template< class C >
    constexpr auto apply( C& c ) -> decltype(mem_usage(c)) { return mem_usage(c); }
}
namespace concept {
  template <typename T>
  auto mem_usage(const T& v) -> decltype(adl_mem_usage::apply(v))
    { return adl_mem_usage::apply(v); }
} // concept
} // framework
#endif

#endif
