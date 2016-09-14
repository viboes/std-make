// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>

#include <functional>
#include <cassert>
#include <iostream>

#include <experimental/fundamental/v2/callable_wrapper.hpp>
#include <experimental/optional.hpp>

template <class T>
struct is_optional : std::false_type {};
template <class T>
struct is_optional<std::experimental::optional<T>> : std::true_type {};

template <class R>
R f_fptr(R (&c)(int)) { return c(0); }

//int f_fptr(int (&c)(int)) { return c(0); }

template <class C>
typename std::result_of<C(int)>::type f_orig(C  c) { return c(0); }


template <class R, class C >
R f_aux(std::experimental::callable_wrapper<C, R(int)> c) { return c(0); }

template <class C>
auto f(C c)
    -> decltype( f_aux(std::experimental::make_callable_wrapper<int>(c)) )
{
    return f_aux(std::experimental::make_callable_wrapper<int>(c)) ;
}

template <class T, class U, class C >
std::experimental::optional<U> mbind_aux(std::experimental::optional<T> o, std::experimental::callable_wrapper<C, std::experimental::optional<U>(T)> c) {
    if (o) return c(*o);
    else return std::experimental::nullopt;
}

// auto mbind(std::experimental::optional<T>, (T -> optional<U>) ) -> optional<U>

template <class T, class C, typename = typename std::enable_if<
    //std::experimental::meta::is_callable_v<C(T)> &&
    is_optional<typename std::result_of<C(T)>::type>::value
    >::type >
auto mbind(std::experimental::optional<T> o, C c)
    -> decltype( mbind_aux(o, std::experimental::make_callable_wrapper<T>(c)) )
{
    return mbind_aux(o, std::experimental::make_callable_wrapper<T>(c)) ;
}

template <class T, class U, class C >
std::experimental::optional<U> fmap_aux(std::experimental::callable_wrapper<C, U(T)> c, std::experimental::optional<T> o) {
    if (o) return std::experimental::make_optional(c(*o));
    else return std::experimental::nullopt;
}

// auto fmap((T -> U), std::experimental::optional<T> ) -> optional<U>

template <class T, class C, typename = typename std::enable_if<true
    //&& std::experimental::meta::is_callable_v<C(T)>
>::type >
auto fmap(C c, std::experimental::optional<T> o)
    -> decltype( fmap_aux(std::experimental::make_callable_wrapper<T>(c), o) )
{
    return fmap_aux(std::experimental::make_callable_wrapper<T>(c), o) ;
}


struct X {
  X() {}
    int operator()(int i) const  { return i; }
    short operator()(short i) const { return 2*i; }
};

// fixme when
#if 0
static_assert(std::experimental::meta::is_callable_v<X(int)>, "");
#endif
struct XO {
    std::experimental::optional<int> operator()(int i) const { return i; }
};

int fX(int i) { return 2*i; }
std::experimental::optional<int> fXo(int i) { return 2*i; }

template <class T>
struct check{};

int main() {
  using namespace std;
  namespace stde = std::experimental;

    assert(f_fptr(fX) == 0);

    X x;
    assert(f_orig(x) == 0);

    static_assert(is_same<typename result_of<X(int)>::type, int>::value, "");
    assert(x(1) == 1);

    stde::callable_wrapper<X, typename result_of<X(int)>::type(int)>c {x};
    assert(c(1) == 1);
    assert(f_aux(c) == 0);
    assert(f(x) == 0);
    assert(f(c) == 0);
    assert(f(fX) == 0);

    {
    static_assert(is_optional<typename result_of<XO(int)>::type>::value, "");
    XO xo;
    stde::optional<int> o{1};
    assert(*mbind(o, xo) == 1);
    assert(*mbind(o, fXo) == 2);
    }
    {
    stde::optional<int> o;
    assert(mbind(o, fXo) == stde::nullopt);
    }
    {
    stde::optional<int> o{1};
    assert(*fmap(x, o) == 1);
    assert(*fmap(fX, o) == 2);
    }
    {
    stde::optional<short> o{1};
    assert(*fmap(x, o) == 2);
    }
    {
    const X xc;
    stde::optional<short> o{1};
    assert(*fmap(xc, o) == 2);
    }
    {
    stde::optional<int> o;
    assert(fmap(x, o) == stde::nullopt);
    }


    {
    XO xo;
    stde::optional<int> o{1};
    auto r  = fmap(xo, o);
    static_assert(is_same<decltype(r), stde::optional<stde::optional<int>>>::value, "");

    }
    {
    // uncomment the following lines like to see the kind of compile erors
    //X x;
    //stde::optional<int> o{1};
    //auto r  = mbind(o, x);
    }
    {
    // uncomment the following like to see the kind of compile erors
    //int i=0;
    //stde::optional<int> o{1};
    //auto r  = fmap(i, o);
    }


}
