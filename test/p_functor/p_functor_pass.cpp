// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/functor.hpp>

#include <experimental/functor.hpp>
#include <boost/detail/lightweight_test.hpp>


int main()
{
#if __cplusplus >= 201402L

  namespace stde = std::experimental;

  static_assert(! stde::is_functor_v<int>, "");

  #endif

  return ::boost::report_errors();
}
