// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP
#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_HPP

#if __cplusplus >= 201402L

#include <type_traits>
#include <vector>
#include <utility>
#include <cstddef>
#include <experimental/meta.hpp>

//#define JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_DIFF
#if defined JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_DIFF
#else
#define mem_usage_impl mem_usage
#endif

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
  namespace adl
  {
    template <typename T>
    typename std::enable_if<std::is_trivial<T>::value, size_t>::type
    mem_usage_impl(const T& v)
    { return sizeof v;}
#if 0
    template <typename T>
      size_t mem_usage_impl(const std::vector<T>& v);

    template <typename T, typename U>
    size_t mem_usage_impl(const std::pair<T, U>& v)
    {
      return mem_usage_impl(v.first) + mem_usage_impl(v.second);
    }

    template <typename T>
    size_t mem_usage_impl(const std::vector<T>& v)
    {
      size_t ans = sizeof(v);
      for (const T& e : v) ans += mem_usage_impl(e);
      ans += (v.capacity() - v.size()) * sizeof(T);
      return ans;
    }
#endif
    struct mem_usage_fn
    {
      template <typename T>
      auto operator()(const T& v)  const-> decltype(mem_usage_impl(v))
      { return mem_usage_impl(v);}
    };
  } // adl
#if defined JASEL_EXAMPLE_FRAMEWORK_MEM_USAGE_DIFF
  template <typename T>
  auto mem_usage(const T& v)
  -> decltype( adl::mem_usage_fn{}(v) )
  { return adl::mem_usage_fn{}(v);}
#else
  // To avoid ODR violations:
    template <class T>  constexpr T __static_const{};
    // std::begin is a global function object
    namespace {
      constexpr auto const & mem_usage = __static_const<adl::mem_usage_fn>;
    }
#endif
} // mem_usage_able
} // funcdamental_v3
} // experimental
} // std

#endif

#endif
