#include <tuple>
#include <iostream>

template <size_t I, class X>
class bitfield_ref;

struct X3 {
  unsigned i:2;
  int j:5;
  int k;
public:

};

template <>
class bitfield_ref<0,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator unsigned() const { return x.i; }
  bitfield_ref& operator=(int v)
  {
    x.i = v;
    return *this;
  }
};

template <>
class bitfield_ref<1,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator int() const { return x.j; }
  bitfield_ref& operator=(int v)
  {
    x.j = v;
    return *this;
  }
};

template <>
class bitfield_ref<0,const X3> {
  X3 const& x;
public:
  bitfield_ref(X3 const& x) : x(x) {}
  operator unsigned() const { return x.i; }
};

template <>
class bitfield_ref<1, const X3> {
  X3 const& x;
public:
  bitfield_ref(X3 const& x) : x(x) {}
  operator int() const { return x.j; }
};

template <>
class bitfield_ref<0,X3&&> {
  X3 &&x;
public:
  bitfield_ref(X3 && x) : x(std::move(x)) {}
  operator unsigned() const { return x.i; }
};

template <>
class bitfield_ref<1, X3&&> {
  X3 && x;
public:
  bitfield_ref(X3 && x) : x(std::move(x)) {}
  operator int() const { return x.j; }
};

namespace std {
  template <>
  class tuple_size<X3> : integral_constant<size_t, 3> {};
  template <>
  class tuple_element<0,X3> { public: using type = bitfield_ref<0, const X3>; };
  template <>
  class tuple_element<1,X3> { public: using type = bitfield_ref<1, const X3>; };
  template <>
  class tuple_element<2,X3> { public: using type = int; };
}

bitfield_ref<0, X3> get_element(std::integral_constant<size_t, 0>, X3 & x) {
  return bitfield_ref<0, X3>(x);
}
bitfield_ref<1, X3> get_element(std::integral_constant<size_t, 1>, X3 & x) {
  return bitfield_ref<1, X3>(x);
}
int& get_element(std::integral_constant<size_t, 2>, X3 & x) {
  return x.k;
}

bitfield_ref<0, const X3> get_element(std::integral_constant<size_t, 0>, X3 const& x) {
  return bitfield_ref<0, const X3>(x);
}
bitfield_ref<1, const X3> get_element(std::integral_constant<size_t, 1>, X3 const& x) {
  return bitfield_ref<1, const X3>(x);
}
int const& get_element(std::integral_constant<size_t, 2>, X3 const& x) {
  return x.k;
}
#if 1
bitfield_ref<0, X3&&> get_element(std::integral_constant<size_t, 0>, X3 && x) {
  return bitfield_ref<0, X3&&>(std::move(x));
}
bitfield_ref<1, X3&&> get_element(std::integral_constant<size_t, 1>, X3 && x) {
  return bitfield_ref<1, X3&&>(std::move(x));
}
int && get_element(std::integral_constant<size_t, 2>, X3 && x) {
  return std::move(x.k);
}
#endif

template <std::size_t I>
decltype(auto) get(X3 & x) {
  return get_element(std::integral_constant<size_t, I>{}, x);
}
template <std::size_t I>
decltype(auto) get(X3 const& x) {
  return get_element(std::integral_constant<size_t, I>{}, x);
}
template <std::size_t I>
decltype(auto) get(X3 && x) {
  return get_element(std::integral_constant<size_t, I>{}, std::forward<X3>(x));
}

X3 make_X3() { return X3{}; }

int main()
{
    X3 x {0,1,2};
    std::cout << x.i << " " << x.j << " " << x.k << std::endl;
    {
      int xi = get<0>(x);
      std::cout << xi << std::endl;
    }
    {
      auto xi = get<0>(x);
      static_assert(std::is_same<decltype(xi), bitfield_ref<0,X3>>::value, "Hrr");
      std::cout << xi << std::endl;
    }
    {
      auto xi = get<0>(make_X3());
      static_assert(std::is_same<decltype(xi), bitfield_ref<0, X3&&>>::value, "Hrr");
      std::cout << xi << std::endl;
    }
#if 0
    {
      auto &xi = get<0>(x);
      static_assert(std::is_same<decltype(xi), bitfield_ref<0,X3>&>::value, "Hrr");
      std::cout << xi << std::endl;
    }
#endif
    {
      auto &&xi = get<0>(x);
      static_assert(std::is_same<decltype(xi), bitfield_ref<0,X3>&&>::value, "Hrr");
      std::cout << xi << std::endl;
    }
    {
      auto &&xi = get<0>(make_X3());
      static_assert(std::is_same<decltype(xi), bitfield_ref<0,X3&&>&&>::value, "Hrr");
      std::cout << xi << std::endl;
    }
    {
      std::cout << get<0>(x) << std::endl;
    }
    {
      std::cout << get<0>(make_X3()) << std::endl;
    }
    {
      auto& xk = get<2>(x);
      std::cout << xk << std::endl;
      xk=1;
    }
    {
      auto xi = get<0>(x);
      static_assert(std::is_same<decltype(xi), bitfield_ref<0, X3>>::value, "Hrr");
      std::cout << xi << std::endl;
      xi = 3;
      std::cout << xi << std::endl;
    }
    {
      auto xi = get<1>(x);
      static_assert(std::is_same<decltype(xi), bitfield_ref<1, X3>>::value, "Hrr");
      std::cout << xi << std::endl;
      xi = 3;
      std::cout << xi << std::endl;
    }
    {
      auto xr = make_X3();
      auto xi = get<0>(xr);
      static_assert(std::is_same<decltype(xi), bitfield_ref<0, X3>>::value, "Hrr");
      std::cout << xi << std::endl;
      xi = 3;
      std::cout << xi << std::endl;
      std::cout << xr.i << std::endl<< std::endl;
    }
    {
      auto xr = make_X3();
      static_assert(std::is_same<decltype(xr), X3>::value, "Hrr");
      auto xi = get<1>(xr);
      static_assert(std::is_same<decltype(xi), bitfield_ref<1, X3>>::value, "Hrr");
      std::cout << xi << std::endl;
      xi = 3;
      std::cout << xi << std::endl;
      std::cout << xr.i << std::endl<< std::endl;
    }

}
