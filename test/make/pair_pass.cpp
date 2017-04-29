// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>

#include <utility>
#include <memory>
#include <chrono>
#include <tuple>
#include <typeinfo>
//error: '(anonymous struct at make/pair_pass.cpp:15:15)' cannot be defined in a type alias template
//using apair = struct {
//template <class T, class U>
//using apair = struct {
//    T first;
//    U second;
//};

#include <system_error>
#include <cassert>
#include <utility>
#include <future>

template <size_t I, class X>
class bitfield_ref;


template <class OS, size_t I, class X>
OS& operator<<(OS& os, bitfield_ref<I,X> const& br)
{
  os << br;
  return os;
}



struct X3 {

  int i:2;
  int j:5;
  int k;
public:

};

template <>
class bitfield_ref<0,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator int() { return x.i; }
};

template <>
class bitfield_ref<1,X3> {
  X3& x;
public:
  bitfield_ref(X3& x) : x(x) {}
  operator int() { return x.j; }
};

template <>
class bitfield_ref<0,const X3> {
  X3 const& x;
public:
  bitfield_ref(X3 const& x) : x(x) {}
  operator int() { return x.i; }
};

template <>
class bitfield_ref<1, const X3> {
  X3 const& x;
public:
  bitfield_ref(X3 const& x) : x(x) {}
  operator int() { return x.j; }
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

template <std::size_t I>
decltype(auto) get(X3 & x) {
  return get_element(std::integral_constant<size_t, I>{}, x);
}
template <std::size_t I>
decltype(auto) get(X3 const& x) {
  return get_element(std::integral_constant<size_t, I>{}, x);
}

// everything as thread except that do nothing on the destructor. The user is the responsible to doing whatever is needed before destruction.
struct  basic_thread {};

//as now - An implementation could inherit from basic_thread if he considers the ABI change is acceptable
struct  thread : basic_thread {} ;

enum class thread_destruction {terminate, join, detach};

template <thread_destruction P>
struct scoped_thread : basic_thread {
  scoped_thread() {}
  scoped_thread(scoped_thread&&) {}
  scoped_thread(thread&&) {}
};

using terminating_thread = scoped_thread<thread_destruction::terminate>;
using joining_thread = scoped_thread<thread_destruction::join>;
using detaching_thread = scoped_thread<thread_destruction::detach>;

template <thread_destruction P>
void fscoped_thread(scoped_thread<P>&&) {}

template <class T>
using atype = T(*)();

struct H{};
struct P{};

struct NoCopiableNotMovableH {
  NoCopiableNotMovableH(NoCopiableNotMovableH const&) = delete;
  NoCopiableNotMovableH(NoCopiableNotMovableH &&) = delete;
};

namespace std {

struct coroutine {
  template <class T, class ...Args>
  struct traits
  {
    using type = decltype(coroutine_traits(atype<T>{}, atype<Args>{} ...));
  };
  template <class T>
  struct handle
  {
    using type = decltype(coroutine_handle(atype<T>{}));
  };
};

template <class T, class ...Args>
promise<T> coroutine_traits(atype<future<T>>, atype<Args> ...); // undefined

template <class T>
H coroutine_handle(atype<promise<T>>); // undefined

}

P coroutine_traits(atype<int>, atype<short>); // undefined

H coroutine_handle(atype<P>); // undefined


namespace std {
    template <class T, size_t N>
      class tuple_size<T [N]> : public integral_constant<std::size_t, N> {};
    template <size_t I, class T, size_t N>
      class tuple_element<I, T [N] > { public: using type = T; };

//    template <size_t I, class T, size_t N>
//      constexpr T&& get(T v[N]>&&) noexcept {�return c[I]; }
//    template <size_t I, class T, size_t N>
//      constexpr const T& get(const T v[N]) noexcept {�return c[I]; }


}

struct Xbf {
   int a;
   int i:2;
   int j: 5;
};
Xbf fXbf();


template <size_t I, class T, size_t N>
  constexpr T& get(T v[N]) noexcept { return v[I]; }

template <size_t I>
struct __index {};
template <typename T>
struct __type {};

template <class T, std::size_t I>
struct position {
  using type = T;
  T value;
  T& get(__index<I>) { return value; }
  T& get(__type<T>) { return value; }
};

template <class ...Ts>
struct type_sequence {};

namespace std {
  template <typename T0, typename T1>
  class tuple_element<0, type_sequence<T0,T1>> {public: using type=T0;};
  template <typename T0, typename T1>
  class tuple_element<1, type_sequence<T0,T1>> {public: using type=T1;};
}
template <typename Ts, typename Is>
struct __position_aggregate;

template <typename ...Ts, size_t ...Is>
struct __position_aggregate<type_sequence<Ts...>, std::index_sequence<Is...>>
: position<Ts, Is>... {
  //using position<Ts, Is>::get ...;

  template <size_t I>
  typename std::tuple_element<I, type_sequence<Ts...>>::type& get(__index<I>) {
    return this->position<typename std::tuple_element<I, type_sequence<Ts...>>::type, I>::get(__index<I>{});
  }
};

template <typename ...Ts>
struct position_aggregate : __position_aggregate<type_sequence<Ts...>, std::make_index_sequence<sizeof...(Ts)>> {};

namespace std {
  template <typename ...Ts>
  class tuple_size<position_aggregate<Ts...>> : public std::integral_constant<std::size_t, sizeof...(Ts)> {};
  template <size_t I, typename ...Ts>
  class tuple_element<I, position_aggregate<Ts...>> : tuple_element<I, type_sequence<Ts...>> {};
}
namespace XXX {
  struct B {};
  struct C {};

