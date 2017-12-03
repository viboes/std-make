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

#include <functional>
#include <type_traits>
#include "currency.hpp"
#include <experimental/fundamental/v2/config.hpp>

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

// fixme: Should be Rep, Currency?
// fixme: Should we add a Period parameter to count in Million Dollars, or in cents?
//      Don't having a period , forces almost to work with double.
//      Having a period allows to work with integers most of the time

template <class Currency, class Rep>
struct money
{
    money() = default;

    // todo: Add template <class Rep2> as duration
    explicit constexpr money(Rep v) : rep(v) {}


    constexpr money(money const& m) = default;

    // fixme: Should we restrict it as do duration and require a money_cast?
    // or just define an explicit constructor as does pair.
    template <class C, class R>
    constexpr money(money<C, R> m)
    : rep (detail::to_currency<Currency>(m))
    {
    }
    JASEL_CXX14_CONSTEXPR money& operator=(money const& m) = default;
#if 0
    template <class C, class R>
    constexpr money& operator=(money<C, R> m)
    {
        rep = detail::to_currency<Currency>(m);
        return *this;
    }
#endif

    // fixme: Should we replace it by count as duration does?
    constexpr Rep count() const noexcept { return rep; }
    constexpr Currency currency() const noexcept { return Currency{}; }

    // todo: add zero, min, max


    friend constexpr bool operator==(money m1, money m2) noexcept
    {
        return m1.count() == m2.count();
    }
    friend constexpr bool operator!=(money m1, money m2) noexcept
    {
        return ! (m1 == m2);
    }
    friend constexpr money operator+(money m1) noexcept
    {
        return m1 ;
    }
    friend constexpr money operator+(money m1, money m2) noexcept
    {
        return money(m1.count() + m2.count()) ;
    }

    JASEL_CXX14_CONSTEXPR money& operator++() noexcept
    {
        rep++;
        return *this ;
    }
    JASEL_CXX14_CONSTEXPR money operator++(int) noexcept
    {
        money tmp = *this;
        rep++;
        return tmp ;
    }
    JASEL_CXX14_CONSTEXPR money& operator+=(money m) noexcept
    {
        rep += m.count();
        return *this ;
    }

    friend constexpr money operator-(money m1, money m2) noexcept
    {
        return money(m1.count() - m2.count()) ;
    }
    friend constexpr money operator-(money m1) noexcept
    {
        return money(- m1.count()) ;
    }
    JASEL_CXX14_CONSTEXPR money& operator--() noexcept
    {
        rep--;
        return *this ;
    }
    JASEL_CXX14_CONSTEXPR money operator--(int) noexcept
    {
        money tmp = *this;
        rep--;
        return tmp ;
    }
    JASEL_CXX14_CONSTEXPR money& operator-=(money m) noexcept
    {
        rep -= m.count();
        return *this ;
    }

    friend constexpr money operator*(money m, double x) noexcept
    {
        return money(m.count() * x) ;
    }
    friend constexpr money operator*(double x, money m) noexcept
    {
        return m * x;
    }

    friend constexpr money operator/(money m, double x) noexcept
    {
        return money(m.count() / x) ;
    }
    JASEL_CXX14_CONSTEXPR money& operator/=(double x) noexcept
    {
        rep /= x;
        return *this;
    }
    friend JASEL_CXX14_CONSTEXPR money operator%(money m, int s) noexcept
    {
        using CR = typename std::common_type<Rep, int>::type;
        auto r = CR(m.count()) % s ;
        return money(r) ;
    }
#if defined __clang__
    // gcc doesn't accepts this
    template <class R1, class R2>
    friend constexpr money<Currency, typename std::common_type<R1,R2>::type> operator%(money<Currency, R1> m, money<Currency, R2> n) noexcept
    {
        return money(m.count() % n.count()) ;
    }
#endif
    JASEL_CXX14_CONSTEXPR money& operator%=(int x) noexcept
    {
        rep %= x;
        return *this;
    }

private:
    Rep rep;
};

namespace money_expr
{
template <class M1, class M2, class Op>
struct binary
{
    M1 m1;
    M2 m2;

    // fixme:: shouldn't this requires that the representation is convertible from the common_type?
    template <class C, class R>
    JASEL_CXX14_CONSTEXPR operator money<C,R>() {
        return money<C,R>( Op{}(detail::to_currency<C>(m1), detail::to_currency<C>(m2)));
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
        return money<C,R>( detail::to_currency<C>(m1) + detail::to_currency<C>(m2));
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
        return money<C,R>( detail::to_currency<C>(m1) - detail::to_currency<C>(m2));
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
        return  detail::to_currency<C>(m1) / detail::to_currency<C>(m2);
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
        return money<C,R>( detail::to_currency<C>(m1) % detail::to_currency<C>(m2));
    }
};

}


template <class C1, class R1, class C2, class R2>
constexpr auto operator+(money<C1,R1> m1, money<C2,R2> m2) noexcept -> money_expr::add<money<C1,R1>, money<C2,R2>>
{
    return money_expr::add<money<C1,R1>, money<C2,R2>>{m1, m2} ;
    //return money_expr::binary<money<C1,R1>, money<C2,R2>, std::plus>{m1, m2} ;
}

template <class C1, class R1, class C2, class R2>
constexpr auto operator-(money<C1,R1> m1, money<C2,R2> m2) noexcept -> money_expr::substract<money<C1,R1>, money<C2,R2>>
{
    return money_expr::substract<money<C1,R1>, money<C2,R2>>{m1, m2} ;
}

template <class OSTREAM, class C, class R>
OSTREAM& operator<<(OSTREAM& os, money<C, R> m)
{
    os<< m.count() << m.currency().symbol();
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

