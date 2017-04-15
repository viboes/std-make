// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/not_null.hpp>

#include <experimental/utility.hpp>
#include <memory>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

int read_value(stdex::not_null<std::unique_ptr<int>> p)
{
    return *p;
}

void test_not_null_unique_ptr()
{
    int ans = read_value(stdex::as_not_null(std::unique_ptr<int>{new int{2}}));
    assert (2 == ans);
}


int main()
{
  test_not_null_unique_ptr();

  return ::boost::report_errors();
}

