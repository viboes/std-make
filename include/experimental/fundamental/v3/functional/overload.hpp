//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2014-2016.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V3_FUNCTIONAL_OVERLOAD_HPP
#define JASEL_FUNDAMENTAL_V3_FUNCTIONAL_OVERLOAD_HPP

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
      // fixme: check if we need to declare the operator() constexpr
#define JASEL_CONSTEXPR_CWG_1581 constexpr

      template< class F>
      struct wrap_call {
          using type = F;
          type f;

          /* Perfect forwarding constructor */
          template<typename U,
                   /* Make sure that it does not act as copy constructor for wrap_call& */
                   enable_if_t<!is_base_of<wrap_call, decay_t<U>>::value
                   /* require that F can be constructed from U&& */
                   && is_constructible<F, U>::value, bool> = true>
          constexpr wrap_call(U&& fct) : f(forward<U>(fct)) { }

          /* todo: using std::invoke would allow to handle anything std::invoke manage already C++17 */
          template< class ...Xs >
          JASEL_CONSTEXPR_CWG_1581 auto operator () (Xs &&... xs) const &
          JASEL_NOEXCEPT_DECLTYPE_RETURN( f(forward<Xs>(xs)...) )

          template< class ...Xs >
          JASEL_CONSTEXPR_CWG_1581 auto operator () (Xs &&... xs) const &&
          JASEL_NOEXCEPT_DECLTYPE_RETURN( move(f)(forward<Xs>(xs)...) )

          template< class ...Xs >
          JASEL_CONSTEXPR_CWG_1581 auto operator () (Xs &&... xs) &
          JASEL_NOEXCEPT_DECLTYPE_RETURN( f(forward<Xs>(xs)...) )

          template< class ...Xs >
          constexpr auto operator () (Xs &&... xs) &&
          JASEL_NOEXCEPT_DECLTYPE_RETURN( move(f)(forward<Xs>(xs)...) )

      };

      /* Function version */
      /* Ignoring ellipsis, there is no way to wrap it */

      template<typename FunctionType, typename R, typename... Args>
      struct non_member_function_wrapper
      {
          using type = FunctionType;
          type f;

          constexpr non_member_function_wrapper(type fct) : f(fct) {}

          JASEL_CONSTEXPR_CWG_1581 auto operator()(Args... args) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( f(forward<Args>(args)...) )

      };

    #define WRAP_CALL_FOR_NON_MEMBER_FUNCTION(ptr_or_ref, noexcept_qual) \
     template<typename R, typename... Args> \
     struct wrap_call<R (ptr_or_ref)(Args...) noexcept_qual>  \
        : non_member_function_wrapper<R (ptr_or_ref)(Args...) noexcept_qual, R, Args...> \
     {  \
         using base = non_member_function_wrapper<R (ptr_or_ref)(Args...) noexcept_qual, R, Args...>; \
         using base::base; \
     }


      WRAP_CALL_FOR_NON_MEMBER_FUNCTION(*,);
      WRAP_CALL_FOR_NON_MEMBER_FUNCTION(&,);
#if __cplusplus > 201402L && defined __clang__
      WRAP_CALL_FOR_NON_MEMBER_FUNCTION(*, noexcept);
      WRAP_CALL_FOR_NON_MEMBER_FUNCTION(&, noexcept);
#endif

