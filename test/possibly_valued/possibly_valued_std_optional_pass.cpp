// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/possibly_valued.hpp>
// <experimental/optional.hpp>

#if defined JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
#warning JASEL_STD_EXPERIMENTAL_FUNDAMENTALS_V2_STD_OPTIONAL_NOT_INSTALLED
int main()
{
}
#else

#define JASEL_STD_EXPERIMENTAL_FACTORIES_USE_OPTIONAL
//#define JASEL_FUNDAMENTAL_EXTENDED

#include <experimental/optional.hpp>

#include <boost/detail/lightweight_test.hpp>

std::experimental::nullopt_t null() { return std::experimental::nullopt;}

int twice(int i) {
  return 2*i;
}

template<class T>
void g1(T &x) {
  auto xx = std::experimental::deref(T(1));
  std::cout << xx << std::endl;
}

template<class T>
void h1(T const &x) {
  g1(x);
}


int main()
{
  namespace stde = std::experimental;

  static_assert(stde::is_possibly_valued<stde::optional<int>>::value, "ERROR");

  static_assert(std::is_same<
      stde::possibly_valued::value_type_t<stde::optional<int>>,
      int
    >::value, "ERROR");

#if defined JASEL_FUNDAMENTAL_EXTENDED
  static_assert(std::is_same<stde::possibly_valued::value_type_t<stde::optional<int&>>, int&>::value, "ERROR");
#endif
  {
    stde::optional<int> x(stde::nullopt);
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x = stde::nullopt;
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x { stde::nullopt };
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x = stde::optional<int>(stde::nullopt) ;
    BOOST_TEST(stde::possibly_valued::has_value(x));
    BOOST_TEST(stde::possibly_valued::deref(x) == stde::nullopt);
  }

# if !defined __clang__ && defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ < 6)
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
#endif
#endif

#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
#endif


#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { null() } };
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST(! stde::possibly_valued::has_value(x));
  }
#endif
  {
    stde::optional<int> x = stde::nullopt;
    BOOST_TEST(! stde::possibly_valued::has_value(x));

  }
  {
    int v=1;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(stde::possibly_valued::has_value(x));
    BOOST_TEST(stde::possibly_valued::deref(x) == 1);
    x = 2;
    BOOST_TEST(stde::possibly_valued::deref(x) == 2);
  }
  {
    int v=1;
    const stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(stde::possibly_valued::deref(x) == 1);
    h1(x);
  }
  {
    int v=0;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(stde::possibly_valued::deref(x) == 0);
    static_assert(std::is_same<int&, decltype(stde::possibly_valued::deref(x))>::value, "a");
  }
  {
    int v=0;
    BOOST_TEST(stde::possibly_valued::deref(stde::make_optional(v)) == 0);
    static_assert(std::is_same<stde::optional<int>, decltype(stde::make_optional(v))>::value, "a");
  }

  {
    stde::optional<int> o((stde::in_place));
    BOOST_TEST(stde::possibly_valued::has_value(o));
  }
  {
    int v=0;
    stde::optional<int&> x = stde::make_optional(std::ref(v));
    BOOST_TEST(&v == &x.value());
    BOOST_TEST(&v == std::addressof(x.value()));
    BOOST_TEST(&v == std::addressof(deref(x)));

  }


  //possibly_valued::value_or
  {
    stde::optional<int> x = stde::none<stde::optional>();
    int y = stde::possibly_valued::value_or(x, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    int y = stde::possibly_valued::value_or(x, 2);
    BOOST_TEST(y == 1);
  }

  //possibly_valued::apply_or
  {
    stde::optional<int> x = stde::none<stde::optional>();
    int y = stde::possibly_valued::apply_or(x, twice, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    int y = stde::possibly_valued::apply_or(x, twice, -1);
    BOOST_TEST(y == 2);
  }

  return ::boost::report_errors();
}
#endif
