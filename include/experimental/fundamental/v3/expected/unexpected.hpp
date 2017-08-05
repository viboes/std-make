// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2014 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP

#include <experimental/fundamental/v3/expected/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/utility.hpp>

#include <exception>
#include <utility>
#include <type_traits>
#include <system_error>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  template <typename ErrorType = error_code>
  class unexpected
  {
  public:
    ErrorType error_;
    unexpected() = delete;

    JASEL_0_REQUIRES(is_copy_constructible<ErrorType>::value)
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(ErrorType const& e) :
      error_(e)
    {
    }
    JASEL_0_REQUIRES(is_move_constructible<ErrorType>::value)
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(ErrorType&& e) :
      error_(move(e))
    {
    }

    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(unexpected<Err> const& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err>::value)
        ) :
      error_(e.error_)
    {
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(unexpected<Err>&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err&&>::value)
        ) :
      error_(move(e.error_))
    {
    }

    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(Err const& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err>::value)
        ) :
      error_(e)
    {
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(ErrorType&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err&&>::value)
        ) :
      error_(move(e))
    {
    }

#if ! defined JASEL_NO_CXX11_RVALUE_REFERENCE_FOR_THIS

    BOOST_CXX14_CONSTEXPR
    BOOST_FORCEINLINE ErrorType const& value() const&
    {
      return error_;
    }
    BOOST_CXX14_CONSTEXPR
    BOOST_FORCEINLINE ErrorType& value() &
    {
      return error_;
    }
    JASEL_CONSTEXPR_IF_MOVE_ACCESSORS
    BOOST_FORCEINLINE ErrorType&& value() &&
    {
      return constexpr_move(error_);
    }
#else
    BOOST_CONSTEXPR
    BOOST_FORCEINLINE ErrorType const& value() const
    {
      return error_;
    }
    BOOST_FORCEINLINE ErrorType& value()
    {
      return error_;
    }
#endif
  };

  template <class E>
  BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected<typename decay<E>::type> make_unexpected(E&& ex)
  {
    return unexpected<typename decay<E>::type> (forward<E>(ex));
  }

  template <>
  struct unexpected<exception_ptr>
  {
    exception_ptr error_;
  public:
    unexpected() = delete;

    BOOST_FORCEINLINE explicit unexpected(exception_ptr const& e) :
      error_(e)
    {
    }

    // fixme: warning: passing result of std::move() as a const reference argument; no move will actually happen [misc-move-const-arg]
    BOOST_FORCEINLINE explicit unexpected(exception_ptr &&e) :
      error_(move(e))
    {
    }

    template <class E>
    BOOST_FORCEINLINE explicit unexpected(E e) :
      error_(make_exception_ptr(e))
    {
    }
    BOOST_FORCEINLINE exception_ptr const& value() const
    {
      return error_;
    }
  };

  template <class E>
  BOOST_CONSTEXPR bool operator==(const unexpected<E>& x, const unexpected<E>& y)
  {
    return x.value() == y.value();
  }
  template <class E>
  BOOST_CONSTEXPR bool operator!=(const unexpected<E>& x, const unexpected<E>& y)
  {
    return !(x == y);
  }

  template <class E>
  BOOST_CONSTEXPR bool operator<(const unexpected<E>& x, const unexpected<E>& y)
  {
    return x.value() < y.value();
  }

  template <class E>
  BOOST_CONSTEXPR bool operator>(const unexpected<E>& x, const unexpected<E>& y)
  {
    return (y < x);
  }

  template <class E>
  BOOST_CONSTEXPR bool operator<=(const unexpected<E>& x, const unexpected<E>& y)
  {
    return !(y < x);
  }

  template <class E>
  BOOST_CONSTEXPR bool operator>=(const unexpected<E>& x, const unexpected<E>& y)
  {
    return !(x < y);
  }

  inline BOOST_CONSTEXPR bool operator<(const unexpected<exception_ptr>&, const unexpected<exception_ptr>&)
  {
    return false;
  }
  inline BOOST_CONSTEXPR bool operator>(const unexpected<exception_ptr>&, const unexpected<exception_ptr>&)
  {
    return false;
  }
  inline BOOST_CONSTEXPR bool operator<=(const unexpected<exception_ptr>& x, const unexpected<exception_ptr>& y)
  {
    return x==y;
  }
  inline BOOST_CONSTEXPR bool operator>=(const unexpected<exception_ptr>& x, const unexpected<exception_ptr>& y)
  {
    return x==y;
  }

  template <typename E>
  struct is_unexpected : false_type {};
  template <typename E>
  struct is_unexpected<unexpected<E>> : true_type {};

  BOOST_FORCEINLINE unexpected<exception_ptr> make_unexpected_from_current_exception()
  {
    return unexpected<exception_ptr> (current_exception());
  }

}}
} // namespace

#endif // JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP
