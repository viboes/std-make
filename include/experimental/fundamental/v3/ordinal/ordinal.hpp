// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2014-2018 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_HPP
#define JASEL_FUNDAMENTAL_V3_ORDINAL_ORDINAL_HPP

#include <experimental/meta.hpp>
#include <experimental/fundamental/v2/config.hpp>
#include <utility>
#include <limits>
#include <climits>
#include <type_traits>

// Open points:
// fixme: should this be bounded_ordinal as it is bounded
// fixme: should pos/val/succ/pred be renamed to whole words position/value/successor/predecessor
// fixme: should the position depend on ordinal type?
// fixme: should the position depend on the size?
// fixme: Shouldn't the size trait be part of ordinal::traits?

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

namespace ordinal {

using index_t = long;
//using index_t = size_t;

namespace detail
{
    struct not_a_ordinal_tag{};
}

#if defined JASEL_DOXYGEN_INVOKED
    template <class T, class Enabler=void>
      struct traits;
#else
    template <class T, class Enabler=void>
      struct traits : traits<T, meta::when<true>> {};

    // Default specialization
    template <typename T, bool condition>
    struct traits<T, meta::when<condition>> : detail::not_a_ordinal_tag
    {
      using size = void;
      using size_type = index_t;
#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
      template <class U>
      static
      U val(size_type)  = delete;

      template <class U>
      static
      size_type pos(U && )  = delete;

#endif
    };
#endif

    // arithmetic traits: usable by bounded with a step of 1
    // fixme: do we need to make SizeType width enough
    template <class T, T Low, T High, index_t Step = 1, class SizeType=index_t>
    struct arithmetic_traits
    {
      static_assert(is_arithmetic<T>::value, "T must be arithmetic");

      using size_type = SizeType;
      using value_type = T;

      static constexpr size_type size_v = size_type(High)-size_type(Low)+1u;
      using size = integral_constant<size_type, size_v>;
      static constexpr value_type val(size_type p) { return static_cast<value_type>(p*Step+Low); }

      static constexpr size_type pos(value_type v)  { return static_cast<size_type>(v*Step-Low); }
    };

    // todo add a logarithmic 2 traits: usable with bit masks
#if 0
    // uncomment when we have constexpr for numeric_limits<T>::min(), numeric_limits<T>::max()
    template <typename T>
    struct integral_traits: : arithmetic_traits<T, numeric_limits<T>::min(), numeric_limits<T>::max()>
    {
      static_assert(is_integral<T>::value, "T must be integral");
      static_assert(sizeof(T) <= sizeof(index_t), "The sizeof T must not be greater than the sizeof index_t");
    };
    // todo specialize ordinal_traits using when<condition> is_integral<T> and sizeof(T) <= sizeof(index_t)
#endif
#if 0
    //error: non-type template argument is not a constant expression
    //      using size = integral_constant<size_type, (High-Low+1)/Step>;
    template <>
    struct traits<long long> : arithmetic_traits<long long, LLONG_MIN, LLONG_MAX> {};
    template <>
    struct traits<unsigned long long> : arithmetic_traits<unsigned long long, 0, ULLONG_MAX> {};
    template <>
    struct traits<long> : arithmetic_traits<long, LONG_MIN, LONG_MAX,unsigned long, unsigned long long> {};
    template <>
    struct traits<unsigned long> : arithmetic_traits<unsigned long, 0, ULONG_MAX,unsigned long, unsigned long long> {};
#endif
    template <>
    struct traits<int> : arithmetic_traits<int, INT_MIN, INT_MAX> {};
    template <>
    struct traits<unsigned int> : arithmetic_traits<unsigned int, 0, UINT_MAX> {};
    template <>
    struct traits<short> : arithmetic_traits<short, SHRT_MIN, SHRT_MAX> {};
    template <>
    struct traits<unsigned short> : arithmetic_traits<unsigned short, 0, USHRT_MAX> {};
    template <>
    struct traits<signed char> : arithmetic_traits<signed char, SCHAR_MIN, SCHAR_MAX> {};
    template <>
    struct traits<unsigned char> : arithmetic_traits<unsigned char, 0, UCHAR_MAX> {};
    // fixme should char be an ordinal type?
    //  it is isomorphic to 0..(CHAR_MAX-CHAR_MIN+1)
    // fixme: is bool an ordinal type?
    //  it is isomorphic to 0..1
    template <>
    struct traits<bool> : arithmetic_traits<bool, false, true> {};


namespace meta {

  // forwards to traits<O>::size
  template <class O>
  struct size : traits<O>::size {};

