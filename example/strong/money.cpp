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

namespace currency {

struct dollar
{
    constexpr const char* symbol() const noexcept { return "$"; }
    constexpr const char* code() const noexcept { return "USD"; }

    template <class OSTREAM>
    friend OSTREAM& operator<<(OSTREAM& os, dollar c)
    {
        os<< c.symbol();
        return os;
    }
};

struct euro
{
    constexpr const char* symbol() const noexcept { return "$"; }
    constexpr const char* code() const noexcept { return "EUR"; }

    template <class OSTREAM>
    friend OSTREAM& operator<<(OSTREAM& os, euro c)
    {
        os<< c.symbol();
        return os;
    }
};

///customization point for currency conversions
/// same currency => 1.0
template <class C>
double conversion_factor(C , C )
{
    return 1;
}

/// specific
double conversion_factor(dollar , euro )
{
    return 2;
}
/// Not found
template <class From, class To>
double conversion_factor(From , To )
{
    return 1/conversion_factor(To{}, From{});
}

}


template <class Currency, class Rep = double>
struct money;

namespace detail
{
/// helper function that converts a money to a specific currency using the conversion_factor customization point
template <class Currency, class C, class R>
double to_currency(money<C, R> const& m)
{
    return m.value() * currency::conversion_factor(m.currency(), Currency{});
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
    constexpr money& operator=(money const& m) = default;
#if 0
    template <class C, class R>
    constexpr money& operator=(money<C, R> m)
    {
        rep = detail::to_currency<Currency>(m);
        return *this;
    }
#endif

    // fixme: Should we replace it by count as duration does?
    constexpr Rep value() const noexcept { return rep; }
    constexpr Currency currency() const noexcept { return Currency{}; }

    // todo: add zero, min, max


    friend constexpr bool operator==(money m1, money m2) noexcept
    {
        return int(m1.value() - m2.value()) == 0 ;
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
        return money(m1.value() + m2.value()) ;
    }

    constexpr money& operator++() noexcept
    {
        rep++;
        return *this ;
    }
    constexpr money operator++(int) noexcept
    {
        money tmp = *this;
        rep++;
        return tmp ;
    }
    constexpr money& operator+=(money m) noexcept
    {
        rep += m.value();
        return *this ;
    }

    friend constexpr money operator-(money m1, money m2) noexcept
    {
        return money(m1.value() - m2.value()) ;
    }
    friend constexpr money operator-(money m1) noexcept
    {
        return money(- m1.value()) ;
    }
    constexpr money& operator--() noexcept
    {
        rep--;
        return *this ;
    }
    constexpr money operator--(int) noexcept
    {
        money tmp = *this;
        rep--;
        return tmp ;
    }
    constexpr money& operator-=(money m) noexcept
    {
        rep -= m.value();
        return *this ;
    }

    friend constexpr money operator*(money m, double x) noexcept
    {
        return money(m.value() * x) ;
    }
    friend constexpr money operator*(double x, money m) noexcept
    {
        return m * x;
    }

    friend constexpr money operator/(money m, double x) noexcept
    {
        return money(m.value() / x) ;
    }
    constexpr money& operator/=(double x) noexcept
    {
        rep /= x;
        return *this;
    }
    friend constexpr money operator%(money m, int s) noexcept
    {
        using CR = std::common_type_t<Rep, int>;
        auto r = CR(m.value()) % s ;
        return money(r) ;
    }
    template <class R1, class R2>
    friend constexpr money<Currency, std::common_type_t<R1,R2>> operator%(money<Currency, R1> m, money<Currency, R2> n) noexcept
    {
        return money(m.value() % n.value()) ;
    }
    constexpr money& operator%=(int x) noexcept
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
    constexpr operator money<C,R>() {
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
    constexpr operator money<C,R>()
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
    constexpr operator money<C,R>()
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
    constexpr operator R()
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
    constexpr operator money<C,R>()
    {
        return money<C,R>( detail::to_currency<C>(m1) % detail::to_currency<C>(m2));
    }
};

}


template <class C1, class R1, class C2, class R2>
constexpr auto operator+(money<C1,R1> m1, money<C2,R2> m2) noexcept
{
    return money_expr::add<money<C1,R1>, money<C2,R2>>{m1, m2} ;
    //return money_expr::binary<money<C1,R1>, money<C2,R2>, std::plus>{m1, m2} ;
}

template <class C1, class R1, class C2, class R2>
constexpr auto operator-(money<C1,R1> m1, money<C2,R2> m2) noexcept
{
    return money_expr::substract<money<C1,R1>, money<C2,R2>>{m1, m2} ;
}

template <class OSTREAM, class C, class R>
OSTREAM& operator<<(OSTREAM& os, money<C, R> m)
{
    os<< m.value() << m.currency().symbol();
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
        BOOST_TEST(long(d.value()) == 5);
    }
    {
        dollars d;
        d = dollars(5);
        BOOST_TEST(long(d.value()) == 5);
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

