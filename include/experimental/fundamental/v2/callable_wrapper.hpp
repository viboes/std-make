// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V2_CALLABLE_WRAPPER_HPP
#define JASEL_FUNDAMENTAL_V2_CALLABLE_WRAPPER_HPP

#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{


template <class C, class Sig>
struct callable_wrapper;

template <class C, class R, class ...Args>
struct callable_wrapper<C, R(Args...)>
{
    callable_wrapper(C c) : fct(c) {}
    //R operator()(Args ...args) {
    //    return R(fct(args...));
    //}
    R operator()(Args ...args) const {
        return R(fct(args...));
    }
private:
    C fct;
};


template <class ...Args, class C>
auto make_callable_wrapper(C c) -> decltype(callable_wrapper<C, std::result_of_t<C(Args...)>(Args...)>(c))
{
    return callable_wrapper<C, std::result_of_t<C(Args...)>(Args...)>(c);
}


}
}
}

template <class C>
using XX = std::experimental::callable_wrapper<C, int(int)>;

#endif // header
