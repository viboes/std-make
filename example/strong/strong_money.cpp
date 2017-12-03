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

template <class M>
using currency_t = typename M::currency_type;

template <class Currency, class Rep = double>
struct money;

namespace std
{
template <class Currency, class UT1, class UT2>
struct common_type<money<Currency, UT1>, money<Currency, UT2>>
{
 using type = money<Currency,common_type_t< UT1, UT2>>;
};
namespace experimental
{
inline  namespace fundamental_v3
{
namespace mixin
{
template <class Currency, class UT1, class UT2>
struct is_compatible_with<money<Currency, UT1>, money<Currency, UT2>> : true_type {};
}
}
namespace meta
{
template <class Currency, class UT1, class UT2>
struct rebind<money<Currency, UT1>, UT2> : id<money<Currency, UT2>> {};
}
}
}

// fixme: Should be Rep, Currency?
// fixme: Should we add a Period parameter to count in Million Dollars, or in cents?
//      Don't having a period, forces almost to work with double.
//      Having a period allows to work with integers most of the time

template <class Currency, class Rep>
struct money
    : stdex::strong_type<money<Currency, Rep>, Rep>, stdex::mixins<money<Currency, Rep>
        , stdex::meta_mixin::additive_with_if<>
        , stdex::meta_mixin::comparable<>
        , stdex::meta_mixin::integer_multiplicative_with<Rep>
        , stdex::meta_mixin::modable<>
        , stdex::meta_mixin::hashable<>
        //, mixin::streamable<>
        //, mixin::ordinal<>
    >

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
    JASEL_CXX14_CONSTEXPR money& operator=(money const& m) = default;
#if 0
    template <class C, class R>
    constexpr money& operator=(money<C, R> m)
    {
        rep = currency::convert<C, Currency>(m.count());
        return *this;
    }
#endif

    constexpr Rep count() const noexcept
        { return this->underlying(); }

    //constexpr Currency currency() const noexcept { return Currency{}; }

    // todo: add zero, min, max

};

namespace money_expr
{
/// helper function that converts a money to a specific currency using the conversion_factor customization point
template <class Currency, class C, class R>
double to_currency(money<C, R> const& m)
{
    return currency::convert<C, Currency>(m.count());
}

template <class M1, class M2, class Op>
struct binary
{
    M1 m1;
    M2 m2;

    // fixme:: shouldn't this require that the representation is convertible from the common_type?
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>() {
        return money<C,R>( Op{}(to_currency<C>(m1), to_currency<C>(m2)));
    }
};

template <class M1, class M2>
struct add
{
    M1 m1;
    M2 m2;

    // fixme:: shouldn't this requires that the representation is convertible from the common_type?
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( to_currency<C>(m1) + to_currency<C>(m2));
    }
};
template <class M1, class M2>
struct substract
{
    M1 m1;
    M2 m2;

    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( to_currency<C>(m1) - to_currency<C>(m2));
    }
};

template <class C, class M1, class M2>
struct divide
{
    M1 m1;
    M2 m2;

    template <class R>
    JASEL_CXX14_CONSTEXPR operator R()
    {
        return money<C,R>( to_currency<C>(m1) / to_currency<C>(m2));
    }
};

template <class M1, class M2>
struct modulo
{
    M1 m1;
    M2 m2;

    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>()
    {
        return money<C,R>( to_currency<C>(m1) % to_currency<C>(m2));
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

