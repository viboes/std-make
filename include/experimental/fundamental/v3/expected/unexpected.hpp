// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
// (C) Copyright 2014-2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP
#define JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP

#include <experimental/fundamental/v3/expected/config.hpp>
#include <experimental/fundamental/v3/config/requires.hpp>
#include <experimental/wrapped.hpp>
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
  class unexpected;
  template <typename ErrorType>
  class unexpected
  {
#if ! defined __clang__
      // fixme:: gcc needs this to be public or declare expected as friend
  public:
#endif
      ErrorType error_;
      template < class Err >
      friend class unexpected;
  public:
    unexpected() = delete;

    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(Err&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, decay_t<Err>&&>::value )
        ) :
      error_(move(e))
    {
    }

    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(unexpected<Err> const& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err>::value && ! is_convertible<Err, ErrorType>::value)
        ) :
      error_(e.error_)
    {
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected(unexpected<Err> const& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err>::value && is_convertible<Err, ErrorType>::value)
        ) :
      error_(e.error_)
    {
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR explicit unexpected(unexpected<Err>&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err&&>::value && ! is_convertible<Err&&, ErrorType>::value)
        ) :
      error_(move(e.error_))
    {
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected(unexpected<Err>&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err&&>::value && is_convertible<Err&&, ErrorType>::value)
        ) :
      error_(move(e.error_))
    {
    }

#if 0
    BOOST_CONSTEXPR unexpected(unexpected const& e) = default;
    BOOST_CONSTEXPR unexpected(unexpected&& e) = default;

    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected& operator=(unexpected<Err> const& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err>::value && is_convertible<Err, ErrorType>::value)
        )
    {
        error_ = e.error_;
    }
    template < class Err >
    BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected& operator=(unexpected<Err>&& e
        , JASEL_REQUIRES( is_constructible<ErrorType, Err&&>::value && is_convertible<Err&&, ErrorType>::value)
        )
    {
        error_ = move(e.error_);
    }
#endif

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
    JASEL_CONSTEXPR_IF_MOVE_ACCESSORS
    BOOST_FORCEINLINE const ErrorType&& value() const&&
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

    void swap(unexpected & e) {
        using std::swap;
        swap(error_, e.error_);
    }
  };

  template <class E>
  BOOST_FORCEINLINE BOOST_CONSTEXPR unexpected<typename decay<E>::type> make_unexpected(E&& ex)
  {
    return unexpected<typename decay<E>::type> (forward<E>(ex));
  }

#if 1
  template <>
  struct unexpected<exception_ptr>
  {
#if ! defined __clang__
      // fixme:: gcc needs this to be public or declare expected as friend
  public:
#endif
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
#if defined __clang__
#if ! defined JASEL_NO_CXX11_RVALUE_REFERENCE_FOR_THIS

    BOOST_CXX14_CONSTEXPR
    BOOST_FORCEINLINE exception_ptr const& value() const&
    {
      return error_;
    }
    BOOST_CXX14_CONSTEXPR
    BOOST_FORCEINLINE exception_ptr& value() &
    {
      return error_;
    }
    JASEL_CONSTEXPR_IF_MOVE_ACCESSORS
    BOOST_FORCEINLINE exception_ptr&& value() &&
    {
      return constexpr_move(error_);
    }
    JASEL_CONSTEXPR_IF_MOVE_ACCESSORS
    BOOST_FORCEINLINE const exception_ptr&& value() const&&
    {
      return constexpr_move(error_);
    }
#else
    BOOST_CONSTEXPR
    BOOST_FORCEINLINE exception_ptr const& value() const
    {
      return error_;
    }
    BOOST_FORCEINLINE exception_ptr& value()
    {
      return error_;
    }
#endif
#else
    //fixme:: issue with gcc
    BOOST_FORCEINLINE exception_ptr const& value() const
    {
      return error_;
    }
    BOOST_FORCEINLINE exception_ptr& value()
    {
      return error_;
    }
#endif

    void swap(unexpected & e) {
        using std::swap;
        swap(error_, e.error_);
    }

  };
#endif

  // fixme: Add is_comparable
  template <class E1, class E2
      , JASEL_REQUIRES( is_convertible<E1, E2>::value || is_convertible<E2, E1>::value)
  >
  BOOST_CONSTEXPR bool operator==(const unexpected<E1>& x, const unexpected<E2>& y)
  {
    return x.value() == y.value();
  }
  template <class E1, class E2
      , JASEL_REQUIRES( is_convertible<E1, E2>::value || is_convertible<E2, E1>::value)
  >
  BOOST_CONSTEXPR bool operator!=(const unexpected<E1>& x, const unexpected<E2>& y)
  {
    return x != y;
  }

  template <class E>
  BOOST_CONSTEXPR bool operator<(const unexpected<E>& x, const unexpected<E>& y)
  {
    return x.value() < y.value();
  }

  template <class E>
  BOOST_CONSTEXPR bool operator>(const unexpected<E>& x, const unexpected<E>& y)
  {
      return x.value() > y.value();
  }

  template <class E>
  BOOST_CONSTEXPR bool operator<=(const unexpected<E>& x, const unexpected<E>& y)
  {
      return x.value() <= y.value();
  }

  template <class E>
  BOOST_CONSTEXPR bool operator>=(const unexpected<E>& x, const unexpected<E>& y)
  {
      return x.value() >= y.value();
  }
#if 1
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
#endif

  template <typename E>
  struct is_unexpected : false_type {};
  template <typename E>
  struct is_unexpected<unexpected<E>> : true_type {};

  BOOST_FORCEINLINE unexpected<exception_ptr> make_unexpected_from_current_exception()
  {
    return unexpected<exception_ptr> (current_exception());
  }

  namespace wrapping  {
      template <class T>
      struct traits<unexpected<T>>
      {
          template <class U>
          static auto underlying(U&& u)
          JASEL_DECLTYPE_RETURN_NOEXCEPT (
                  u.value()
          )

      };

  }
}}
} // namespace

#endif // JASEL_EXPERIMENTAL_V3_EXPECTED_UNEXPECTED_HPP