  struct XB {
    int get(B) { return 0; }
  };
  struct XC {
    int get(C) { return 1; }
  };

  struct XA : XB, XC {
    using XB::get;
    using XC::get;
  };
}

namespace N0 {
  struct A{};
struct F1 {
  template <class T>
  int operator()(T) const {
    return 0;
  }
};
constexpr const F1 f={};
}

namespace N1 {
  struct A{};
struct F1 {
  template <class T>
  int operator()(T) const {
    return 1;
  }
};
constexpr const F1 f={};
}

namespace N2 {
  struct A{};
  int f(A) {
    return 2;
  }
}

namespace N3 {
  struct A{};
  int f(A) {
    return 3;
  }
}
namespace disable {
  struct _A_A_{};
  int f() {
    return 1000;
  }
}

struct throw_t{};
struct no_throw_t{};
struct unexpet_t{};

template <class T, class E>
struct expected {
  expected(T) {}
  expected(unexpet_t, E) {}
};

template <class T, class E>
struct result_type;
template <class T>
struct result_type<T, throw_t> { using type = T; };
template <class T>
struct result_type<T, no_throw_t> { using type = expected<T, std::error_code>; };
template <class T, class E>
using result_type_t = typename result_type<T,E>::type;

enum class file_status {a,b};

class directory_entry {
public:
  file_status  status(throw_t) const  {
    return file_status::a;
  }
  expected<file_status, std::error_code>  status(no_throw_t) const noexcept {
    return file_status::a;
  }
  template <class ErrorPolicy=throw_t>
  //      requires Same<ErrorPolicy, throw_t> or Same<ErrorPolicy, no_throw_t>
  result_type_t<file_status, ErrorPolicy>  status() const noexcept(std::is_same<ErrorPolicy, no_throw_t>::value) {
    return status(ErrorPolicy{});
  }
};


template <class T, class U>
struct helper {
  using type = struct {
    T first;
    U second;
  };
};

template <class T, class U>
using apair = typename helper<T,U>::type;


template <class T, class U>
struct pair
{
    T first;
    U second;
    template <class R, class S>
    pair& operator=(pair<R,S> const& p)  {
      first=p.first;
      second=p.second;
      return *this;
    }
    template <class R, class S>
    operator pair<R,S>() const {
      return {first, second};
    }
};

template <class T, class U>
struct pair2 {
    pair<T,U> value;
    pair2() : value{} {}
    pair2& operator=(pair2 const&) = default;
    pair2& operator=(pair2 &&) = default;
};


template <class T, class U>
struct pair3 {
    using storage = struct
        {
            T first;
            U second;
        };
};

struct pair4 {
    struct
        {
            int first;
            int second;
        } x;
        pair4() : x() {}
};

class A
{
    int data_;
public:
    A(int data) : data_(data) {}

    bool operator==(const A& a) const {return data_ == a.data_;}
};

class AC
{
    int data_;
public:
    constexpr AC(int data) : data_(data) {}

    constexpr bool operator==(const AC& a) const {return data_ == a.data_;}
};

struct Base
{
    virtual ~Base() {}
};

struct Derived
    : public Base
{
};

