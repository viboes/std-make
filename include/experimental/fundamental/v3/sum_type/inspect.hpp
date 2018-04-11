//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2018.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_SUM_TYPE_INSPECT_HPP
#define JASEL_FUNDAMENTAL_V3_SUM_TYPE_INSPECT_HPP

#include <experimental/fundamental/v3/product_type/apply.hpp>
#include <experimental/fundamental/v3/sum_type/match.hpp>
#include <experimental/fundamental/v3/sum_type/sum_type.hpp>
#include <experimental/meta.hpp>
#include <utility>
#include <experimental/type_traits.hpp>
#include <experimental/tuple.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace sum_type
{
    // explicit result type on a product of sum types
    template <class R, class... STs, class F>
    decltype(auto) visit(F && f, STs&& ...sts)
    {
            return sum_type_detail::apply_impl<R>(forward<F>(f), forward<STs>(sts)...);
    }

    // result type deduced the nested typedef result_type of all functions on a product of sum types
    template <class... STs, class F
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      sum_type_detail::have_result_type_member<F>::value, int> = 0
#endif
      >
    decltype(auto) visit(F && f, STs&& ...sts)
    {
      using R = sum_type_detail::deduced_result_type_t<decay_t<F>>;
      return visit<R>(forward<F>(f), forward<STs>(sts)...);
    }

    // explicit result type on a product of sum types
    template <class R, class... STs, class F>
    decltype(auto) match2(tuple<STs...> const& those, F && f)
    {
      return apply(
          [&](auto && ... args) -> decltype(auto)
          {
            return sum_type::visit<R>(forward<F>(f), forward<decltype(args)>(args)...);
          },
          those);
    }

    // result type deduced the nested typedef result_type of all functions on a product of sum types
    template <class... STs, class F
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      sum_type_detail::have_result_type_member<F>::value, int> = 0
#endif
      >
    decltype(auto) match2(tuple<STs...> const& those, F && f)
    {
      using R = sum_type_detail::deduced_result_type_t<decay_t<F>>;
      return match2<R>(those, forward<F>(f));
    }

    template <class ...STs>
    class inspector
    {
        std::tuple<STs...> storage;

    public:
        template <class ...Ts>
        inspector(Ts&& ...ts) : storage(std::forward<Ts>(ts)...) {};

        template <class F>
        auto match(F&& f)
        {
            return sum_type::match2(storage, std::forward<F>(f));
        }

        template <class F>
        auto operator|(F&& f)
        {
            return sum_type::match2(storage, std::forward<F>(f));
        }

        template <class F>
        auto operator()(F&& f)
        {
            return sum_type::match2(storage, std::forward<F>(f));
        }
        template <class R, class F>
        R match(F&& f)
        {
            return sum_type::match2<R>(storage, std::forward<F>(f));
        }
    };

    template <class ...Ts>
    inspector<decay_t<Ts>...> inspect(Ts&&...ts)
    {
        return inspector<decay_t<Ts>...>(std::forward<Ts>(ts)...);
    }


}
}}}

#endif // header
