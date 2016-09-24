// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/product_type.hpp>
#if __cplusplus >= 201402L and defined JASEL_SUPPORT_SWAPPABLE

#include <experimental/swappable.hpp>
#include <array>
#include <tuple>

#include <boost/detail/lightweight_test.hpp>

int main()
{
  namespace stde = std::experimental;
  {
    using T = int;
    using U = int;

    int i=0;
    int j=1;
    static_assert(! stde::swappable::is_adl_swappable_v<T>, "Hrr");
    static_assert(! stde::swappable::is_trait_swappable_v<T,U>, "Hrr");
    static_assert(std::is_move_constructible<T>::value, "Hrr");
    static_assert(std::is_move_assignable<T>::value, "Hrr");

#if defined __clang
    auto xx = stde::meta::is_valid<
      decltype(
            stde::swappable::swap((T&)std::declval<T>(), (U&)std::declval<U>())
      )>;
    static_assert(stde::is_swappable<T>::value, "Hrr");
#endif
    stde::swappable::swap(i,j);
    BOOST_TEST(1 == i);
    BOOST_TEST(0 == j);


  }
  {
      int arr1[] = {0,1,2};
      int arr2[] = {3,4,5};
#if defined __clang
      static_assert(stde::is_swappable<decltype(arr1), decltype(arr2)>::value, "Hrr");
#endif
      stde::swappable::swap(arr1,arr2);
      BOOST_TEST(3 == arr1[0]);
      BOOST_TEST(4 == arr1[1]);
      BOOST_TEST(5 == arr1[2]);
      BOOST_TEST(0 == arr2[0]);
      BOOST_TEST(1 == arr2[1]);
      BOOST_TEST(2 == arr2[2]);
  }
  {
      using T = std::array<int,3>;

      T arr1 = {{0,1,2}};
      T arr2 = {{3,4,5}};
#if defined __clang
      static_assert( stde::swappable::is_adl_swappable_v<T>, "Hrr");
#endif
      static_assert(! stde::swappable::is_trait_swappable_v<T>, "Hrr");
#if defined __clang
      static_assert(stde::is_swappable<T>::value, "Hrr");
#endif
      swap(arr1,arr2);
      swap(arr1,arr2);
      stde::swappable::swap(arr1, arr2);
      BOOST_TEST(3 == arr1[0]);
      BOOST_TEST(4 == arr1[1]);
      BOOST_TEST(5 == arr1[2]);
      BOOST_TEST(0 == arr2[0]);
      BOOST_TEST(1 == arr2[1]);
      BOOST_TEST(2 == arr2[2]);
  }
  {
      using T = std::tuple<int, int,int>;

      T arr1 {0,1,2};
      T arr2 {3,4,5};
#if defined __clang
      static_assert( stde::swappable::is_adl_swappable_v<T>, "Hrr");
#endif
      static_assert(! stde::swappable::is_trait_swappable_v<T>, "Hrr");
#if defined __clang
      static_assert(stde::is_swappable<T>::value, "Hrr");
#endif
      swap(arr1,arr2);
      swap(arr1,arr2);
      stde::swappable::swap(arr1, arr2);
      BOOST_TEST(3 == std::get<0>(arr1));
      BOOST_TEST(4 == std::get<1>(arr1));
      BOOST_TEST(5 == std::get<2>(arr1));
      BOOST_TEST(0 == std::get<0>(arr2));
      BOOST_TEST(1 == std::get<1>(arr2));
      BOOST_TEST(2 == std::get<2>(arr2));
  }

  return ::boost::report_errors();
}
#else
int main() { return 0; }
#endif
