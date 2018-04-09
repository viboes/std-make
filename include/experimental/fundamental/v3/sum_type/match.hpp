//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_SUM_TYPE_MATCH_HPP
#define JASEL_FUNDAMENTAL_V3_SUM_TYPE_MATCH_HPP

#include <experimental/fundamental/v3/functional/overload.hpp>
#include <experimental/fundamental/v3/product_type/apply.hpp>
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

    //using meta::types;
    using meta::id;

#if ! defined JASEL_DOXYGEN_INVOKED
    namespace sum_type_detail
    {
      /**
       * Result: The result type of the overload
       * Functor: The nullary void function to call when there are no more alternatives in Types
       * DataTypeTuple: the product type of the current alternative of the sum types already visited
       * Sts: the product type of the sum types not yet visited
       * Types: the alternatives for the ST been visited
       *
       * Defines the operator(T const&) for each T in Types. This is the function called by match custom for ST
       */
      template <class Result, class Functor, class DataTypeTuple, class ST, class STs, class Ns>
      class applier;

      /**
       * Stores:
       * * where to store the result,
       * * the nullary void function to call when there are no more alternatives in Types,
       * * the product type of the reference to the current alternative of the sum types already visited
       */
      template <class R, class F, class ... DTs, class ST, class ... STs>
      class applier<R, F, tuple<DTs const&...>, ST, tuple<STs const&...>, index_sequence<> >
      {
      public:


        applier(R *r_, F &fct_, tuple<DTs const&...> &&members_,
            tuple<STs const&...> &&sums_)
        : fct(fct_),
          r(r_),
          members(move(members_)),
          sums(move(sums_))
        {}

        F &fct;
        R *r;
        tuple<DTs const&...> members;
        tuple<STs const&...> sums;

        // only here to make it possible in the derived class
        // using super::operator();
        void operator()() const
        {
        }
      };

      /**
       * Defines the operator(T const&). This is the function called by match custom for ST.
       * Inherits from the applier that will define the other operator(U) for U in Ts.
       */
      template <class R, class F, class... DTs, class ST, class... STs, size_t N, size_t... Ns>
      class applier<R, F, tuple<DTs const&...>, ST, tuple<STs const&...>, index_sequence<N, Ns...>>
      : public applier<R, F, tuple<DTs const&...>, ST, tuple<STs const&...>, index_sequence<Ns...>>
      {
      public:
        using T = sum_type::alternative_t<N, ST>;
        using super = applier<R, F, tuple<DTs const&...>, ST, tuple<STs const&...>, index_sequence<Ns...>>;

        /* Pass everything up to the base case. */
        applier(R *r, F &fct, tuple<DTs const&...> &&members, tuple<STs const&...> &&sums)
          : super(r, fct, move(members), move(sums))
        {}

        // make visible the other operators(U)
        using super::operator();

        void operator()(T const&v) const
        {
          dispatch(index_sequence_for<DTs...>(), v, index_sequence_for<STs...>());
        }

      private:
        /**
         * odts: the product of all the current references
         */
        template <class... ODTs>
        void dispatch_helper(
            tuple<ODTs const&...> &&odts) const
        {
          overload<R>(
              [&](auto *r)
              {
                *r = apply(this->fct, move(odts));
              },
              [&](void *)
              {
                apply(this->fct, move(odts));
              }
          )(this->r);
        }

        /**
         * odts: the product of all the current references up to sum type ST
         * sum, ostd... the sum types not yet visited
         */
        template <class... ODTs, class OST, class... OSTs>
        void dispatch_helper(tuple<ODTs const&...> &&odts, OST const& sum, OSTs const&... osts) const
        {
          // nested applier
          using applier_type = applier<R, F, tuple<ODTs const&...>, OST, tuple<OSTs const&...>,
              make_integer_sequence<size_t, sum_type::size<OST>::value >>;

          // customization point
#if ! defined YAFPL_X1
#error
          match(id<R>{}, sum, applier_type(this->r, this->fct, move(odts), forward_as_tuple(osts...)));
#else
          sum_type::visit(
              applier_type(this->r, this->fct, move(odts), forward_as_tuple(osts...)),
              sum
              );
#endif
        }

        /**
         * This function is just a helper to manage with the indexes
         */
        template <size_t... i, size_t... j>
        void dispatch(index_sequence<i...>, T const& v, index_sequence<j...>) const
        {
          dispatch_helper(forward_as_tuple(std::get<i>(this->members)..., v), std::get<j>(this->sums)...);
        }
      };

      template <class R>
      struct storage
      {
        R get()
        { return move(r);}
        R *ptr()
        { return &r;}
        R r;
      };

      template <>
      struct storage<void>
      {
        void get() &&
        {}
        void *ptr()
        { return nullptr; }
      };

      /**
       * R: the result type
       * F the function object
       * ST: a sum type
       * STs: other sum types
       *
       * @effect calls to the match customization point for @c ST with a function object wrapping @c f and the other sum types @c osts.
       * This function object will end by forwarding the call to @c f with the stored alternative for each one of the @c st/@c sts.
       */
      template <class R, class F, class ST, class... STs>
      decltype(auto) apply_impl(F &&fct, ST const& sum, STs const&... osts)
      {
        using applier_type = applier<R, F, tuple<>, ST, tuple<STs const&...>,
            make_integer_sequence<size_t, sum_type::size<ST>::value> >;

        storage<R> r;
        // customization point
#if ! defined YAFPL_X1
        match(id<R>{}, sum, applier_type(r.ptr(), fct, forward_as_tuple(), forward_as_tuple(osts...)));
#else
        sum_type::visit(applier_type(r.ptr(), fct, forward_as_tuple(), forward_as_tuple(osts...)), sum);
#endif
        return move(r).get();
      }

      // fixme: Shouldn't use is_product_type<T>?
      template <class T>
      struct is_tuple : false_type {};
      template <class T>
      struct is_tuple<T const> : is_tuple<T> {};
      template <class T>
      struct is_tuple<T volatile> : is_tuple<T> {};
      template <class T>
      struct is_tuple<T const volatile> : is_tuple<T> {};
      template <class ...Ts>
      struct is_tuple<tuple<Ts...>> : true_type {};

      template< class, class = void >
      struct has_result_type_member : false_type { };
      template< class T >
      struct has_result_type_member<T, void_t<typename T::result_type>> : true_type { };

      template< class T >
      struct have_result_type_member_x;
      template< template <class...> class TMPL, class T >
      struct have_result_type_member_x<TMPL<T>> : has_result_type_member<T> {};
      template< template <class...> class TMPL, class T1, class T2, class ...Ts >
      struct have_result_type_member_x<TMPL<T1, T2, Ts...>>
        : conjunction<
            has_result_type_member<T1>,
            have_result_type_member_x<TMPL<T2, Ts...>>
          >
      {};

      template<class ...Ts >
      struct have_result_type_member : have_result_type_member_x<meta::types<Ts...>> { };
      // fixme. Why the following doesn't work?
      //template<class ...Ts >
      //struct have_result_type_member : conjunction<have_result_type_member<Ts...>> { };

      template <class F, class ...Fs>
      struct deduced_result_type {
        using type =  typename F::result_type;
      };
      template <class ...Fs>
      using deduced_result_type_t = typename deduced_result_type<Fs...>::type;
    } // sum_type_detail
