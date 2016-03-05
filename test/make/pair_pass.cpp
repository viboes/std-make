// Copyright (C) 2014-2015 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#include <boost/detail/lightweight_test.hpp>

#include <utility>
#include <memory>
#include <cassert>
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
        S * operator& () { assert(false); return this; }
        S const * operator& () const { assert(false); return this; }
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
int main()
{
  {
    energy e1{1.0};
    energy e2{1.0};
    energy e3 = e1 + e2;
    assert(e3.get_value() == 2.0);
    //assert(e3.value == 2.0); // value is protected

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
//    assert(p.first == 1);
//    assert(p.second == 2);
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
    assert(p.first == 1);
    assert(p.second == 2);
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
    assert(p.first == 1);
    assert(p.second == 2);
  }
  {
    struct {
        int first;
        int second;
    } p {1, 2};
    assert(p.first == 1);
    assert(p.second == 2);
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
//    assert(p.first == 1);
//    assert(p.second == 2);
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
    assert(p.first == 0.0f);
    assert(p.second == nullptr);
  }
  {
    struct {
        double first;
        const char* second;
    } p, q{};
    p = q;
    assert(p.first == 0.0f);
    assert(p.second == nullptr);
  }
  {
    struct {
        double first;
        const char* second;
    } q{}, p(q);
    p = q;
    assert(p.first == 0.0f);
    assert(p.second == nullptr);
  }
  {
    struct {
        double first;
        std::unique_ptr<int> second;
    } p, q{};
    p = std::move(q);
    assert(p.first == 0.0f);
    assert(p.second == nullptr);
  }
  {
    struct {
        double first;
        std::unique_ptr<int> second;
    } q{}, p(std::move(q));
    p = std::move(q);
    assert(p.first == 0.0f);
    assert(p.second == nullptr);
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
  assert(p.first.value == 1);
  assert(p.second.value == 2);
  }
  {
    typedef pair<int, int> Q;
    Q q { 1, 2 };
    typedef pair<Im, Im> P;
    P p(q);
    assert(p.first.value == 1);
    assert(p.second.value == 2);
  }

//  {
//  typedef pair<Ex, Ex> P;
//  P p{1, 2};
//  assert(p.first.value == 1);
//  assert(p.second.value == 2);
//  }

  {
  //typedef pair<std::chrono::hours, std::chrono::minutes> P;
  //P p{1,2};
  //assert(p.first == 0.0f);
  //assert(p.second == nullptr);
  }

  {
  typedef pair2<float, short*> P;
  P p;
  assert(p.value.first == 0.0f);
  assert(p.value.second == nullptr);
  }
  {
  typedef pair<float, short*> P;
  P p{};
  assert(p.first == 0.0f);
  assert(p.second == nullptr);
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
      assert(p.first == 3.5f);
      assert(p.second == nullptr);
  }

  {
      typedef pair<double, short*> P;
      P p{3.5f, 0};
      assert(p.first == 3.5f);
      assert(p.second == nullptr);
  }
  {
      typedef pair<A, short> P;
      P p{1, 2};
      assert(p.first == A(1));
      assert(p.second == 2);
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
      assert(p2.first == 3);
      assert(p2.second == 4);
  }
  {
      typedef pair<int, short> P1;
      //typedef pair<double, long> P2;
      P1 p2;
      p2 = {3, 4};
      assert(p2.first == 3);
      assert(p2.second == 4);
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
        //erreur: no matching function for call to Ôpair<int, short int>::pair(int, int)Õ
        //         P1 p1(3, 4);
        P1 p1{3, 4};
        //P1 p2;
        P2 p2;
        p2 = p1;
        //erreur: no match for Ôoperator=Õ (operand types are ÔP2 {aka pair<double, long int>}Õ and ÔP1 {aka pair<int, short int>}Õ)
        //         p2 = p1;

        assert(p2.first == 3);
        assert(p2.second == 4);
    }
    // assign_rv_pair
    {
        typedef pair<std::unique_ptr<int>, short> P;
        P p1{std::unique_ptr<int>(new int(3)), 4};
        //P p1(std::unique_ptr<int>(new int(3)), 4);
        //erreur: no matching function for call to Ôpair<std::unique_ptr<int>, short int>::pair(std::unique_ptr<int>, int)Õ
        //         P p1(std::unique_ptr<int>(new int(3)), 4);
        P p2;
        p2 = std::move(p1);
        assert(*p2.first == 3);
        assert(p2.second == 4);
    }
    // assign_rv_pair_U_V
    {
        typedef pair<std::unique_ptr<Derived>, int> P1;
        //typedef pair<std::unique_ptr<Base>, long> P2;
        P1 p1{std::unique_ptr<Derived>(), 4};
        //P1 p1(std::unique_ptr<Derived>(), 4);
        //erreur: no matching function for call to Ôpair<std::unique_ptr<Derived>, short int>::pair(std::unique_ptr<Derived>, int)Õ
        //         P1 p1(std::unique_ptr<Derived>(), 4);
        P1 p2;
        //P2 p2;
        p2 = std::move(p1);
        //erreur: no match for Ôoperator=Õ (operand types are ÔP2 {aka pair<std::unique_ptr<Base>, long int>}Õ and Ôstd::remove_reference<pair<std::unique_ptr<Derived>, int>&>::type {aka pair<std::unique_ptr<Derived>, int>}Õ)
        //         p2 = std::move(p1);

        assert(p2.first == nullptr);
        assert(p2.second == 4);
    }

    {
        typedef pair<std::unique_ptr<int>, short*> P;
        P p{std::unique_ptr<int>(new int(3)), nullptr};
        assert(*p.first == 3);
        assert(p.second == nullptr);
    }

    {
        typedef pair<int, short> P1;
        P1 p1{3, 4};
        P1 p2{5, 6};
        std::swap(p1,p2);
        assert(p1.first == 5);
        assert(p1.second == 6);
        assert(p2.first == 3);
        assert(p2.second == 4);
    }
    {
        typedef pair<int, S> P1;
        P1 p1{3, S(4)};
        P1 p2{5, S(6)};
        std::swap(p1,p2);
        assert(p1.first == 5);
        assert(p1.second == 6);
        assert(p2.first == 3);
        assert(p2.second == 4);
    }

    {
        typedef pair<int, int*> P1;
        typedef pair<int*, int> P2;
        typedef pair<P1, P2> P3;
        P3 p3{{3, nullptr}, {nullptr, 4}};
        assert(p3.first.first == 3);
        assert(p3.first.second == nullptr);
        assert(p3.second.first == nullptr);
        assert(p3.second.second == 4);
    }
}
