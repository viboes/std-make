// Copyright (C) 2014 Vicente J. Botet Escriba
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

#define VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR

#include <experimental/make.hpp>
#include <memory>
#include <boost/detail/lightweight_test.hpp>

namespace std {

#if __cplusplus <= 201103L
  template <int = 0, int..., class T>
    unique_ptr<decay_t<T>> make_unique(T&& x)
  {
    return unique_ptr<decay_t<T>>(new decay_t<T>(x));
  }

  // explicit overloads
  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T> const& x)
  {
    return unique_ptr<T>(new T(x));
  }

  template <class T>
    unique_ptr<T> make_unique(remove_reference_t<T>&& x)
  {
    return unique_ptr<T>(new T(std::forward<remove_reference_t<T>>(x)));
  }

  // variadic overload
  template <class T, class ...Args>
    unique_ptr<T> make_unique(Args&&... args)
  {
    return unique_ptr<T>(new T(forward<Args>(args)...));
  }
#endif

  // customization point for template (needed because std::unique_ptr doesn't has a conversion constructor)
  template <class T, class X>
  unique_ptr<T> make(experimental::type<unique_ptr<T>>, X&& x)
  {
    return make_unique<T>(forward<X>(x));
  }

  // customization point for template (needed because std::unique_ptr doesn't uses experimental::in_place_t)
  template <class T, class ...Args>
  unique_ptr<T> make(experimental::type<unique_ptr<T>>, experimental::in_place_t, Args&& ...args)
  {
    return make_unique<T>(forward<Args>(args)...);
  }


  // Holder customization
  template <class D>
  struct unique_ptr<experimental::_t, D>
  {
      template<class T>
      using apply = unique_ptr<T, experimental::rebind<D, T>>;
  };

  template <>
  struct default_delete<experimental::_t> : experimental::lift<default_delete> {};

  // todo remove this specialization
  template <>
  struct unique_ptr<void> {};

#ifdef VIBOES_STD_EXPERIMENTAL_FUNDAMENTALS_V2_MAKE_TYPE_CONSTRUCTOR
  namespace experimental
  {
  // type_constructor customization
    template <class T, class D>
    struct type_constructor<unique_ptr<T,D>> : identity<unique_ptr<_t, D>> {};
    template <class T>
    struct type_constructor<default_delete<T>> : identity<default_delete<_t>> {};
  }
#endif
}

struct A
{
  int v;
  A(): v(3) {}
  A(int v): v(std::move(v)) {}
  A(int v1, int v2): v(v1+v2) {}
};

int main()
{
  namespace stde = std::experimental;
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = std::make_unique<int>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<int>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=1;
    std::unique_ptr<A> x = std::make_unique<A>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    int v=1;
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>(v,v);
    BOOST_TEST(x->v == 2);
  }
  {
    std::unique_ptr<A> x = stde::make<std::unique_ptr<A>>();
    BOOST_TEST_EQ(x->v,  3);
  }
  {
    int v=0;
    std::unique_ptr<int> x = stde::make<std::unique_ptr<stde::_t>>(v);
    BOOST_TEST(*x == 0);
  }
  {
    int v=0;
    std::unique_ptr<int,std::default_delete<int> > x = stde::make<std::unique_ptr<stde::_t, std::default_delete<stde::_t> >>(v);
    BOOST_TEST(*x == 0);
  }
  return ::boost::report_errors();
}
