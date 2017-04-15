// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Based on https://github.com/akrzemi1/explicit/blob/master/include/ak_toolkit/only_when.hpp
// See https://github.com/akrzemi1/explicit/blob/master/doc/only_when.md
// Copyright (C) 2016 Andrzej Krzemienski.
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_UTILITY_ONLY_WHEN_HPP
#define JASEL_FUNDAMENTAL_V3_UTILITY_ONLY_WHEN_HPP

#include <utility>
#include <type_traits>
#include <experimental/fundamental/v2/config.hpp>

namespace std
{
  namespace experimental
  {
  inline  namespace fundamental_v3
  {
    template <typename T, template <typename, typename> class TypePred>
    class only_when
    {
      T _val;

    public:
      template <typename U, JASEL_ENABLE_IF(TypePred<T, U>::value)>
      only_when (U&& v) : _val(forward<U>(v))
      {}

      template <typename U, JASEL_ENABLE_IF(!TypePred<T, U>::value)>
      only_when (U&&) = delete;

      T get() const
      { return _val;}
    };

#if ! defined JASEL_DOXYGEN_INVOKED
    namespace detail
    {
      template <typename, typename T>
      using _is_signed_integral = typename conditional<
      is_signed<T>::value && is_integral<T>::value,
      true_type,
      false_type>::type;

      template <typename I, typename T>
      std::false_type _test_no_narrowing(long long);

      template <typename I, typename T>
      auto _test_no_narrowing(int) -> decltype(I
          { declval<T>()}, true_type
          {});

      template <typename I>
      struct _int_no_double_test
      {
        _int_no_double_test(I)
        {}
        _int_no_double_test(long double) = delete;
        _int_no_double_test(unsigned long long) = delete;
      };
# if defined __GNUC__ && ! defined __clang__
      template <typename I, typename T>
      struct _is_int_convertible_but_no_float : conditional<decltype(_test_no_narrowing<I, T>(1))::value
      && is_convertible<T, I>::value
      && is_constructible<_int_no_double_test<I>, T>::value,
      true_type, false_type>::type
      {};
#else
      template <typename I, typename T>
      struct _is_int_convertible_but_no_float : conditional<decltype(_test_no_narrowing<I, T>(1))::value
      && is_convertible<T, I>::value,
      true_type, false_type>::type
      {};
#endif
      // todo: move to detail
      template <typename T, typename U>
      struct _is_lvalue_ref_or_wrapper2 : conditional<
      is_convertible<U&&, T&>::value && !is_convertible<U&&, T&&>::value,
      true_type,
      false_type
      >::type
      {};
    }
#endif
    using only_int = only_when<int, detail::_is_int_convertible_but_no_float>;
    template <typename T>
    using only_lvalue = only_when<T&, detail::_is_lvalue_ref_or_wrapper2>;
  }
}
}

#endif // header
