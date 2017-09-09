// Copyright (C) 2014-2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/nullable.hpp>
// <experimental/optional.hpp>

#include <experimental/expected.hpp>

#include <boost/detail/lightweight_test.hpp>

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int twice(int i) {
  return 2*i;
}

std::experimental::optional<int> mtwice(int i) {
  return std::experimental::make_optional(2*i);
}

std::experimental::optional<double> inverse(double x) {
  if (x==0) return std::experimental::nullopt;
  return 1/x;
}

template <class T>
struct check;


template<class T>
T& g(T &x) {
  return std::experimental::deref(T(1));
}

template<class T>
void g1(T &x) {
  auto xx = std::experimental::deref(T(1));
  std::cout << xx << std::endl;

}

template<class T>
T const& h(T const &x) {
  return g(x);
}
template<class T>
void h1(T const &x) {
  g1(x);
}

int res(std::experimental::nullopt_t) {
  return -1;
}


int main()
{
  namespace stde = std::experimental;

  static_assert(not std::is_base_of<stde::nullable::tag, stde::nullable::traits<stde::none_t>>::value, "ERROR");
  static_assert(not stde::is_nullable<stde::none_t>::value, "ERROR");
  static_assert(stde::is_nullable<stde::optional<int>>::value, "ERROR");

  static_assert(std::is_same<
      stde::nullable::none_type_t<stde::optional<int>>,
      stde::nullopt_t
    >::value, "ERROR");
  static_assert(std::is_same<
      stde::nullable::value_type_t<stde::optional<int>>,
      int
    >::value, "ERROR");

#if defined JASEL_FUNDAMENTAL_EXTENDED
  static_assert(std::is_same<stde::value_type_t<stde::optional<int&>>, int&>::value, "ERROR");
#endif
  {
    auto n = stde::none_t{};
    stde::optional<int> x(n);
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<stde::optional<stde::_t>>());
    BOOST_TEST(stde::nullopt == stde::nullopt);
    BOOST_TEST(stde::deref_none(x) == stde::nullopt);

  }
  {
    stde::optional<stde::optional<int>> x = stde::none() ;
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<stde::optional<stde::_t>>());
  }
  {
    stde::optional<stde::optional<int>> x { stde::none() };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<stde::optional<stde::_t>>());
  }
  {
    stde::optional<stde::optional<int>> x = stde::optional<int>(stde::none()) ;
    BOOST_TEST(x);
    BOOST_TEST(has_value(x));
    BOOST_TEST(x != stde::none());
    BOOST_TEST(stde::none() != x);
    BOOST_TEST(x != stde::none<stde::optional<stde::_t>>());

    BOOST_TEST(stde::deref(x) == stde::none());
    BOOST_TEST(stde::none() == stde::deref(x));
    BOOST_TEST(stde::deref(x) == stde::none<stde::optional<stde::_t>>());
  }
  {
    stde::optional<stde::optional<int>> x { stde::optional<int>(stde::none()) };
    BOOST_TEST(x);
    BOOST_TEST(has_value(x));
  }

# if !defined __clang__ && defined __GNUC__ // NOTE: GNUC is also defined for Clang
#   if (__GNUC__ < 6)
  {
    stde::optional<stde::optional<int>> x { { stde::none() } };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif
#endif

#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::none() } };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif


#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::nullopt } };
    BOOST_TEST( ! x);
    BOOST_TEST( ! has_value(x));
  }
