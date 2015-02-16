// Copyright (C) 2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/make.hpp>

//  template <template <class ...> class M, class X>
//  auto make(X&& x);
//  template <class M, class X>
//  auto make(X&& x);
//  template <class M, class ...Args>
//  auto make(Args&& ...args);


#include <experimental/make.hpp>
#include <utility>
#include <boost/detail/lightweight_test.hpp>

namespace std {


  // customization point for template (needed because std::unique_ptr doesn't has a conversion constructor)
  template <class T1, class T2, class X1, class X2>
  constexpr pair<T1, T2> make(experimental::meta::type<pair<T1, T2>>, X1&& x1, X2&& x2)
  {
    return pair<T1, T2>(forward<X1>(x1), forward<X2>(x2));
  }

  // customization point for template (needed because std::unique_ptr doesn't uses experimental::in_place_t)
  template <class T1, class T2, class ...Args>
  constexpr pair<T1, T2> make(experimental::meta::type<pair<T1, T2>>, experimental::in_place_t, Args&& ...args)
  {
    return pair<T1, T2>(forward<Args>(args)...);
  }

  // Holder customization
  template <>
  struct pair<experimental::_t, experimental::_t>
  {
  private:
    template <class Types>
    struct impl;
    template <class T, class U>
    struct impl<experimental::meta::types<T, U>> : experimental::meta::id<pair<T, U>>{};
    // this specialization is needed to avoid instantiation of pair<T>
    template <class T>
    struct impl<experimental::meta::types<T>> : experimental::meta::id<T>{};
  public:
      template <class ...Ts>
      using apply = experimental::meta::eval<impl<experimental::meta::types<Ts...>>>;
  };

  // todo remove this specialization
  template <>
  struct pair<void, void> {};

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
  namespace experimental
  {
    namespace meta
    {
      // type_constructor customization
      template <class T1, class T2>
      struct type_constructor<pair<T1,T2>> : id<pair<_t, _t>> {};
    }
  }
#endif
}

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<long, long> x = stde::make<std::pair<long, long> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    int v=0;
    std::pair<int, int> x = stde::make<std::pair<stde::_t, stde::_t> >(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  {
    constexpr int v=0;
    constexpr std::pair<int, int> x = stde::make<std::pair>(v, v);
    BOOST_TEST(x.first == 0);
    BOOST_TEST(x.second == 0);
  }
  return ::boost::report_errors();
}
