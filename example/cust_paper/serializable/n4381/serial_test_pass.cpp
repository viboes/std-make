// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L

#include "stdx/serializable.hpp"

#include <tuple>
#include <experimental/utility.hpp>
#include <array>

#include <iostream>

#include <boost/detail/lightweight_test.hpp>
bool trace = false;

int main()
{
  namespace stdx = std::experimental;
  {
    {
      stdx::archives::binary_archive ar;
      int v = 0;
      ar.save(v);
      std::cout << ar.size() << "\n";
    }
    {
      stdx::archives::binary_archive ar;
      int v = 0;
      stdx::serializable::save(ar, v);
      std::cout << ar.size() << "\n";
    }
#if 0

    {
      stdx::archives::binary_archive ar;
      std::vector<int> v;
      // compile fails as expected
      decltype(stdx::serializable::save(ar, v)) xx;
//      ../example/cust_paper/serializable/n4381/serial_test_pass.cpp:41:16: error: no matching function for call to object of type 'const std::experimental::fundamental_v3::serializable::adl_serializable::save_fn'
//            decltype(stdx::serializable::save(ar, v)) xx;
//                     ^~~~~~~~~~~~~~~~~~~~~~~~
//      ../example/cust_paper/serializable/n4381/stdx/serializable.hpp:80:22: note: candidate template ignored: substitution failure [with T = std::__1::vector<int, std::__1::allocator<int> > &, Archine = std::experimental::fundamental_v3::binary_archive]: no matching function for call to 'save'
//            constexpr auto operator()(Archine& ar, T&& v)  const noexcept -> decltype(save(ar, forward<T>(v)))
//                           ^                                                          ~~~~
//      1 error generated.


    }
#endif
#if __cplusplus > 201402L
    {
      using T = std::tuple<int, int>;
      static_assert(not std::is_trivial<T>::value, "");
      static_assert(stdx::is_product_type<T>::value, "");
      T t;
      stdx::archives::binary_archive ar;
      stdx::serializable::save(ar, t);
      std::cout << ar.size() << "\n";
    }
    {
      using T = std::pair<int, int>;
      static_assert(! std::is_trivial<T>::value, "");
      static_assert(stdx::is_product_type<T>::value, "");
      T t;
      stdx::archives::binary_archive ar;
      stdx::serializable::save(ar, t);
      std::cout << ar.size() << "\n";
    }
    {
      using T = std::array<int, 3>;
      static_assert(std::is_trivial<T>::value, "");
      static_assert(stdx::is_product_type<T>::value, "");
      T t;
      stdx::archives::binary_archive ar;
      stdx::serializable::save(ar, t);
      std::cout << ar.size() << "\n";
    }
  {
    // pt of trivial ok as trivial is declared before pt
    stdx::archives::binary_archive ar;
    std::tuple<std::array<int,3>, int> t;
    stdx::serializable::save(ar, t);
    std::cout << ar.size() << "\n";
  }
#if 0
  {
    // trivial of pt KO as trivial is declared before pt
    stdx::archives::binary_archive ar;
    std::pair<std::pair<int, short>,int> t;
    stdx::serializable::save(ar, t);
    std::cout << ar.size() << "\n";
  }
#endif
#if 0
  {
    // trivial of pt KO as trivial is declared before pt
    stdx::archives::binary_archive ar;
    static_assert(! std::is_trivial<std::array<std::pair<int, short>,2>>::value, "");
    static_assert(stdx::is_product_type<std::array<std::pair<int, short>,2>>::value, "");
    std::array<std::pair<int, short>,2> t;
    stdx::serializable::save(ar, t);
    std::cout << ar.size() << "\n";
  }
#endif
#endif
  }
  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