  // this traits has a sense only for compile time ordinal types
  template <class O, O V>
  struct pos;

  // this traits has a sense only for compile time ordinal types
  template <class O, index_t I>
  struct val;

  // todo Specialize pos/val for integral types.
  // fixme: Should these traits make use of when(cnd) to allow conditional specializations?
  template <class O>
  struct first : meta::val<O, 0> {};

  template <class O>
  struct last : meta::val<O, meta::size<O>::val-1> {};

  template <class O, O V>
  struct succ : meta::val<O, meta::pos<O,V>::value+1> {};

  template <class O, O V>
  struct pred : meta::val<O, meta::pos<O,V>::value-1> {};

}

  /// @par Returns
  ///   the number of elements of the `Ordinal` type.
  template <class O>
  constexpr
  auto size() noexcept -> decltype(meta::size<O>::value)
  { return meta::size<O>::value; }

  template <class Ord>
  constexpr auto val(index_t pos) noexcept -> decltype(traits<Ord>::val(pos))
  {
      //JASEL_EXPECTS(valid_position<Ord>(pos));
      return traits<Ord>::val(pos);
  }

  template <class Ord>
  constexpr auto pos(Ord&& val) noexcept -> decltype(traits<experimental::meta::uncvref_t<Ord>>::pos(std::forward<Ord>(val)))
  {
      //JASEL_EXPECTS(unique<Ord>());
      return traits<experimental::meta::uncvref_t<Ord>>::pos(std::forward<Ord>(val));
  }

  template <class Ord>
  constexpr auto first() noexcept -> decltype(ordinal::val<Ord>(0))
  {
    return ordinal::val<Ord>(0);
  }

  template <class Ord>
  constexpr auto last() noexcept -> decltype(ordinal::val<Ord>(ordinal::meta::size<Ord>::value-1))
  {
    return ordinal::val<Ord>(ordinal::meta::size<Ord>::value-1);
  }

  template <class Ord>
  constexpr auto succ(Ord&& val) noexcept -> decltype(ordinal::val<Ord>(ordinal::pos(val)+1))
  {
    return ordinal::val<Ord>(ordinal::pos(val)+1) ;
  }
  template <class Ord>
  constexpr auto pred(Ord&& val) noexcept -> decltype(ordinal::val<Ord>(ordinal::pos(val)-1))
  {
    return ordinal::val<Ord>(ordinal::pos(val)-1) ;
  }

  template <class Ord>
  constexpr auto compare(Ord const& x, Ord const& y) noexcept
  {
    return compare(ordinal::pos(x), ordinal::pos(x));
  }

  template <class Ord>
  struct less
  {
      bool operator()(Ord const& x, Ord const& y) const noexcept
      {
        return ordinal::pos(x) < ordinal::pos(y);
      }
      typedef Ord first_argument_type;
      typedef Ord second_argument_type;
      typedef bool result_type;
  };
  template <class Ord>
  struct less_equal
  {
      bool operator()(Ord const& x, Ord const& y) const noexcept
      {
        return ordinal::pos(x) <= ordinal::pos(y);
      }
      typedef Ord first_argument_type;
      typedef Ord second_argument_type;
      typedef bool result_type;
  };
  template <class Ord>
  struct greater
  {
      bool operator()(Ord const& x, Ord const& y) const noexcept
      {
        return ordinal::pos(x) > ordinal::pos(y);
      }
      typedef Ord first_argument_type;
      typedef Ord second_argument_type;
      typedef bool result_type;
  };
  template <class Ord>
  struct greater_equal
  {
      bool operator()(Ord const& x, Ord const& y) const noexcept
      {
        return ordinal::pos(x) >= ordinal::pos(y);
      }
      typedef Ord first_argument_type;
      typedef Ord second_argument_type;
      typedef bool result_type;
  };

}

  // fixme check for the real operations size/val/pos
  template <class T>
  struct is_ordinal
#if ! defined JASEL_DOXYGEN_INVOKED
     : integral_constant<bool,
           ! is_base_of<ordinal::detail::not_a_ordinal_tag, ordinal::traits<T>>::value
       > {}
#endif
      ;
  template <class T>
  struct is_ordinal<const T> : is_ordinal<T> {};
  template <class T>
  struct is_ordinal<volatile T> : is_ordinal<T> {};
  template <class T>
  struct is_ordinal<const volatile T> : is_ordinal<T> {};

#if __cplusplus >= 201402L || defined JASEL_DOXYGEN_INVOKED
  template <class T>
  constexpr bool is_ordinal_v = is_ordinal<T>::value ;
#endif

  // todo add is_ct_ordinal<CTO>
}
}
}

#endif // header
