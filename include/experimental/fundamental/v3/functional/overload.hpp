//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_FUCTIONAL_V1_OVERLOAD_HPP
#define YAFPL_FUCTIONAL_V1_OVERLOAD_HPP

#if __cplusplus >= 201402L

#include <experimental/fundamental/v2/config.hpp>

#include <functional>
#include <type_traits>
#include <utility>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

    namespace detail
    {
      template <class F, bool IsFinal=is_class<F>::value && is_final<F>::value>
      struct forwarder : F
      {
        using type = F;
        using F::operator();
        constexpr forwarder(F fct) : F(move(fct))
        {}
      };
      template< class F>
      struct wrap_call {
          using type = F;
          type f;

          template<typename G,
                   /* Make sure that it does not act as copy constructor for wrap_call& */
                   enable_if_t<!is_base_of<wrap_call, decay_t<G>>::value
                   /* require that F can be constructed from U&& */
                   && is_constructible<F, G>::value, bool> = true>
          constexpr wrap_call(G&& fct) : f(forward<G>(fct)) { }

          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &
            noexcept(noexcept( f(forward<Xs>(xs)...) ))
            -> decltype(f(forward<Xs>(xs)...))
          {
              return f(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &&
            noexcept(noexcept( move(f)(forward<Xs>(xs)...) ))
            -> decltype(move(f)(forward<Xs>(xs)...))
          {
              return move(f)(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &
            noexcept(noexcept( f(forward<Xs>(xs)...) ))
            -> decltype(f(forward<Xs>(xs)...))
          {
              return f(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &&
            noexcept(noexcept( move(f)(forward<Xs>(xs)...) ))
            -> decltype(move(f)(forward<Xs>(xs)...))
          {
              return move(f)(forward<Xs>(xs)...);
          }
      };
      template< class F>
      struct forwarder<F, true>
#if 0

      : wrap_call<F>
      {
        template<typename G,
                 /* Make sure that it does not act as copy constructor for wrap_call& */
                 enable_if_t<!is_base_of<forwarder, decay_t<G>>::value
                 /* require that F can be constructed from U&& */
                 && is_constructible<F, G>::value, bool> = true>
        constexpr forwarder(G&& fct) : wrap_call<F>(forward<G>(fct)) { }
      };
#else
      {
          using type = F;
          type f;

          template<typename G,
                   /* Make sure that it does not act as copy constructor for wrap_call& */
                   enable_if_t<!is_base_of<forwarder, decay_t<G>>::value
                   /* require that F can be constructed from U&& */
                   && is_constructible<F, G>::value, bool> = true>
          constexpr forwarder(G&& fct) : f(forward<G>(fct)) { }

          //template <class FF>
          //constexpr forwarder(FF&& fct) : f(move(fct)) { }

          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &
            noexcept(noexcept( f(forward<Xs>(xs)...) ))
            -> decltype(f(forward<Xs>(xs)...))
          {
              return f(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &&
            noexcept(noexcept( move(f)(forward<Xs>(xs)...) ))
            -> decltype(move(f)(forward<Xs>(xs)...))
          {
              return move(f)(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &
            noexcept(noexcept( f(forward<Xs>(xs)...) ))
            -> decltype(f(forward<Xs>(xs)...))
          {
              return f(forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &&
            noexcept(noexcept( move(f)(forward<Xs>(xs)...) ))
            -> decltype(move(f)(forward<Xs>(xs)...))
          {
              return move(f)(forward<Xs>(xs)...);
          }
      };
#endif
      template< class R, class ...X>
      struct forwarder<R(*)(X...), false> {
          using type = R(*)(X...);
          type f;

          constexpr forwarder(type f) : f(f) { }

          constexpr R operator () (X &&... x) const noexcept(noexcept( f(forward<X>(x)...) ))
          {
              return f(forward<X>(x)...);
          }
      };

      template< class R, class ...X >
      struct forwarder<R(&)(X...), false> {
          using type = R(&)(X...);
          type f;

          constexpr forwarder(type f) : f(f) { }

          constexpr R operator () (X &&... x) const noexcept(noexcept( f(forward<X>(x)...) ))
          {
              return f(forward<X>(x)...);
          }
      };

      template<class R, class O, class...X>
      struct forwarder<R(O::*)(X...), false> : forwarder<decltype(mem_fn(declval<R(O::*)(X...)>())), false> {
        using base = forwarder<decltype(mem_fn(declval<R(O::*)(X...)>())), false>;
        using type = R(O::*)(X...);

        constexpr forwarder(type f) : base(mem_fn(f)) { }
        using base::operator();
      };

      template <class R, class F>
      struct explicit_forwarder : forwarder<F>
      {
        using result_type = R;
        using forwarder<F>::operator();
        constexpr explicit_forwarder(F fct) : forwarder<F>(move(fct))
        {}
      };

      template <class F, class ...Fs>
      struct overloader;

      template <class F>
      struct overloader<F> : forwarder<F>
      {
        template <class OF>
        constexpr overloader(OF&& of)
        : forwarder<F>(forward<OF>(of))
        {}

        using forwarder<F>::operator();
      };

      template <class F1, class F2, class ...Fs>
      struct overloader<F1, F2, Fs...>
      : forwarder<F1>, overloader<F2, Fs...>
      {
        template <class OF1, class OF2, class ...OFs>
        constexpr overloader(OF1&& of1, OF2&& of2, OFs&& ...ofs)
        : forwarder<F1>(forward<OF1>(of1)), overloader<F2, Fs...>(forward<OF2>(of2), forward<OFs>(ofs)...)
        {}

        using forwarder<F1>::operator();
        using overloader<F2, Fs...>::operator();
      };

      ///
      template <class R, class ...Fs>
      struct explicit_overloader : overloader<Fs...>
      {
        using result_type = R;
        using overloader<Fs...>::operator();

        template <class ...OFs>
        constexpr explicit_overloader(OFs&& ...fs) : overloader<Fs...>(forward<OFs>(fs)...) {}
      };

    } // detail

    template <class F>
    constexpr auto overload(F && f) noexcept(noexcept( detail::forwarder<decay_t<F>>(forward<F>(f)) ))
    {
      return detail::forwarder<decay_t<F>>(forward<F>(f));

    }

    template <class F1, class F2, class ... Fs>
    constexpr auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return detail::overloader<decay_t<F1>,decay_t<F2>, decay_t<Fs>...>(
          forward<F1>(f1), forward<F2>(f2), forward<Fs>(fcts)...);
    }

    template <class R, class F>
    constexpr auto overload(F && f) noexcept(noexcept( detail::explicit_forwarder<R, decay_t<F>>(forward<F>(f)) ))
    {
      return detail::explicit_forwarder<R, decay_t<F>>(forward<F>(f));
    }

    template <class R, class F1, class F2, class ... Fs>
    constexpr auto overload(F1 && f1, F2 && f2, Fs &&... fcts)
    {
      return detail::explicit_overloader<R, decay_t<F1>,decay_t<F2>, decay_t<Fs>...>(
          forward<F1>(f1), forward<F2>(f2), forward<Fs>(fcts)...);
    }

}}}
#endif
#endif // header
