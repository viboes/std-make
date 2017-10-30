// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_type.hpp>

/// This example tries to see how close we can define money using strong_type
/// money should be close to strong_counter as it is close to duration.
/// However, there are some issues.
///     There is not a common currency (until we decide to have it)
///         Adding money with different currencies needs the currency result, and so needs to be lazy
///     Having a common currency means to do two operations to convert from two currencies.
///         The compiler could optimize it if the currency is know at compile time, but in general this is not the case.
///     Should we have implicit or explicit conversions between currencies?

#include <iostream>
#include <functional>
#include <type_traits>
#include "currency.hpp"
#include <experimental/strong_counter.hpp>
#include <experimental/fundamental/v2/config.hpp>

namespace stdex = std::experimental;

#define STRONG_COUNTER 1

#if STRONG_COUNTER==0
template <class M>
using currency_t = typename M::currency_type;

template <class Currency, class Rep = double>
struct money;
namespace detail
{
/// helper function that converts a money to a specific currency using the conversion_factor customization point
template <class Currency, class C, class R>
double to_currency(money<C, R> const& m)
{
    return m.count() * currency::conversion_factor(m.currency(), Currency{});
}

}
#else
template <class M>
using currency_t = typename M::domain::currency_type;

#endif

template<class Currency>
struct money_domain
{
    using currency_type =  Currency;
  template <class T, class C, class U
          , typename std::enable_if <
              std::conjunction <
                std::is_convertible<U, T>, // no overflow
                std::disjunction<
                      std::is_floating_point<T>,
                      std::conjunction<
                          //std::integral_constant<bool, std::ratio_divide<P, Period>::den == 1>,
                          std::true_type,
                          std::negation< std::is_floating_point<U> >
                      >
                  >
              >::value
          >::type* = nullptr
  >
  static T inter_domain_convert(money_domain<C>, U const& u)
  {
      return T(currency::convert<C,Currency>(u));
    //return T(
        //std::chrono::duration_cast<std::chrono::duration<T, Period>>(std::chrono::duration<U,P>(u)).count()
        //);
  }
  template <class T, class C, class U>
  static T inter_domain_cast(money_domain<C>, U const& u)
  {
      return T(currency::convert<C,Currency>(u));
//    return T(
//        std::chrono::duration_cast<std::chrono::duration<T, Period>>(std::chrono::duration<U,P>(u)).count()
//    );
  }
  template <class T, class U
          , typename std::enable_if <
              std::conjunction <
                std::is_convertible<U, T>,
                std::disjunction<
                      std::is_floating_point<T>,
                      std::negation< std::is_floating_point<U> >
                >
              >::value
          >::type* = nullptr
          >
  static T intra_domain_convert(U const& u) { return T(u); }
};

namespace std
{
#if STRONG_COUNTER==0
template <class Currency, class UT1, class UT2>
struct common_type<money<Currency, UT1>, money<Currency, UT2>>
{
 using type = money<Currency,common_type_t< UT1, UT2>>;
};

namespace experimental
{

namespace mixin {
template <class Currency, class UT1, class UT2>
struct is_compatible_with<money<Currency, UT1>, money<Currency, UT2>> : true_type {};
}

namespace meta
{
template <class Currency, class UT1, class UT2>
struct rebind<money<Currency, UT1>, UT2> : id<money<Currency, UT2>> {};
}
}
#endif

namespace experimental
{
namespace mixin {
  template <class Currency>
  struct is_compatible_with<money_domain<Currency>, money_domain<Currency>> : std::true_type {};
}

template <class Currency>
struct domain_converter<money_domain<Currency>> : money_domain<Currency>  {  };
}

}



// fixme: Should be Rep, Currency?
// fixme: Should we add a Period parameter to count in Million Dollars, or in cents?
//      Don't having a period , forces almost to work with double.
//      Having a period allows to work with integers most of the time

#if STRONG_COUNTER==1
template <class Currency, class Rep=double>
using money = stdex::strong_counter<money_domain<Currency>, Rep>;

namespace detail
{
/// helper function that converts a money to a specific currency using the conversion_factor customization point
template <class Currency, class C, class R>
double to_currency(money<C, R> const& m)
{
    return m.count() * currency::conversion_factor(C{}, Currency{});
}

}

//template <class Currency, class Rep>
//struct money : stdex::strong_counter<money_domain<Currency>, Rep>
//{
//    using base_type = stdex::strong_counter<money_domain<Currency>, Rep>;
//    using base_type::base_type;
//}
#else

template <class Currency, class Rep>
struct money
    : stdex::strong_type<money<Currency, Rep>, Rep>
    , stdex::mixin::additive_with_if<money<Currency, Rep>>
    , stdex::mixin::comparable<money<Currency, Rep>>
    , stdex::mixin::integer_multiplicative_with<money<Currency, Rep>, Rep>
    , stdex::mixin::modable<money<Currency, Rep>>
    , stdex::mixin::hashable<money<Currency, Rep>>
    //, mixin::streamable<strong_counter<Domain, UT>>
    //, mixin::ordinal<money<Currency, Rep>>

{
    using base_type = stdex::strong_type<money<Currency, Rep>, Rep>;
    using base_type::base_type;

    using currency_type = Currency;

    template <class UT2>
    using rebind = money<Currency, UT2>;

    money() = default;

    // todo: Add template <class Rep2> as duration
    explicit constexpr money(Rep v) : base_type(v) {}

    constexpr money(money const& m) = default;

    // fixme: Should we restrict it as do duration and require a money_cast?
    // or just define an explicit constructor as does pair.
    template <class C, class R>
    constexpr money(money<C, R> m)
    : base_type (currency::convert<C, Currency>(m.count()))
    {
    }
    constexpr money& operator=(money const& m) = default;
#if 0
    template <class C, class R>
    constexpr money& operator=(money<C, R> m)
    {
        rep = currency::convert<C, Currency>(m.count());
        return *this;
    }
#endif

    // fixme: Should we replace it by count as duration does?

    constexpr Rep count() const noexcept
        { return this->underlying(); }

    //constexpr Currency currency() const noexcept { return Currency{}; }

    // todo: add zero, min, max

};
#endif

namespace money_expr
{
template <class M1, class M2, class Op>
struct binary
{
    M1 m1;
    M2 m2;
    using C1 =  currency_t<M1>;
    using C2 =  currency_t<M2>;

    // fixme:: shouldn't this requires that the representation is convertible from the common_type?
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>() {
        return money<C,R>( Op{}(currency::convert<C1, C>(m1.count()), currency::convert<C2, C>(m2.count())));
    }
};

template <class M1, class M2>
struct add
{
    M1 m1;
    M2 m2;

    using C1 =  currency_t<M1>;
    using C2 =  currency_t<M2>;

    // fixme:: shouldn't this requires that the representation is convertible from the common_type?
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( currency::convert<C1, C>(m1.count()) + currency::convert<C2, C>(m2.count()));
    }
};
template <class M1, class M2>
struct substract
{
    M1 m1;
    M2 m2;

    using C1 =  currency_t<M1>;
    using C2 =  currency_t<M2>;
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( currency::convert<C1, C>(m1.count()) - currency::convert<C2, C>(m2.count()));
    }
};

template <class C, class M1, class M2>
struct divide
{
    M1 m1;
    M2 m2;

    using C1 =  currency_t<M1>;
    using C2 =  currency_t<M2>;
    template <class R>
    JASEL_CXX14_CONSTEXPR operator R()
    {
        return  currency::convert<C1, C>(m1.count()) / currency::convert<C2, C>(m2.count());
    }
};

template <class M1, class M2>
struct modulo
{
    M1 m1;
    M2 m2;

    using C1 =  currency_t<M1>;
    using C2 =  currency_t<M2>;
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( currency::convert<C1, C>(m1.count()) % currency::convert<C2, C>(m2.count()));
    }
};

}


template <class C1, class R1, class C2, class R2, typename = std::enable_if_t< ! std::is_same<C1, C2>::value>>
constexpr auto operator+(money<C1,R1> m1, money<C2,R2> m2) noexcept
-> money_expr::add<money<C1,R1>, money<C2,R2>>
{
    return money_expr::add<money<C1,R1>, money<C2,R2>>{m1, m2} ;
    //return money_expr::binary<money<C1,R1>, money<C2,R2>, std::plus>{m1, m2} ;
}

template <class C1, class R1, class C2, class R2, typename = std::enable_if_t< ! std::is_same<C1, C2>::value>>
constexpr auto operator-(money<C1,R1> m1, money<C2,R2> m2) noexcept
-> money_expr::substract<money<C1,R1>, money<C2,R2>>
{
    return money_expr::substract<money<C1,R1>, money<C2,R2>>{m1, m2} ;
}

template <class OSTREAM, class C, class R>
OSTREAM& operator<<(OSTREAM& os, money<C, R> m)
{
    os<< m.count() << C{}.symbol();
    return os;
}

using dollars = money<currency::dollar>;
using euros = money<currency::euro>;


#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
    {
        dollars d(5);
        BOOST_TEST(long(d.count()) == 5);
    }
    {
        dollars d;
        d = dollars(5);
        BOOST_TEST(long(d.count()) == 5);
    }
    {
        dollars d;
        d = dollars(5);
        BOOST_TEST(d == dollars(5));
    }
    {
        dollars d1(5), d2(10);
        BOOST_TEST(d1+d2 == dollars(15));
    }
    {
        dollars d(5);
        BOOST_TEST(+d  == d);
    }
    {
        dollars d1(5), d2(10);
        BOOST_TEST(d1-d2 == dollars(-5));
    }
    {
        dollars d(5);
        BOOST_TEST(-d  == dollars(-5));
    }
    {
        dollars d(5);
        BOOST_TEST(d * 3  == dollars(15));
    }
    {
        dollars d(5);
        BOOST_TEST(3 * d  == dollars(15));
    }
    {
        dollars d(15);
        BOOST_TEST(d / 3  == dollars(5));
    }
    {
        using idollars = money<currency::dollar, int>;
        idollars d(16);
        BOOST_TEST(d % 3  == idollars(1));
    }
//    {
//        using CR = std::common_type_t<double,int>;
//        double d(16);
//        BOOST_TEST(CR(d) % 3  == 1);
//    }
    {
        dollars d(10);
        euros e(5);
        euros x = d + e;
        std::cout <<x << std::endl;
        BOOST_TEST(x == euros(25));
    }
    {
        euros e1(5);
        dollars d(10);
        euros e2(5);
        euros x = e1 + d + e2;
        std::cout <<x << std::endl;
        BOOST_TEST(x == euros(30));
    }
    {
        dollars d(10);
        euros e(5);
        e =  d;
        BOOST_TEST(e == euros(20));
    }
    {
        dollars d(10);
        euros e(5);
        euros x = d - e;
        std::cout <<x << std::endl;
        BOOST_TEST(x == euros(15));
    }
    {
        dollars d(10);
        euros e(5);
        euros x = d - e;
        std::cout <<x << std::endl;
        BOOST_TEST(x == euros(15));
    }
    {
        euros e1(5);
        dollars d(10);
        euros e2(5);
        euros x = e1 - d - e2;
        std::cout <<x << std::endl;
        BOOST_TEST(x == euros(-20));
    }
    return ::boost::report_errors();
}

