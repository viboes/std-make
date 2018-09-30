// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L
#include <type_traits>
#include <experimental/optional.hpp>
#include <utility>
#include <vector>
#include <experimental/meta.hpp>

//#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_DIFF

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
  using std::experimental::meta::when;
  using std::experimental::meta::is_valid;
  using std::experimental::meta::void_t;

namespace mem_usage_able
{
namespace adl {

  template< class C >
    constexpr auto mem_usage( C& c ) noexcept -> decltype(c.mem_usage()) { return c.mem_usage(); }
  template <typename T>
  constexpr std::enable_if_t<std::is_trivial<T>::value, size_t>
     mem_usage(const T& v) noexcept { return sizeof v; }

  template <typename T>
#if 1
  constexpr size_t mem_usage(const std::vector<T>& v) noexcept;
#else
  // unable to forward declare SFINAE interface
  constexpr auto mem_usage(const std::vector<T>& v) noexcept -> decltype(mem_usage(v[0]));
#endif

  // overload for std::optional
  template <typename T>
  constexpr auto mem_usage(const std::experimental::optional<T>& v) noexcept -> decltype(mem_usage(*v))
  {
      size_t ans = sizeof(v);
      if (v) ans += mem_usage(*v) - sizeof(*v);
      return ans;
  }

  // overload for std::pair
  template <typename T, typename U>
  constexpr auto mem_usage(const std::pair<T,U>& v) noexcept -> decltype(mem_usage(v.first)
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
  constexpr size_t mem_usage(const std::vector<T>& v) noexcept
#else
    // unable to forward declare SFINAE interface
    constexpr auto mem_usage(const std::vector<T>& v) noexcept -> decltype(mem_usage(v[0]))
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
    constexpr auto apply( C& c ) noexcept -> decltype(mem_usage(c)) { return mem_usage(c); }
}

  template <typename T>
  constexpr auto mem_usage(const T& v) noexcept -> decltype(adl::apply(v))
    { return adl::apply(v); }
} // concept

}}}
#endif

#endif