#undef WRAP_CALL_FOR_NON_MEMBER_FUNCTION

      template<typename PointerType, typename Clazz, typename R, typename... Args>
      struct function_member_wrapper
      {
          using type = PointerType;
          type f;

          constexpr function_member_wrapper(type fct) : f(fct) {}

          template<typename U,
                   enable_if_t<is_base_of<Clazz, decay_t<U>>::value
                                // && is_convertible<decltype((declval<U>().*f)(declval<Args>()...)), R>::value
                                 , bool> = true>
          JASEL_CONSTEXPR_CWG_1581 auto operator()(U&& obj, Args... args) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( (forward<U>(obj).*f)(forward<Args>(args)...) )

          template<typename U
                   //, enable_if_t<is_convertible<decltype((declval<U&>().*f)(declval<Args>()...)), R>::value, bool> = true
                   >
          JASEL_CONSTEXPR_CWG_1581 auto operator()(reference_wrapper<U> ref, Args... args) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( (ref.get().*f)(forward<Args>(args)...) )

          template<typename U,
                   enable_if_t<!is_base_of<Clazz, decay_t<U>>::value
                                 //&& is_convertible<decltype(((*declval<U>()).*f)(declval<Args>()...)), R>::value
                                 , bool> = true>
          JASEL_CONSTEXPR_CWG_1581 auto operator()(U&& ptr, Args... args) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( ((*forward<U>(ptr)).*f)(forward<Args>(args)...) )

      };

      /* Function version */
      /* I just ignore ellipsis, there is no way to wrap it */
      #define WRAP_CALL_FOR_MEMBER_FUNCTION(cv_ref_qual) \
      template<typename C, typename R, typename... Args> \
      struct wrap_call<R (C::*)(Args...) cv_ref_qual>  \
         : function_member_wrapper<R (C::*)(Args...) cv_ref_qual, C, R, Args...> \
      {  \
          using base = function_member_wrapper<R (C::*)(Args...) cv_ref_qual, C, R, Args...>; \
          using base::base; \
      }

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

#if __cplusplus > 201402L && defined __clang__

      WRAP_CALL_FOR_MEMBER_FUNCTION(noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile noexcept);

      WRAP_CALL_FOR_MEMBER_FUNCTION(& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile& noexcept);

      WRAP_CALL_FOR_MEMBER_FUNCTION(&& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const&& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(volatile&& noexcept);
      WRAP_CALL_FOR_MEMBER_FUNCTION(const volatile&& noexcept);
#endif

      #undef WRAP_CALL_FOR_MEMBER_FUNCTION

      template<typename PointerType, typename Clazz, typename R>
      struct data_member_wrapper
      {
          using type = PointerType;
          type f;

          constexpr data_member_wrapper(type fct) : f(fct) {}

          template<typename U,
                   enable_if_t<is_same<Clazz, decay_t<U>>::value
                                 , bool> = true>
          JASEL_CONSTEXPR_CWG_1581 auto operator()(U&& obj) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( forward<U>(obj).*f )

          template<typename U>
          JASEL_CONSTEXPR_CWG_1581 auto operator()(reference_wrapper<U> ref) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( ref.get().*f )

          template<typename U,
                   enable_if_t<!is_same<Clazz, decay_t<U>>::value
                                 , bool> = true>
          JASEL_CONSTEXPR_CWG_1581 auto operator()(U&& ptr) const
          JASEL_NOEXCEPT_DECLTYPE_RETURN( (*forward<U>(ptr)).*f )

      };

      #define WRAP_CALL_FOR_DATA_MEMBER() \
      template<typename C, typename R> \
      struct wrap_call<R C::*>  \
         : data_member_wrapper<R C::*, C, R> \
      {  \
          using base = data_member_wrapper<R C::*, C, R>; \
          using base::base; \
      }

      WRAP_CALL_FOR_DATA_MEMBER();

#undef WRAP_CALL_FOR_DATA_MEMBER

      // If F is not final class make it direct base of the overload, otherwise wrap in into a call.
      // Notice that both F and wrap_call<F> will be constructible from same object (perfect forwarding constructor)
      // todo: Check if the class has a final virtual destructor, which inhibits also inheriting
      template<typename F>
      struct overload_storage
         : conditional<is_class<F>::value && !is_final<F>::value, F, wrap_call<F>>
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
              : base1(forward<U1>(fct1))
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
              : base1(forward<U1>(fct1))
              , bases(forward<U2>(fct2), forward<Us>(fcts)...)
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
        constexpr explicit_overloader(OFs&& ...fs) : overloader<Fs...>(forward<OFs>(fs)...) {}
      };

}
      template<typename F1, typename ...Fs>
      constexpr auto overload(F1&& f1, Fs&& ...fs)
      {
          return detail::overloader<decay_t<F1>, decay_t<Fs>...>(forward<F1>(f1), forward<Fs>(fs)...);
      }

      template <class R, class F1, class ... Fs>
      constexpr auto overload(F1 && f1, Fs &&... fcts)
      {
        return detail::explicit_overloader<R, decay_t<F1>, decay_t<Fs>...>(
          forward<F1>(f1), forward<Fs>(fcts)...);
      }

}}}

#endif
#endif // header
