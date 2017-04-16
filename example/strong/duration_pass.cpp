// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_counter.hpp>

/**
 * This example tries to see how close we can define std::chrono duration using strong_counter
 *
 * We need a specific Domain for durations to state how durations convert between them.
 */

#include <experimental/strong_counter.hpp>
#include <experimental/type_traits.hpp>
#include <ratio>
#include <chrono>
#include <utility>
#include <type_traits>

namespace stdex = std::experimental;

// fixme: Shouldn't all the strong_counter domains require most of these conditions (except the Period related)?
// todo generalize to dimension_1_domain<duration, Period>
// todo generalize to dimensionless_domain<Tag>
// fixme Shouldn't those have a Tag parameter to make them strongly typed?
template<class Period>
struct duration_domain
{
  template <class T, class P, class U
          , typename std::enable_if <
              stdex::conjunction <
                std::is_convertible<U, T>, // no overflow
                stdex::disjunction<
                      std::chrono::treat_as_floating_point<T>,
                      stdex::conjunction<
                          std::integral_constant<bool, std::ratio_divide<P, Period>::den == 1>,
                          stdex::negation< std::chrono::treat_as_floating_point<U> >
                      >
                  >
              >::value
          >::type* = nullptr
  >
  static T inter_domain_convert(duration_domain<P>, U const& u)
  {
    return T(
        std::chrono::duration_cast<std::chrono::duration<T, Period>>(std::chrono::duration<U,P>(u)).count()
        );
  }
  template <class T, class P, class U>
  static T inter_domain_cast(duration_domain<P>, U const& u)
  {
    return T(
        std::chrono::duration_cast<std::chrono::duration<T, Period>>(std::chrono::duration<U,P>(u)).count()
    );
  }
  template <class T, class U
          , typename std::enable_if <
              stdex::conjunction <
                std::is_convertible<U, T>,
                stdex::disjunction<
                      std::chrono::treat_as_floating_point<T>,
                      stdex::negation< std::chrono::treat_as_floating_point<U> >
                >
              >::value
          >::type* = nullptr
          >
  static T intra_domain_convert(U const& u) { return T(u); }
};



template<
    class Rep,
    class Period = std::ratio<1>
>
using duration = stdex::strong_counter<duration_domain<Period>, Rep>;


namespace std {
  template <class P1, class P2>
  struct common_type<duration_domain<P1>, duration_domain<P2>>
  {
     using CP = typename common_type<std::chrono::duration<int,P1>,std::chrono::duration<int,P2>>::type::period;
     using type = duration_domain<CP>;
  };

namespace experimental {
  template <class Period>
  struct domain_converter<duration_domain<Period>> : duration_domain<Period>  {  };
}
}

static_assert(std::is_same<
                  std::common_type<
                    duration_domain<std::ratio<1,10>>,
                    duration_domain<std::ratio<1,100>>
                  >::type
                , duration_domain<std::ratio<1,100>>
              >::value, "");

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

template <class T> struct check;
int main()
{
#if 0
  {
    using U = int;
    using T = int;
    using P = std::ratio<1,10>;
    using Period = std::ratio<1,10>;

  static_assert(
      stdex::conjunction <
                      std::is_convertible<U, T>, // no overflow
                      stdex::disjunction<
                            std::chrono::treat_as_floating_point<T>,
                            stdex::conjunction<
                                std::integral_constant<bool, std::ratio_divide<P, Period>::den == 1>,
                                stdex::negation< std::chrono::treat_as_floating_point<U> >
                            >
                        >
                    >::value
      , "");
  }

  auto x1 = duration_domain<std::ratio<1,10>>::inter_domain_convert<int>(duration_domain<std::ratio<1,10>>{}, 1);
  auto x2 = stdex::inter_domain_convert<duration_domain<std::ratio<1,10>>, int>(duration_domain<std::ratio<1,10>>{}, 1);
#endif

    {
      duration<int, std::ratio<1,10>> d1 {30};
      auto d2 = d1 ++;
      BOOST_TEST(d1 != d2);
      BOOST_TEST(d1.count() == 31);
      BOOST_TEST(d2.count() == 30);
    }
    {
      duration<int, std::ratio<1,10>> d1 {30};
      auto d2 = ++d1;
      BOOST_TEST(d1 == d2);
      BOOST_TEST(d1.count() == 31);
      BOOST_TEST(d2.count() == 31);
    }
    {
      duration<int, std::ratio<1,10>> d1 {30};
      duration<int, std::ratio<1,10>> d2{d1};
      BOOST_TEST(d2.count() == 30);
    }
    {
      duration<int, std::ratio<1,10>> d1 {30};
      duration<int, std::ratio<1,10>> d2 =d1;
      BOOST_TEST(d2.count() == 30);
    }
    {
      duration<int, std::ratio<1,10>> d1 {30};
      duration<int, std::ratio<1,10>> d2;
      d2 = d1;
      BOOST_TEST(d2.count() == 30);
    }
    {
      duration<int, std::ratio<1,10>> d1 {300};
      duration<int, std::ratio<1,100>> d2  = d1;

      static_assert(
          std::ratio_divide<std::ratio<1,10>, std::ratio<1,100>>::den == 1
          , "");

      static_assert(
          stdex::detail::is_inter_domain_convertible<duration_domain<std::ratio<1,100>>, int, int, duration_domain<std::ratio<1,10>>>::value
          , "");
      static_assert(
          std::ratio_divide<std::ratio<1,100>, std::ratio<1,10>>::den != 1
          , "");


      static_assert(
          ! stdex::detail::is_inter_domain_convertible<duration_domain<std::ratio<1,10>>, int, int, duration_domain<std::ratio<1,100>>>::value
          , "ERROR");

#if defined __clang__ && (__clang_major__ < 4)
      duration<int, std::ratio<1,10>> d3;
      // fixme: This shouldn't compile - clang-3.9.1 BUG
      std::cout << "------- \n";
      d3= d2; // calling move assignment :(
      std::cout << "------- \n";
#else
      BOOST_TEST(d1 == d2);
#endif
      BOOST_TEST_EQ(d2.count() , 3000);
    }
    {
      duration<int, std::ratio<1,100>> d1 {303};
      duration<int, std::ratio<1,10>> d2  = stdex::strong_counter_cast<duration<int, std::ratio<1,10>>>(d1);
#if defined __clang__ && (__clang_major__ < 4) // clang-3.9.1 BUG
      // fixme: This should compile
      //BOOST_TEST(d1 != d2);
#else
      BOOST_TEST(d1 != d2);
#endif
      BOOST_TEST(d2.count() == 30);
    }
    {
      duration<int, std::ratio<1,10>> d1 {300};
      duration<int, std::ratio<1,10>> d2  = d1;
      auto d = d1 + d2;
      BOOST_TEST(d.count() == 600);
    }
    {
      duration<int, std::ratio<1,10>> d1 {300};
      duration<int, std::ratio<1,10>> d2  = d1;
      auto d = d1 - d2;
      BOOST_TEST(d.count() == 0);
    }
    {
      duration<int, std::ratio<1,100>> d1 {3};
      duration<int, std::ratio<1,10>> d2{1};
      auto d =  d1 + d2;
      BOOST_TEST_EQ(d.count() , 13);
    }
    {
      duration<int, std::ratio<1,100>> d1 {3};
      duration<int, std::ratio<1,10>> d2{1};
      auto d =  d2 + d1;
      BOOST_TEST_EQ(d.count() , 13);
    }
    return ::boost::report_errors();
}

