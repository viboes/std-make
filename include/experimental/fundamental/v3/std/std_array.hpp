// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2016 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_ARRAY_HPP
#define JASEL_FUNDAMENTAL_V3_ARRAY_HPP

#include <experimental/make.hpp>
#include <experimental/meta.hpp>
#include <experimental/functor.hpp>
#include <experimental/product_type.hpp>
#include <array>
#include <type_traits>

namespace std
{
#if ! defined JASEL_HAS_MAKE_ARRAY

  namespace details {
    template<class> struct is_ref_wrapper : false_type {};
    template<class T> struct is_ref_wrapper<reference_wrapper<T>> : true_type {};

    //template<class T>
    //using not_ref_wrapper = experimental::negation<is_ref_wrapper<decay_t<T>>>;

    template <class D, class...> struct return_type_helper { using type = D; };
    template <class... Types>
    struct return_type_helper<void, Types...> : common_type<Types...> {
        //static_assert(experimental::conjunction_v<not_ref_wrapper<Types>...>,
        //              "Types cannot contain reference_wrappers when D is void");
    };

    template <class D, class... Types>
    using return_type = array<typename return_type_helper<D, Types...>::type,
                                   sizeof...(Types)>;
  }

  template < class D = void, class... Types>
  constexpr details::return_type<D, Types...> make_array(Types&&... t) {
    return {{forward<Types>(t)... }};
  }
#endif

namespace experimental
{

  struct array_tc
  {
      template <class ...T>
      using invoke = array<typename common_type<typename decay<T>::type...>::type, sizeof...(T)>;
  };
  // type_constructor customization
  template <class T, size_t N>
  struct type_constructor<array<T, N>> : meta::id<array_tc> {};

inline namespace fundamental_v3 {
namespace type_constructible {
  template <class T, size_t N>
  struct traits<array<T, N>> : type_constructible::tag
  {
    template <class ...Xs>
    static constexpr
    array<T, sizeof...(Xs)> make(Xs&& ...xs)
    {
      return {{forward<Xs>(xs)...}};
    }
  };
}
#if __cplusplus >= 201402L

  namespace functor {
    template <>
    struct traits<array_tc> : product_type::as_functor {};
  }
  namespace applicative {
    template <>
    struct traits<array_tc> : product_type::as_applicative {};
  }
#endif
}
}
}

#endif // header