#endif

    // explicit result type
    template <class R, class ST, class F1, class F2, class... Fs
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      ! sum_type_detail::is_tuple<ST>::value, int> = 0
#endif
      >
    decltype(auto) match(ST const& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
#if ! defined YAFPL_X1
      return match(id<R>{}, that, overload(forward<F1>(f1), forward<F2>(f2), forward<Fs>(fs)...));
#else
      return sum_type::visit(overload(forward<F1>(f1), forward<F2>(f2), forward<Fs>(fs)...), that);
#endif
    }
    template <class R, class ST, class F1, class... Fs
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      ! sum_type_detail::is_tuple<ST>::value, int> = 0
#endif
      >
    decltype(auto) match(ST & that, F1 && f1, Fs &&... fs)
    {
#if ! defined YAFPL_X1
      return match(id<R>{}, that, overload(forward<F1>(f1), forward<Fs>(fs)...));
#else
      return sum_type::visit(overload(forward<F1>(f1), forward<Fs>(fs)...), that);
#endif
    }

    // explicit result type on a product of sum types
    template <class R, class... STs, class... Fs>
    decltype(auto) match(tuple<STs...> const& those, Fs &&... fcts)
    {
      return apply(
          [&](auto && ... args) -> decltype(auto)
          {
            return sum_type_detail::apply_impl<R>(overload(forward<Fs>(fcts)...), forward<decltype(args)>(args)...);
          },
          those);
    }

    // result type deduced the nested typedef result_type of all functions
    template <class ST, class F1, class F2, class... Fs
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      ! sum_type_detail::is_tuple<ST>::value
      && sum_type_detail::have_result_type_member<F1>::value, int> = 0
#endif
      >
    decltype(auto) match(ST const& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
      using R = sum_type_detail::deduced_result_type_t<decay_t<F1>, decay_t<F2>, decay_t<Fs>...>;
      return match<R>(that, forward<F1>(f1), forward<F2>(f2), forward<Fs>(fs)...);
    }
    template <class ST, class F1, class F2, class... Fs
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      ! sum_type_detail::is_tuple<ST>::value
      && sum_type_detail::have_result_type_member<F1>::value, int> = 0
#endif
      >
    decltype(auto) match(ST& that, F1 && f1, F2 && f2, Fs &&... fs)
    {
      using R = sum_type_detail::deduced_result_type_t<decay_t<F1>, decay_t<F2>, decay_t<Fs>...>;
      return match<R>(that, forward<F1>(f1), forward<F2>(f2), forward<Fs>(fs)...);
    }

    // result type deduced the nested typedef result_type of all functions on a product of sum types
    template <class... STs, class F, class... Fs
#if ! defined JASEL_DOXYGEN_INVOKED
    , enable_if_t<
      sum_type_detail::have_result_type_member<F>::value, int> = 0
#endif
      >
    decltype(auto) match(tuple<STs...> const& those, F && f, Fs &&... fs)
    {
      using R = sum_type_detail::deduced_result_type_t<decay_t<F>, decay_t<Fs>...>;
      return match<R>(those, forward<F>(f), forward<Fs>(fs)...);
    }
}
}}}

#endif // header