#endif
  {
    stde::optional<stde::optional<int>> x { stde::none<stde::optional>() };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x = stde::none<stde::optional>() ;
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  {
    stde::optional<stde::optional<int>> x = stde::none<stde::optional<stde::optional<int>> >() ;
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
  // fixme: do we want this to work. See https://github.com/viboes/std-make/issues/14
#if 0
  {
    stde::optional<stde::optional<int>> x = stde::none<stde::optional<int>>() ;
    BOOST_TEST(x);
    BOOST_TEST(has_value(x));
  }
#endif
  {
    stde::optional<stde::optional<int>> x { stde::none<stde::optional>() };
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }

#if defined __clang__
  {
    stde::optional<stde::optional<int>> x { { stde::none<stde::optional>() } };
    BOOST_TEST( ! x);
    BOOST_TEST( ! has_value(x));
  }
#endif
  {
    stde::optional<int> x = stde::none<stde::optional>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x == stde::none<stde::optional<stde::_t>>());

#if defined JASEL_FUNDAMENTAL_EXTENDED
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(! stde::has_value(y));
#endif
  }
  {
    int v=1;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());
    BOOST_TEST(deref(x) == 1);
#if defined JASEL_FUNDAMENTAL_EXTENDED
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(deref(y) == 2);
#endif
    x = 2;
    BOOST_TEST(deref(x) == 2);

  }
  {
    int v=1;
    const stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(*x == 1);
    BOOST_TEST(x != stde::none());
    BOOST_TEST(deref(x) == 1);
    h1(x);
    //BOOST_TEST(h(x) == 1);

#if defined JASEL_FUNDAMENTAL_EXTENDED
    stde::optional<int> y = stde::fmap(twice, x);
    BOOST_TEST(deref(y) == 2);
#endif

  }
  {
    int v=0;
    stde::optional<int> x = stde::make_optional(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(deref(x) == 0);
    static_assert(std::is_same<int&, decltype(deref(x))>::value, "a");
  }
  {

    int v=0;
    BOOST_TEST(deref(stde::make_optional(v)) == 0);
    //check<decltype(stde::make_optional(v))> xx;
    static_assert(std::is_same<stde::optional<int>, decltype(stde::make_optional(v))>::value, "a");
  }
#if 0
  {
    short v=0;
    stde::optional<int> x = stde::make_optional<int>(v);
    BOOST_TEST(*x == 0);
    BOOST_TEST(value(x) == 0);
  }
#endif
  static_assert(std::is_constructible<A,int, int>::value, "a");
  static_assert(std::is_constructible<A>::value, "b");

  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t,int, int>::value, "a");
  static_assert(std::is_constructible<stde::optional<A>, stde::in_place_t>::value, "b");

#if 0
  {
    int v=1;
    stde::optional<A> x = stde::make_optional<A>(stde::in_place, v,v);
    BOOST_TEST(x->v == 2);
    BOOST_TEST(deref(x).v == 2);
  }
  {
    stde::optional<int> x = stde::make_optional<int>(stde::in_place);
    BOOST_TEST_EQ(*x,  0);
    BOOST_TEST(deref(x) == 0);
  }
#endif
  {
    stde::optional<int> o((stde::in_place));
    BOOST_TEST(o);
    BOOST_TEST(has_value(o));
  }
#if 0
  {
    stde::optional<int> o((stde::in_place));
    stde::optional<int> x = stde::make_optional<int>();
    BOOST_TEST(! x);
    BOOST_TEST(! has_value(x));
  }
