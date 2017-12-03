// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_NON_ZERO_INTEGER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_NON_ZERO_INTEGER_HPP

#include <cassert>

#if __cplusplus > 201402L && defined __clang__
#include <optional>
#endif

struct non_zero_integer
{
public:
    using underlying_type = int;

#if __cplusplus > 201402L && defined __clang__
    // safe construction
    static auto make( int v ) -> std::optional<non_zero_integer> {
        if (v == 0) return std::nullopt;
        return non_zero_integer(v);
    }
#endif

    // unsafe construction
    static auto cast( int v ) -> non_zero_integer {
        assert (v != 0 && "v cannot be 0");
        return non_zero_integer(v);
    }

    // access to underlying type
    operator underlying_type() const { return underlying(); }
    underlying_type underlying() const { return _value; }

    friend auto operator+(non_zero_integer x) -> non_zero_integer
    {
         return x;
    }

    friend auto operator-(non_zero_integer x) -> non_zero_integer
    {
         return non_zero_integer( -x.underlying() );
    }

    friend auto operator+(non_zero_integer x, non_zero_integer y) -> non_zero_integer
    {
         return non_zero_integer( x.underlying() + y.underlying() );
    }
    // implicit non_zero_integer + int -> int
    // implicit int + non_zero_integer  -> int

    non_zero_integer& operator+=(non_zero_integer x) {
        _value += x.underlying();
        return *this;
    }

    // implicit auto operator-(non_zero_integer x, non_zero_integer y) -> int;
    // implicit non_zero_integer - int -> int
    // implicit int - non_zero_integer  -> int

    non_zero_integer& operator-=(non_zero_integer) = delete;


    friend auto operator*(non_zero_integer x, non_zero_integer y) -> non_zero_integer
    {
         return non_zero_integer( x.underlying() * y.underlying() );
    }
    // implicit non_zero_integer * int -> int
    // implicit int * non_zero_integer -> int

    non_zero_integer& operator*=(non_zero_integer x) {
        _value *= x.underlying();
        return *this;
    }

    // while the implicit implementation is the same it is here to state clearly that the division is safe
    friend auto operator/(int x, non_zero_integer y) -> int
    {
        return x / y.underlying();
    }
    // implicit non_zero_integer / non_zero_integer -> int

    non_zero_integer& operator/=(non_zero_integer) = delete;

    //implicit or deleted
    //friend auto operator/(non_zero_integer, int) = delete;

    // implicit int / non_zero_integer -> int

    // implicit non_zero_integer % non_zero_integer -> int

    non_zero_integer& operator%=(non_zero_integer) = delete;

    // implicit non_zero_integer % int -> int
    // implicit non_zero_integer % int -> int


private:
    explicit non_zero_integer(int v) : _value(v) {}
    underlying_type _value;
};

#if __cplusplus > 201402L && defined __clang__
using opt_non_zero_integer = std::optional<non_zero_integer>;
#endif
// implicit
//template <class OSTREAM>
//OSTREAM& operator<<(OSTREAM& os, non_zero_integer x);


// this function is not provided because we don't want to manage with exceptions for non_zero_integer
//   template <class ISTREAM>
//   ISTREAM& operator>>(ISTREAM& is, non_zero_integer& nzi) = delete;
// The user should do the following
//   int i;
//   os >> i;
//   auto onzi = non_zero_integer::make(i);
// We could as well define
//   template <class ISTREAM>
//   ISTREAM& operator>>(ISTREAM& os, optional<non_zero_integer>& onzi);
//
//   optional<non_zero_integer> onzi;
//   os >> onzi;

#if __cplusplus > 201402L && defined __clang__
template <class ISTREAM>
ISTREAM& operator>>(ISTREAM& is, std::optional<non_zero_integer>& onzi)
{
    int i;
    is >> i;
    onzi = non_zero_integer::make(i);
    return is;
}
#endif












#endif
