// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

// Should the mixin operations be defined using SFINAE on the underlying operations?
// The advantage of not using SFINAE is compile time
// The liability of not using SFINAE is that we define an invalid operation

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_MIXIN_ADITIVE_HPP

#include <experimental/fundamental/v2/config.hpp>
#include <experimental/fundamental/v3/strong/mixins/is_compatible_with.hpp>
#include <experimental/type_traits.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace mixin
{
    template <class Final, class Other=Final>
    struct strong_type_plus_assign_with
    {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Other const& y) noexcept
        {
            x._backdoor()._underlying() += y.underlying();
            return x;
        }
    };
    template <class Final, class Other=Final>
    struct strong_type_plus_assign_with_other_derived
    {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator+=(Final& x, Other const& y) noexcept
        {
            x = x + y;
            return x;
        }
    };
    template <class Final, class Other=Final, class Result=Final>
    struct strong_type_plus_with
    {
        friend constexpr
        Result operator+(Final const& x, Other const& y) noexcept
        {
            return Result(x.underlying() + y.underlying());
        }
    };
    template <class Final, class Other, class Result=Final>
    struct strong_type_plus_other_with_commut
    {
        friend constexpr
        Result operator+(Other const& x, Final const& y) noexcept
        {
            return y + x;
        }
    };

    template <class Final, class Other, class Result=Final>
    struct addable_with
        : strong_type_plus_assign_with<Final, Other>
        , strong_type_plus_with<Final, Other, Result>
        , strong_type_plus_other_with_commut<Final, Other, Result>
    {};

    template <class Final, class Other=Final>
    struct strong_type_minus_assign_with
    {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Other const& y) noexcept
        {
            x._backdoor()._underlying() -= y.underlying();
            return x;
        }
    };
    template <class Final, class Other=Final>
    struct strong_type_minus_assign_with_other_derived
    {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator-=(Final& x, Other const& y) noexcept
        {
            x = x - y;
            return x;
        }
    };
    template <class Lhs, class Rhs=Lhs, class Result=Lhs>
    struct strong_type_minus_with
    {
        friend constexpr Result operator-(Lhs const& x, Rhs const& y) noexcept
        {
            return Result(x.underlying() - y.underlying());
        }
    };
    template <class Final, class Other>
    struct substractable_with
        : strong_type_minus_assign_with<Final, Other>
        , strong_type_minus_with<Final, Other, Final>
        , strong_type_minus_with<Final, Final, Other>
    {};
    template <class Final>
    struct strong_type_post_increment_derived
    {
        friend JASEL_MUTABLE_CONSTEXPR Final operator++(Final& x, int) noexcept
        {
            Final tmp(x);
            ++x;
            return tmp;
        }
    };
    template <class Final>
    struct unary_plus
    {
        friend constexpr Final operator+(Final const&x) noexcept
        {
            return x;
        }
    };
    template <class Final>
    struct strong_type_pre_increment
    {
        friend JASEL_MUTABLE_CONSTEXPR Final& operator++(Final& x) noexcept
        {
            ++x._backdoor()._underlying();
            return x;
        }
    };
    template <class Final, class Check=no_check>
    struct addable_base
        : strong_type_post_increment_derived<Final>
        , unary_plus<Final>
        , strong_type_pre_increment<Final>
    {};
    template <class Final>
    struct addable_base<Final, void> : strong_type_post_increment_derived<Final>
    {};

    template <class Final>
    struct strong_type_post_decrement_derived
    {
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x, int) noexcept
        {
            Final tmp(x);
            --x;
            return tmp;
        }
    };
    template <class Final, class Check=no_check>
    struct strong_type_unary_minus
    {
        friend constexpr Final operator-(Final const&x) noexcept
        {
            return Final(-x.underlying());
        }
    };
    template <class Final, class Check=no_check>
    struct strong_type_pre_decrement
    {
        friend JASEL_MUTABLE_CONSTEXPR Final operator--(Final& x ) noexcept
        {
            --x._backdoor()._underlying();
            return x;
        }
    };

    template <class Final, class Check=no_check>
    struct substractable_base
        : strong_type_post_decrement_derived<Final>
        , strong_type_unary_minus<Final>
        , strong_type_pre_decrement<Final>
    {};
    template <class Final>
    struct substractable_base<Final, void>
        : strong_type_post_decrement_derived<Final>
    {};

    template <class Final, class Check=no_check>
    struct additive_base: addable_base<Final, Check>
        , substractable_base<Final, Check>
    {};

    template <class Final, class Check=no_check>
    struct addable_assign: addable_base<Final, Check>
        , strong_type_plus_assign_with<Final>
    {};

    //arithmetic assignment when checked relies in the assignment of the result of the arithmetic operation
    template <class Final>
    struct addable_assign<Final, check>: addable_base<Final, check>
        , strong_type_plus_assign_with_other_derived<Final>
    {};
    template <class Final>
    struct addable_assign<Final, void>: addable_base<Final, void>
        , strong_type_plus_assign_with_other_derived<Final>
    {};

    template <class Final, class Check=no_check>
    struct substractable_assign: substractable_base<Final, Check>
        , strong_type_minus_assign_with<Final>
    {};

    //arithmetic assignment when checked relies in the assignment of the result of the arithmetic operation
    template <class Final>
    struct substractable_assign<Final, check>: substractable_base<Final, check>
        , strong_type_minus_assign_with_other_derived<Final>
    {};
    template <class Final>
    struct substractable_assign<Final, void>: substractable_base<Final, void>
        , strong_type_minus_assign_with_other_derived<Final>
    {};

    template <class Final, class Check=no_check>
    struct additive_assign : addable_assign<Final, Check>, substractable_assign<Final, Check>
    {};
    template <class Final, class Check=no_check>
    struct addable: addable_assign<Final, Check>
        , strong_type_plus_with<Final>
    {};
    template <class Final, class Check=no_check>
    struct substractable: substractable_assign<Final, Check>
        , strong_type_minus_with<Final>
    {
    };
    template <class Final, class Check=no_check>
    struct additive: addable<Final, Check>, substractable<Final, Check>
    {};

    // additive heterogeneous
    template <class Final, class Check=no_check, template <class, class> class Pred=is_compatible_with>
    struct additive_with_if : additive_assign<Final, Check>
    {
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        common_type_t<Final, Other> operator+(Final const& x, Other const& y) noexcept
        {
            using CT = common_type_t<Final, Other>;
            return CT(CT(x).underlying() + CT(y).underlying());
        }
        template <class Other, typename = enable_if_t<Pred<Final, Other>::value>>
        friend constexpr
        common_type_t<Final, Other> operator-(Final const& x, Other const& y) noexcept
        {
            using CT = common_type_t<Final, Other>;
            return CT(CT(x).underlying() - CT(y).underlying());
        }
    };
}
namespace meta_mixin
{
    template <class Check=mixin::no_check, template <class, class> class Pred=mixin::is_compatible_with>
    struct additive_with_if
    {
        template <class Final>
        using type = mixin::additive_with_if<Final, Check, Pred>;
    };

    struct strong_type_post_increment_derived
    {
        template <class Final>
        using type = mixin::strong_type_post_increment_derived<Final>;

    };

    struct strong_type_post_decrement_derived
    {
        template <class Final>
        using type = mixin::strong_type_post_decrement_derived<Final>;

    };

}
}
}
}

#endif // header
