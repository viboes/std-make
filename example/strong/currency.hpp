// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MONEY_CURRENCY_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MONEY_CURRENCY_HPP

//#include <functional>
//#include <type_traits>

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
double conversion_factor(euro , dollar )
{
    return 1/2;
}
/// Not found
//template <class From, class To>
//double conversion_factor(From , To )
//{
//    static_assert(false, "No know conversion from currencies");
//    return 0;
//}

template <class FromCurrency, class ToCurrency, class R>
double convert(R r)
{
    return r * currency::conversion_factor(FromCurrency{}, ToCurrency{});
}

}

#endif
