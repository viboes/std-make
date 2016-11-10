// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_EXAMPLE_FRAMEWORK_BOOST_OPTIONAL_HPP
#define JASEL_EXAMPLE_FRAMEWORK_BOOST_OPTIONAL_HPP

#if __cplusplus >= 201402L
#include "../mem_usage.hpp"
#include <boost/optional.hpp>

namespace boost
{
  template <typename T>
    constexpr auto mem_usage(const optional<T>& v) noexcept
      -> decltype( std::experimental::mem_usage(declval<T>()) )
    {
      size_t ans = sizeof(T);
      if (v) ans += std::experimental::mem_usage(*v) - sizeof(*v);
      return ans;
    }
//  template <typename T>
//    constexpr auto mem_usage(optional<T>&& v) noexcept
//      -> decltype( std::experimental::mem_usage(declval<T>()) )
//    {
//      size_t ans = sizeof(T);
//      if (v) ans += std::experimental::mem_usage(*v) - sizeof(*v);
//      return ans;
//    }
}


#endif

#endif
