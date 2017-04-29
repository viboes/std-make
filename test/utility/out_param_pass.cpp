// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/.../utility/out_param.hpp>

#include <experimental/utility.hpp>
#include <string>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

void assign(stdex::out_param<std::string&> obj, std::string const& value)
{
  obj.get() = value;
}

//void func(int in_a, int in_b, stdex::out_param<int&> out)
//{
//}

int main()
{
  {
    std::string s;
    assign(stdex::out(s), "value");
    assert (s == "value");
  }

  return ::boost::report_errors();
}

