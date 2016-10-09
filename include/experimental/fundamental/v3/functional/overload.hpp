//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef YAFPL_FUCTIONAL_V1_FUNCTIONAL_OVERLOAD_HPP
#define YAFPL_FUCTIONAL_V1_FUNCTIONAL_OVERLOAD_HPP

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
      template< class F>
      struct wrap_call {
          using type = F;
          type f;

          /* Perfect forwardign constructor */
          template<typename U,
                   /* Make sure that it does not act as copy constructor for wrap_call& */
                   std::enable_if_t<!std::is_base_of<wrap_call, std::decay_t<U>>::value
                   /* require that F can be constructed from U&& */
                   && std::is_constructible<F, U>::value, bool> = true>
          constexpr wrap_call(U&& fct) : f(std::forward<U>(fct)) { }

          /* You may want to use std::invoke to handle member pointers here */
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &
            noexcept(noexcept( f(std::forward<Xs>(xs)...) ))
            -> decltype(f(std::forward<Xs>(xs)...))
          {
              return f(std::forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) const &&
            noexcept(noexcept( std::move(f)(std::forward<Xs>(xs)...) ))
            -> decltype(std::move(f)(std::forward<Xs>(xs)...))
          {
              return std::move(f)(std::forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &
            noexcept(noexcept( f(std::forward<Xs>(xs)...) ))
            -> decltype(f(std::forward<Xs>(xs)...))
          {
              return f(std::forward<Xs>(xs)...);
          }
          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &&
            noexcept(noexcept( std::move(f)(std::forward<Xs>(xs)...) ))
            -> decltype(std::move(f)(std::forward<Xs>(xs)...))
          {
              return std::move(f)(std::forward<Xs>(xs)...);
          }
      };

      /* Function version */
      /* I just ignore eplisis, there is no way to wrap it */
      template<typename R, typename... Args>
      struct wrap_call<R(*)(Args...)>
      {
          using type = R(*)(Args...);
          type f;

          constexpr wrap_call(type fct) : f(fct) {}

          /* Really need to not select other overload based on constness */
          #if 0
          constexpr R operator()(Args... args)
          {
              /* Esentially does an move for values and rvalue reference, normal operation otherwise */
              return f(std::forward<Args>(args)...);
          }
          #endif
          constexpr R operator()(Args... args) const
          {
              return f(std::forward<Args>(args)...);
          }
      };
      template<typename R, typename... Args>
      struct wrap_call<R(&)(Args...)>
      {
          using type = R(&)(Args...);
          type f;

          constexpr wrap_call(type fct) : f(fct) {}

          /* Really need to not select other overload based on constness */
          #if 0
          constexpr R operator()(Args... args)
          {
              /* Esentially does an move for values and rvalue reference, normal operation otherwise */
              return f(std::forward<Args>(args)...);
          }
          #endif

          constexpr R operator()(Args... args) const
          {
              return f(std::forward<Args>(args)...);
          }
      };

      //Probably there is also need to have version for noexcept pointer/reference

      template<typename PointerType, typename Clazz, typename R, typename... Args>
      struct function_member_wrapper
      {
          using type = PointerType;
          type f;

          constexpr function_member_wrapper(type fct) : f(fct) {}

          template<typename U,
                   std::enable_if_t<std::is_base_of<Clazz, std::decay_t<U>>::value
                                 && std::is_convertible<decltype((std::declval<U>().*f)(std::declval<Args>()...)), R>::value, bool> = true>
          constexpr R operator()(U&& obj, Args... args) const
          {
             return (std::forward<U>(obj).*f)(std::forward<Args>(args)...);
          }

          template<typename U,
                   std::enable_if_t<std::is_convertible<decltype((std::declval<U&>().*f)(std::declval<Args>()...)), R>::value, bool> = true>
          constexpr R operator()(std::reference_wrapper<U> ref, Args... args) const
          {
             return (ref.get().*f)(std::forward<Args>(args)...);
          }

          template<typename U,
                   std::enable_if_t<!std::is_base_of<Clazz, std::decay_t<U>>::value
                                 && std::is_convertible<decltype(((*std::declval<U>()).*f)(std::declval<Args>()...)), R>::value, bool> = true>
          constexpr R operator()(U&& ptr, Args... args) const
          {
             return ((*std::forward<U>(ptr)).*f)(std::forward<Args>(args)...);
          }

      };

      /* Function version */
      /* I just ignore eplisis, there is no way to wrap it */
      #define WRAP_CALL_FOR_MEMBER_FUNCTION(cv_ref_qual) \
      template<typename C, typename R, typename... Args> \
      struct wrap_call<R (C::*)(Args...) cv_ref_qual>  \
         : function_member_wrapper<R (C::*)(Args...) cv_ref_qual, C, R, Args...> \
      {  \
          using base = function_member_wrapper<R (C::*)(Args...) cv_ref_qual, C, R, Args...>; \
          using base::base; \
      };

      WRAP_CALL_FOR_MEMBER_FUNCTION();
      WRAP_CALL_FOR_MEMBER_FUNCTION(const);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile);

      WRAP_CALL_FOR_MEMBER_FUNCTION(&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile&);

      WRAP_CALL_FOR_MEMBER_FUNCTION(&&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const&&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile&&);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile&&);

      // You may also need to add noexcept version of each of them

      #undef WRAP_CALL_FOR_MEMBER_FUNCTION

      /* If F is not final class make it direct base of the overload, otherwise wrap in into a call.
         Notice that both F and wrap_call<F> will be constructible from same object (prefect forwarding constructor) */
      template<typename F>
      struct overload_storage
         : std::conditional<std::is_class<F>::value && !std::is_final<F>::value, F, wrap_call<F>>
      {};

      template<typename F>
      using overload_storage_t = typename overload_storage<F>::type;

      template <class F, class ...Fs>
      class overloader;

      template<typename F1>
      class overloader<F1> : overload_storage_t<F1>
      {
          using base1 = overload_storage_t<F1>;

      public:
          template<typename U1>
          constexpr explicit overloader(U1&& fct1)
              : base1(std::forward<U1>(fct1))
          {}

          using base1::operator();
      };
      template<typename F1, typename F2, class ...Fs>
      class overloader<F1, F2, Fs...> : overload_storage_t<F1>, overloader<F2, Fs...>
      {
          using base1 = overload_storage_t<F1>;
          using bases = overloader<F2, Fs...>;

      public:
          template<typename U1, typename U2, typename ...Us>
          constexpr explicit overloader(U1&& fct1, U2&& fct2, Us&& ...fcts)
              : base1(std::forward<U1>(fct1))
              , bases(std::forward<U2>(fct2), std::forward<Us>(fcts)...)
          {}

          using base1::operator();
          using bases::operator();
      };

      template <class R, class ...Fs>
      struct explicit_overloader : overloader<Fs...>
      {
        using result_type = R;
        using overloader<Fs...>::operator();

        template <class ...OFs>
        constexpr explicit_overloader(OFs&& ...fs) : overloader<Fs...>(std::forward<OFs>(fs)...) {}
      };

}
      template<typename F1, typename ...Fs>
      constexpr auto overload(F1&& f1, Fs&& ...fs)
      {
          return detail::overloader<std::decay_t<F1>, std::decay_t<Fs>...>(std::forward<F1>(f1), std::forward<Fs>(fs)...);
      }

      template <class R, class F1, class ... Fs>
      constexpr auto overload(F1 && f1, Fs &&... fcts)
      {
        return detail::explicit_overloader<R, std::decay_t<F1>, std::decay_t<Fs>...>(
          std::forward<F1>(f1), std::forward<Fs>(fcts)...);
      }

}}}

#endif
#endif // header