    struct S {
        int i;
        S() : i(0) {}
        S(int j) : i(j) {}
        S * operator& () { BOOST_TEST(false); return this; }
        S const * operator& () const { BOOST_TEST(false); return this; }
        bool operator==(int x) const { return i == x; }
        };

    struct Im{ Im(int v):value(v) {} int value; };
    struct Ex{ explicit Ex(int v):value(v){} int value;};

    struct D { D(int); D(const D&) = delete; };

    pair<int, int> foo_pair()
    {
      return {1, -1};  // OK
    }

    decltype(foo_pair()) foo_pair2()
    {
      return {1, -1};  // OK
    }

//    // fixme
//    typeid(int) foo_pair3()
//    {
//      return typeid(int);
//    }


//    template <class T, class U>
//    struct {
//        std::decay_t<T> first;
//        std::decay_t<U> second;
//    }
//    make_pair(T&& t, U&& u)
//    {
//      return {std::forward<T>(t), std::forward<U>(u)};  // OK
//    }

      using TT =
      struct {
          int first;
          int second;
      }
      ;
      TT
      a_pair()
      {
        return {1, -1};  // OK
      }

//      struct {
//          int first;
//          int second;
//      }
//      aa_pair();


    struct Odd {
      explicit Odd(int) = delete;
      Odd(long){}
    };

    namespace ANS //namespace for a concept
    {
      template <class T> void f(T) {} // one of the functions provided at the concept level
    }
    namespace ONS
    {
      template <class T> void g(T) {}
    }
    namespace X
    {
      // inline class C; // declares a class open for namespace association
      namespace CNS // namespace for class C
      {
        class C
        {};
      }
      using CNS::C; // introduce C

      class D
      {
      };
    }

    //using namespace ANS for X::C;
    namespace X
    {
      namespace CNS // reopen class C
      {
        using ANS::f; // Adds f
        // ...
      }
    }

    template< typename T, typename Base, typename Base::underlying Base::*pvalue >
    struct addition : protected Base {
        explicit addition(typename Base::underlying v) : Base(v) {}
        friend T operator+ ( T const & lhs, T const & rhs )
        {
          return T{ lhs.value + rhs.value };
        }
    };


    template <class T>
    struct wrap {
        using underlying = T;
        T value;
        explicit wrap(T v) : value(v) {}

    };

    class energy : public addition<energy, wrap<float>, &wrap<float>::value>
    {
    public:
      explicit energy(float f) : addition(f) {}
      float get_value() const { return this->value; }
    };
    struct nonesuch {
        nonesuch() = delete;
        ~nonesuch() = delete;
        nonesuch(nonesuch const&) = delete;

        void operator=(nonesuch const&) = delete;
      };
    nonesuch always_thows() {throw 1;}