#endif
  {
    int v=0;
    stde::optional<int&> x = stde::make_optional(std::ref(v));
    BOOST_TEST(&v == &x.value());
    BOOST_TEST(&v == std::addressof(x.value()));
    BOOST_TEST(&v == std::addressof(deref(x)));

  }
  {
    stde::optional<int> x;
    BOOST_TEST(x == stde::none());
    BOOST_TEST(stde::none() == x);
    BOOST_TEST(x <= stde::none());
    BOOST_TEST(stde::none() <= x);
    BOOST_TEST(x >= stde::none());
    BOOST_TEST(stde::none() >= x);
  }
  {
    stde::optional<int> x=stde::make_optional(1);
    BOOST_TEST(x != stde::none());
    BOOST_TEST(stde::none() != x);
    BOOST_TEST(x > stde::none());
    BOOST_TEST(stde::none() < x);
  }
  {
    stde::optional<int> x=stde::make_optional(1);
    BOOST_TEST(x != stde::none<stde::optional<int>>());
    BOOST_TEST(stde::none() != x);
    BOOST_TEST(x > stde::none());
    BOOST_TEST(stde::none() < x);
  }

 // nullable::transform
  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int> y = stde::nullable::transform(x, twice);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make_optional(v);
    stde::optional<int> y = stde::nullable::transform(x, twice);
    BOOST_TEST(stde::deref(y) == 2);
  }
  {
    int v=1;
    stde::optional<int> y = stde::nullable::transform(stde::make_optional(v), twice);
    BOOST_TEST(stde::deref(y) == 2);
  }
  {
    int v=1;
    const stde::optional<int> x = stde::make_optional(v);
    stde::optional<int> y = stde::nullable::transform(x, twice);
    BOOST_TEST(stde::deref(y) == 2);
  }
  // nullable::ap
  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::nullable::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::nullable::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    const stde::optional<int> x = stde::make<stde::optional>(1);
    const stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::nullable::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int(*)(int)> f = stde::none<stde::optional>();
    stde::optional<int> y = stde::nullable::ap(f, stde::make<stde::optional>(1));
    BOOST_TEST(! stde::has_value(y));
  }
  {
    stde::optional<int> x = stde::none<stde::optional>();
    stde::optional<int(*)(int)> f = stde::make<stde::optional>(twice);
    stde::optional<int> y = stde::nullable::ap(f, x);
    BOOST_TEST(! stde::has_value(y));
  }
  {
    int v=1;
    stde::optional<int> x = stde::make<stde::optional>(v);
    stde::optional<int(*)(int)> f = stde::make<stde::optional>(twice);
    stde::optional<int> y = stde::nullable::ap(f, x);
    BOOST_TEST(stde::deref(y) == 2);
  }
  {
    int v=1;
    stde::optional<int(*)(int)> f = stde::make<stde::optional>(twice);
    stde::optional<int> y = stde::nullable::ap(f, stde::make<stde::optional>(v));
    BOOST_TEST(stde::deref(y) == 2);
  }
  // nullable::bind
   {
     stde::optional<int> x = stde::none<stde::optional>();
     stde::optional<int> y = stde::nullable::bind(x, mtwice);
     BOOST_TEST(! stde::has_value(y));
   }
   {
     int v=1;
     stde::optional<int> x = stde::make_optional(v);
     stde::optional<int> y = stde::nullable::bind(x, mtwice);
     BOOST_TEST(stde::deref(y) == 2);
   }
   {
     int v=1;
     stde::optional<int> y = stde::nullable::bind(stde::make_optional(v), mtwice);
     BOOST_TEST(stde::deref(y) == 2);
   }
   {
     int v=1;
     const stde::optional<int> x = stde::make_optional(v);
     stde::optional<int> y = stde::nullable::bind(x, mtwice);
     BOOST_TEST(stde::deref(y) == 2);
   }

  //nullable::value_or
  {
    stde::optional<int> x = stde::none<stde::optional>();
    int y = stde::nullable::value_or(x, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    int y = stde::nullable::value_or(x, 2);
    BOOST_TEST(y == 1);
  }
  //nullable::apply_or
  {
    stde::optional<int> x = stde::none<stde::optional>();
    int y = stde::nullable::apply_or(x, twice, 1);
    BOOST_TEST(y == 1);
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    int y = stde::nullable::apply_or(x, twice, -1);
    BOOST_TEST(y == 2);
  }
  //nullable::has_error
  {
    stde::optional<int> x = stde::none<stde::optional>();
    BOOST_TEST(stde::nullable::has_error(x, stde::nullopt));
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    BOOST_TEST(! stde::nullable::has_error(x, stde::nullopt));
  }

  //nullable::resolve
  {
    stde::optional<int> x = stde::none<stde::optional>();
    int y = stde::nullable::resolve(x, res);
    BOOST_TEST(y == -1);
  }
  {
    stde::optional<int> x = stde::make<stde::optional>(1);
    int y = stde::nullable::resolve(x, res);
    BOOST_TEST(y == 1);
  }

  return ::boost::report_errors();
}