    int&& fff();

int main()
{
  {
    energy e1{1.0};
    energy e2{1.0};
    energy e3 = e1 + e2;
    BOOST_TEST(e3.get_value() == 2.0);
    //BOOST_TEST(e3.value == 2.0); // value is protected

  }
  {

    X::C c;
    X::D d;

    ANS::f(c);
    f(c);
    //g(c); // ERROR

    ANS::f(d); //OK
    //f(d); // ERROR?
  }
//  {
//    auto aPair = [] () ->
//    struct
//    {
//      int first;
//      int second;
//    }
//    {
//      return {1, 2};
//    };
//    auto p = aPair();
//    BOOST_TEST(p.first == 1);
//    BOOST_TEST(p.second == 2);
//
//  }
  {
    // error: expected class name
    //struct X : struct {

    using TT = struct {
        int first;
        int s;
    };
    static_assert(sizeof(TT)==8, "");

    using T = struct {
        int first;
        int second;
    };

    static_assert(std::is_same<T, T>{}, "");

//    error: '(anonymous struct at make/pair_pass.cpp:183:32)' cannot be defined in a type specifier
//        static_assert(std::is_same<struct {
//    static_assert(std::is_same<
//        struct {
//            int first;
//            int second;
//          },
//        struct {
//            int first;
//            int second;
//          }>{}, "");
    static_assert(std::is_class<T>{}, "");
    static_assert(sizeof(T)==8, "");

    struct X : T {};
    X p;
    p.first=1;
    p.second= 2;
    BOOST_TEST(p.first == 1);
    BOOST_TEST(p.second == 2);
  }
//  {
//    auto p =(struct {
//        int first;
//        int second;
//    })();
//  }
  {
    struct {
        int first;
        int second;
    } p;
    p.first=1;
    p.second= 2;
    BOOST_TEST(p.first == 1);
    BOOST_TEST(p.second == 2);
  }
  {
    struct {
        int first;
        int second;
    } p {1, 2};
    BOOST_TEST(p.first == 1);
    BOOST_TEST(p.second == 2);
  }
//  {
//    struct {
//        short first;
//        int second;
//    } q {1, 2};
//    struct {
//        int first;
//        int second;
//    } p (q);
//    BOOST_TEST(p.first == 1);
//    BOOST_TEST(p.second == 2);
//  }
//  {
//    short p;
//    double q=1.1;
//    p= q;
//  }
  {
    struct {
        double first;
        const char* second;
    } p {};
    BOOST_TEST(p.first == 0.0f);
    BOOST_TEST(p.second == nullptr);
  }
  {
    struct {
        double first;
        const char* second;
    } p, q{};
    p = q;
    BOOST_TEST(p.first == 0.0f);
    BOOST_TEST(p.second == nullptr);
  }
  {
    struct {
        double first;
        const char* second;
    } q{}, p(q);
    p = q;
    BOOST_TEST(p.first == 0.0f);
    BOOST_TEST(p.second == nullptr);
  }
  {
    struct {
        double first;
        std::unique_ptr<int> second;
    } p, q{};
    p = std::move(q);
    BOOST_TEST(p.first == 0.0f);
    BOOST_TEST(p.second == nullptr);
  }
  {
    struct {
        double first;
        std::unique_ptr<int> second;
    } q{}, p(std::move(q));
    p = std::move(q);
    BOOST_TEST(p.first == 0.0f);
    BOOST_TEST(p.second == nullptr);
  }

  {
    //Odd o2 = 1; // OK
    //Odd o1(1);  // Error
    //pair<Odd,Odd> pd{1,2};

  }
  {
    //std::tuple<D,D> pd{1,2};
    //std::pair<D,D> pd{1,2};
    //pair<D,D> pd{1,2};
  }
  {
  typedef pair<Im, Im> P;
  P p{1, 2};
  BOOST_TEST(p.first.value == 1);
  BOOST_TEST(p.second.value == 2);
  }
  {
    typedef pair<int, int> Q;
    Q q { 1, 2 };
    typedef pair<Im, Im> P;
    P p(q);
    BOOST_TEST(p.first.value == 1);
    BOOST_TEST(p.second.value == 2);
  }

//  {
//  typedef pair<Ex, Ex> P;
//  P p{1, 2};
//  BOOST_TEST(p.first.value == 1);
//  BOOST_TEST(p.second.value == 2);
//  }

  {
  //typedef pair<std::chrono::hours, std::chrono::minutes> P;
  //P p{1,2};
  //BOOST_TEST(p.first == 0.0f);
  //BOOST_TEST(p.second == nullptr);
  }

  {
  typedef pair2<float, short*> P;
  P p;
  BOOST_TEST(p.value.first == 0.0f);
  BOOST_TEST(p.value.second == nullptr);
  }
  {
  typedef pair<float, short*> P;
  P p{};
  BOOST_TEST(p.first == 0.0f);
  BOOST_TEST(p.second == nullptr);
  }

  {
  typedef pair<float, short*> P;
  constexpr P p{};
  static_assert(p.first == 0.0f, "");
  static_assert(p.second == nullptr, "");
  }

  {
      typedef pair<float, short*> P;
      P p{3.5f, 0};
      BOOST_TEST(p.first == 3.5f);
      BOOST_TEST(p.second == nullptr);
  }

  {
      typedef pair<double, short*> P;
      P p{3.5f, 0};
      BOOST_TEST(p.first == 3.5f);
      BOOST_TEST(p.second == nullptr);
  }
  {
      typedef pair<A, short> P;
      P p{1, 2};
      BOOST_TEST(p.first == A(1));
      BOOST_TEST(p.second == 2);
  }

  {
      typedef pair<float, short*> P;
      constexpr P p{3.5f, 0};
      static_assert(p.first == 3.5f, "");
      static_assert(p.second == nullptr, "");
  }
  {
      typedef pair<AC, int> P;
      constexpr P p{1, 2};
      static_assert(p.first == AC(1), "");
      static_assert(p.second == 2, "");
  }
  {
      typedef pair<int, short> P1;
      //typedef pair<double, long> P2;
      P1 p1{3, 4};
      //P2 p2 = p1;
      P1 p2 = p1;
      BOOST_TEST(p2.first == 3);
      BOOST_TEST(p2.second == 4);
  }
  {
      typedef pair<int, short> P1;
      //typedef pair<double, long> P2;
      P1 p2;
      p2 = {3, 4};
      BOOST_TEST(p2.first == 3);
      BOOST_TEST(p2.second == 4);
  }

  {
      typedef pair<int, short> P1;
      //typedef pair<double, long> P2;
      constexpr P1 p1{3, 4};
      constexpr P1 p2 = p1;
      //constexpr P2 p2 = p1;
      static_assert(p2.first == 3, "");
      static_assert(p2.second == 4, "");
  }

    // assign_const_pair_U_V
    {
        typedef pair<int, short> P1;
        typedef pair<double, long> P2;
        //P1 p1(3, 4);
        //erreur: no matching function for call to �pair<int, short int>::pair(int, int)�
        //         P1 p1(3, 4);
        P1 p1{3, 4};
        //P1 p2;
        P2 p2;
        p2 = p1;
        //erreur: no match for �operator=� (operand types are �P2 {aka pair<double, long int>}� and �P1 {aka pair<int, short int>}�)
        //         p2 = p1;

        BOOST_TEST(p2.first == 3);
        BOOST_TEST(p2.second == 4);
    }
    // assign_rv_pair
    {
        typedef pair<std::unique_ptr<int>, short> P;
        P p1{std::unique_ptr<int>(new int(3)), 4};
        //P p1(std::unique_ptr<int>(new int(3)), 4);
        //erreur: no matching function for call to �pair<std::unique_ptr<int>, short int>::pair(std::unique_ptr<int>, int)�
        //         P p1(std::unique_ptr<int>(new int(3)), 4);
        P p2;
        p2 = std::move(p1);
        BOOST_TEST(*p2.first == 3);
        BOOST_TEST(p2.second == 4);
    }
    // assign_rv_pair_U_V
    {
        typedef pair<std::unique_ptr<Derived>, int> P1;
        //typedef pair<std::unique_ptr<Base>, long> P2;
        P1 p1{std::unique_ptr<Derived>(), 4};
        //P1 p1(std::unique_ptr<Derived>(), 4);
        //erreur: no matching function for call to �pair<std::unique_ptr<Derived>, short int>::pair(std::unique_ptr<Derived>, int)�
        //         P1 p1(std::unique_ptr<Derived>(), 4);
        P1 p2;
        //P2 p2;
        p2 = std::move(p1);
        //erreur: no match for �operator=� (operand types are �P2 {aka pair<std::unique_ptr<Base>, long int>}� and �std::remove_reference<pair<std::unique_ptr<Derived>, int>&>::type {aka pair<std::unique_ptr<Derived>, int>}�)
        //         p2 = std::move(p1);

        BOOST_TEST(p2.first == nullptr);
        BOOST_TEST(p2.second == 4);
    }

    {
        typedef pair<std::unique_ptr<int>, short*> P;
        P p{std::unique_ptr<int>(new int(3)), nullptr};
        BOOST_TEST(*p.first == 3);
        BOOST_TEST(p.second == nullptr);
    }

    {
        typedef pair<int, short> P1;
        P1 p1{3, 4};
        P1 p2{5, 6};
        std::swap(p1,p2);
        BOOST_TEST(p1.first == 5);
        BOOST_TEST(p1.second == 6);
        BOOST_TEST(p2.first == 3);
        BOOST_TEST(p2.second == 4);
    }
    {
        typedef pair<int, S> P1;
        P1 p1{3, S(4)};
        P1 p2{5, S(6)};
        std::swap(p1,p2);
        BOOST_TEST(p1.first == 5);
        BOOST_TEST(p1.second == 6);
        BOOST_TEST(p2.first == 3);
        BOOST_TEST(p2.second == 4);
    }

    {
        typedef pair<int, int*> P1;
        typedef pair<int*, int> P2;
        typedef pair<P1, P2> P3;
        P3 p3{{3, nullptr}, {nullptr, 4}};
        BOOST_TEST(p3.first.first == 3);
        BOOST_TEST(p3.first.second == nullptr);
        BOOST_TEST(p3.second.first == nullptr);
        BOOST_TEST(p3.second.second == 4);
    }
    {
//      auto st = &directory_entry::status;
//      error: variable 'st' with type 'auto' has incompatible initializer of type '<overloaded function type>'
//            auto st = &directory_entry::get_status;

      auto st1 = static_cast<file_status (directory_entry::*)(throw_t) const>(&directory_entry::status);

      auto st2 = &directory_entry::status<throw_t>;
    }
    {
      N0::A a;
      using namespace N1;
      BOOST_TEST(1==f(a));
    }
    {
      N2::A a;
      using namespace N0;
      BOOST_TEST(0==f(a));
    }
    {
      N2::A a;
      using N0::f;
      //using namespace N1;
      BOOST_TEST(0==f(a));
    }
    {
      N2::A a;
      using namespace N0;
      //using namespace disable;
      using disable::f;
      //using namespace N1;
      BOOST_TEST(2==f(a));
    }
    {
      N3::A a;
      //using N1::f;
      //using namespace N1;
      using namespace N2;
      BOOST_TEST(3==f(a));
    }
    {
      static_assert(std::is_standard_layout<int>::value, "hrrr");
      static_assert(std::is_trivially_copyable<position_aggregate<int, short>>::value, "hrrr");
      position_aggregate<int, short> as;
      as.get(__index<0>{}) = 0;
    }
    {
      XXX::XA as;
      BOOST_TEST(as.get(XXX::B{}) == 0);
    }
    {
      //constexpr int v[3]= {1,2,3};
      //BOOST_TEST(get<0>(v) == 1);
      static_assert(std::tuple_size<int [3]>::value == 3, "hehe");
      static_assert(std::is_same<std::tuple_element_t<1,int [3]>, int>::value, "hehe");
    }
    {
      double i;
      static_assert(sizeof(i) == 8, "hehe");
      double& ir = i;
      static_assert(sizeof(ir) == 8, "hehe");
      double&& irv = 1;
      static_assert(sizeof(irv) == 8, "hehe");
      static_assert(sizeof(double&&) == 8, "hehe");
      static_assert(sizeof(fff()) == 4, "hehe");

    }
    {
      int arr[3];

      static_assert(std::is_same<decltype(arr), int[3]>::value, "hehe");
      static_assert(std::tuple_size<decltype(arr)>::value== 3, "hehe");
      //static_assert(std::is_same<std::tuple_element_t<1,decltype(arr)>, int>::value, "hehe");
    }
    {
      Xbf x;
      static_assert(sizeof(x.a)==4, "");
      static_assert(std::is_same<decltype(x.a), int>::value, "hehe");
      //static_assert(sizeof(x.i)==4, "");
      static_assert(std::is_same<decltype(x.i), int>::value, "hehe");
      static_assert(std::is_same<decltype(x.j), int>::value, "hehe");
    }
    {
      atype<int> r;
      atype<short> p1;

      static_assert(std::is_same<decltype(coroutine_traits(r,p1)), P>::value, "hehe");
      using P_ =  decltype(coroutine_traits(r,p1));

      atype<P_> p;

      static_assert(std::is_same<decltype(coroutine_handle(p)), H>::value, "hehe");
    }
    {
      atype<std::future<int>> r;
      atype<short> p1;

      static_assert(std::is_same<decltype(coroutine_traits(r,p1)), std::promise<int>>::value, "hehe");
      static_assert(std::is_same<decltype(coroutine_traits(atype<std::future<int>>{},atype<short>{})), std::promise<int>>::value, "hehe");
      static_assert(std::is_same<std::coroutine::traits<std::future<int>,short>::type, std::promise<int>>::value, "hehe");
      using P_ =  decltype(coroutine_traits(r,p1));

      atype<P_> p;

      static_assert(std::is_same<decltype(coroutine_handle(p)), H>::value, "hehe");
    }
    {
      //NoCopiableNotMovableH v;
      atype<NoCopiableNotMovableH> r;
      class B {};
      class D : public B {};
      atype<B> b;
      atype<D> d;
      auto f = [](atype<B>){};
      //f(d);
    }
    {
      joining_thread th;
      fscoped_thread(std::move(th));
    }
    {
      thread th;
      //fscoped_thread(std::move(th));
    }
    {
      X3 x {0,1,2};
      std::cout << x.i << " " << x.j << " " << x.k << std::endl;

      //auto& xi = get<0>(x);
      auto xi = get<0>(x);
      //std::cout << get<0>(x) << std::endl;
      //auto& xj = get<1>(x);
      auto& xk = get<2>(x);
      std::cout << xk << std::endl;
      xk=1;

    }
}
