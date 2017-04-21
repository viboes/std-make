// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_COUNTER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_COUNTER_HPP
//#include <iostream>
/**
 * @file
 * strong_counter<Domain, Rep> is a generalization of std::chrono::duration<Rep, Period>
 *
 * Where Domain is a tag class that has a conversion function between Domains
 *
 * The idea is to be able to define std::chrono::duration<Rep, Period> as an alias of strong_counter<domain::duration<Period>, Rep>.
 *
 * In addition to the underlying() function it provides the count() member function.
 */

#include <experimental/fundamental/v3/strong/strong_type.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>
#include <experimental/fundamental/v3/strong/mixins/additive.hpp>
#include <experimental/fundamental/v3/strong/mixins/comparable.hpp>
#include <experimental/fundamental/v3/strong/mixins/streamable.hpp>
#include <experimental/ordinal.hpp>
#include <experimental/type_traits.hpp>

#include <iosfwd>
#include <limits>
#include <functional>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_counter` is a strongly type tagged by the tag @c Domain that wraps an integral type @c UT and behaves like an @c UT
  The main goal is to be able to define strong counters that are able to control how the mix between them.
  No conversion to the underlying type is provided.

  Example
  <code>
  using X = strong_counter<XDomain, int>;
  using Y = strong_counter<YDomain, int>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */

  // fixme Wondering if strong_counter should be renamed to strong_quantity, as the representation can be a floating point
  // fixme Should this class be final or not
  // Note that strong_counter is not by itself a quantity unit, for that we need a Domain that manage units.
  // This started as a dimensionless strong counter with a Tag
  // Then I moved to a Domain that is able to manage with dimensionless and dimension-1. However the Tag is mixed on the Domain.
  // Wondering if we need a Tag parameter for strongly types and one a Dimension parameter to manage with the conversions.
  // Or maybe we should have quantity<Unit, Rep> and then strong_quantity<Tag, Quantity>.

  //! The domain_values type defines three common representation: zero, min, max.
  //! The zero, min, and max functions in strong_counter forward their work to these functions.
  //! This type can be specialized if the representation Rep requires a specific implementation to return these objects for a specific Domain.
  template <class Domain, class UT>
  struct domain_values {
    //! par Returns: UT(0)
    static constexpr UT zero()  noexcept
        { return UT(0); }
    //! par Returns: numeric_limits<UT>::min()
    static constexpr UT min()  noexcept
        { return numeric_limits<UT>::min(); }
    //! par Returns: numeric_limits<UT>::max()
    static constexpr UT max()  noexcept
        { return numeric_limits<UT>::max(); }
  };

  //! The treat_as_floating_point trait helps determine if a strong_counter can be converted to another strong_counter with a different Doamin.
  //! Implicit conversions between two strong_counters normally depends on the domain.
  //! However, implicit conversions can happen on most domains if treat_as_floating_point<Rep>::value == true.

  template <class Domain, class Rep>
  struct treat_as_floating_point : std::is_floating_point<Rep> {};


  template <class Domain>
  struct strict_copy_domain
  {
    template <class T>
    static T inter_domain_convert(Domain, T const& r) { return r; }
    template <class T>
    static T inter_domain_cast(Domain, T const& r) { return r; }
    template <class T>
    static T intra_domain_convert(T const& r) { return r; }
  };

  struct explicit_conversion_domain
  {
    template <class T, class D, class U>
    static T inter_domain_convert(D, U const& u) { return T{u}; }
    template <class T, class D, class U>
    static T inter_domain_cast(D, U const& u) { return T{u}; }
    template <class T, class U>
    static T intra_domain_convert(U const& u) { return T{u}; }
  };

  struct implicit_conversion_domain
  {
    template <class T, class D, class U>
    static T inter_domain_convert(D, U const& u) { return u; }
    template <class T, class D, class U>
    static T inter_domain_cast(D, U const& u) { return u; }
    template <class T, class U>
    static T intra_domain_convert(U const& u) { return u; }
  };

  //! The domain_converter type defines three conversion functions: inter_domain_convert, inter_domain_cast, intra_domain_convert.
  //! Some strong_counter conversion functions forward their work to these functions.
  //! intra_domain_convert is used by the explicit conversion from the representation
  //! inter_domain_convert is used by the explicit conversion from a strong_counter of another domain
  //! inter_domain_cast is used by the strong_counter_cast conversion from a strong_counter of another domain
  //!
  //! By default these functions just work when an implicit conversion works on the same domain.
  //! This type can be specialized for a specific domain to define when and how these functions work.

  template <class Domain>
  struct domain_converter : strict_copy_domain<Domain> {};

#if !defined JASEL_DOXYGEN_INVOKED

  namespace detail {
  //! @{ Non-member functions associated to the domain_converter customizable functions
  template <class DT, class T, class F, class DF=DT>
  auto inter_domain_convert(DF, F const& r)
  -> decltype(domain_converter<DT>::template inter_domain_convert<T>(DF{}, r))
  {
    return domain_converter<DT>::template inter_domain_convert<T>(DF{}, r);
  }

  template <class DT, class T, class F, class DF=DT>
  auto inter_domain_cast(DF, F const& r)
  -> decltype(domain_converter<DT>::template inter_domain_cast<T>(DF{}, r))
  {
    return domain_converter<DT>::template inter_domain_cast<T>(DF{}, r);
  }

  template <class D, class T, class F>
  auto intra_domain_convert(F const& r)
  -> decltype(domain_converter<D>::template intra_domain_convert<T>(r))
  {
    return domain_converter<D>::template intra_domain_convert<T>(r);
  }
  //! @}

  template <class Domain, class From, class To, class DomainFrom=Domain>
  struct is_inter_domain_convertible
  {
      template<class T>
      static void f(T);

      template<class D, class F, class T, class D2>
      static constexpr auto test(int) ->
      decltype(f(static_cast<T>(inter_domain_convert<D, T>(std::declval<D2>(), std::declval<F>()))), true)
      {
          return true;
      }

      template<class D, class F, class T, class D2>
      static constexpr auto test(...) -> bool
      {
          return false;
      }

      static bool const value = test<Domain,From,To, DomainFrom>(0);
  };

  template <class Domain, class From, class To, class DomainFrom=Domain>
  struct is_inter_domain_cast
  {
      template<class T>
      static void f(T);

      template<class D, class F, class T, class D2>
      static constexpr auto test(int) ->
      decltype(f(static_cast<T>(inter_domain_cast<D, T>(std::declval<D2>(), std::declval<F>()))), true)
      {
          return true;
      }

      template<class D, class F, class T, class D2>
      static constexpr auto test(...) -> bool
      {
          return false;
      }

      static bool const value = test<Domain,From,To, DomainFrom>(0);
  };

  template <class Domain, class From, class To>
  struct is_intra_domain_convertible
  {
      template<class T>
      static void f(T);

      template<class D, class F, class T>
      static constexpr auto test(int) ->
      decltype(f(static_cast<T>(intra_domain_convert<D, T>(std::declval<F>()))), true)
      {
          return true;
      }

      template<class D, class F, class T>
      static constexpr auto test(...) -> bool
      {
          return false;
      }

      static bool const value = test<Domain,From,To>(0);
  };
  }
#endif

  template <class Domain, class UT>
  struct strong_counter;

  namespace mixin {
    template <class Domain1, class UT1, class Domain2, class UT2>
    struct is_compatible_with<strong_counter<Domain1, UT1>, strong_counter<Domain2, UT2>> : true_type {};
  }

  /// strong_counter is a strong number with restricted arithmetic (based on chrono::duration arithmetic).
  /// In the same way we can count seconds, we can count oranges or cars.
  template <class Domain, class UT>
  struct strong_counter final
    : private_strong_type<strong_counter<Domain, UT>, UT>
    , mixin::additive_with_if<strong_counter<Domain, UT>>
    , mixin::comparable<strong_counter<Domain, UT>>
    , mixin::streamable<strong_counter<Domain, UT>>
    //: private_tagged<Domain, UT>
  {
      using domain = Domain;
      using rep = UT;

      using base_type = private_strong_type<strong_counter<Domain, UT>, UT>;
      using base_type::base_type;

      // constructors

      //! \n<b>Effects:</b> Construct an uninitialized strong_counter
      strong_counter() = default;

      strong_counter(strong_counter<Domain, UT> const&) = default;
      strong_counter(strong_counter<Domain, UT> &&) = default;

      //! \n<b>Effects:</b> Constructs a strong_counter from its representations
      //! \n<b>Throws:</b> Anything the copy of the conversion throws.
      //! \n<b>Remarks:</b> This constructor doesn't participates in overload resolution if the representation is not is_intra_domain_convertible
      template <class UT2>
      explicit strong_counter(UT2 const& r
#if !defined JASEL_DOXYGEN_INVOKED
          , typename enable_if<detail::is_intra_domain_convertible<Domain, UT2, UT>::value>::type* = nullptr
#endif
          )
          : base_type(detail::intra_domain_convert<Domain, UT>(r))
      {
      }
      template <class UT2>
      explicit strong_counter(UT2 const& r
#if !defined JASEL_DOXYGEN_INVOKED
          , typename enable_if<! detail::is_intra_domain_convertible<Domain, UT2, UT>::value>::type* = nullptr
#endif
          ) = delete;

      //! \n<b>Effects:</b> Constructs a strong_counter from another strong_counter with a different domain and possibly representation
      //! \n<b>Throws:</b>  Anything the copy of the conversion throws.
      //! \n<b>Remarks:</b> This constructor doesn't participates in overload resolution if the representation is not is_inter_domain_convertible
      template <class Domain2, class UT2>
      strong_counter(strong_counter<Domain2, UT2> const& other
#if !defined JASEL_DOXYGEN_INVOKED
          , typename enable_if<detail::is_inter_domain_convertible<Domain, UT2, UT, Domain2>::value>::type* = nullptr
#endif
          )
          : base_type(detail::inter_domain_convert<Domain,UT>(Domain2{}, other.count()))
      {
      }

      // assignment
#if 1
      strong_counter& operator=(strong_counter<Domain, UT> const&) = default;
      strong_counter& operator=(strong_counter<Domain, UT> &&) = default;
#else
      strong_counter& operator=(strong_counter<Domain, UT> const& other)
      {
        this->value = other.value;
        std::cout << "strong_counter& operator=(strong_counter<Domain, UT> const&) \n";
        return *this;
      }

      strong_counter& operator=(strong_counter<Domain, UT> && other)
      {
        this->value = other.value;
        std::cout << "strong_counter& operator=(strong_counter<Domain, UT> &&) \n";
        return *this;
      }
#endif
      constexpr UT count() const noexcept
          { return this->underlying(); }

      // special counter values
      static constexpr strong_counter zero()  noexcept
          { return strong_counter{domain_values<Domain, UT>::zero()}; }
      static constexpr strong_counter min()  noexcept
          { return strong_counter{domain_values<Domain, UT>::min()}; }
      static constexpr strong_counter max()  noexcept
          { return strong_counter{domain_values<Domain, UT>::max()}; }

      //  Multiplicative operators
      JASEL_MUTABLE_CONSTEXPR strong_counter& operator*=(UT y)  noexcept
          { this->value *= y; return *this; }

      JASEL_MUTABLE_CONSTEXPR strong_counter& operator/=(UT y)  noexcept
          { this->value /= y; return *this; }

      // todo add mixed
      JASEL_MUTABLE_CONSTEXPR strong_counter& operator%=(strong_counter y)  noexcept
          { this->value %= y.count(); return *this; }
      JASEL_MUTABLE_CONSTEXPR strong_counter& operator%=(UT y)  noexcept
          { this->value %= y; return *this; }

      // commented for now, however the behavior should depend on the Domain (e.g. duration domain compare when Tag is different)
      // todo add mixed relational operators for strong_counter
//      // relational operators
//      friend constexpr bool operator==(strong_counter x, strong_counter y)  noexcept
//           { return x.count() == y.count(); }
//      friend constexpr bool operator!=(strong_counter x, strong_counter y)  noexcept
//          { return x.count() != y.count(); }
//      friend constexpr bool operator<(strong_counter x, strong_counter y)  noexcept
//          { return x.count() < y.count(); }
//      friend constexpr bool operator>(strong_counter x, strong_counter y)  noexcept
//          { return x.count() > y.count(); }
//      friend constexpr bool operator<=(strong_counter x, strong_counter y)  noexcept
//          { return x.count() <= y.count(); }
//      friend constexpr bool operator>=(strong_counter x, strong_counter y)  noexcept
//          { return x.count() >= y.count(); }


  };

  // mixed arithmetic for strong_counter

  template <class D1, class R1, class R2>
  constexpr
  strong_counter<D1, typename common_type<R1, R2>::type >
  operator*(const strong_counter<D1, R1>& x,
        const R2& y)
  {
    typedef typename common_type<R1, R2>::type CR;
    typedef strong_counter<D1, CR> CD;
    return CD(CD(x).count() * static_cast<CR>(y));
  }

  template <class D1, class R1, class R2>
  constexpr
  strong_counter<D1, typename common_type<R1, R2>::type >
  operator*(const R2& y, const strong_counter<D1, R1>& x)
  {
    return x * y;
  }

  template <class D1, class R1, class R2>
  constexpr
  strong_counter<D1, typename common_type<R1, R2>::type >
  operator/(const strong_counter<D1, R1>& x,
        const R2& y)
  {
    typedef typename common_type<R1, R2>::type CR;
    typedef strong_counter<D1, CR> CD;
    return CD(CD(x).count() / static_cast<CR>(y));
  }

  template <class D1, class R1, class D2, class R2>
  constexpr
  typename common_type<R1, R2>::type
  operator/(const strong_counter<D1, R1>& x,
        const strong_counter<D2, R2>& y)
  {
    typedef typename common_type<strong_counter<D1, R1>, strong_counter<D2, R2> >::type CD;
    return CD(x).count() / CD(y).count();
  }

  template <class D1, class R1, class R2>
  constexpr
  strong_counter<D1, typename common_type<R1, R2>::type >
  operator%(const strong_counter<D1, R1>& x,
        const R2& y)
  {
    typedef typename common_type<R1, R2>::type CR;
    typedef strong_counter<D1, CR> CD;
    return CD(CD(x).count() % static_cast<CR>(y));
  }

  template <class D1, class R1, class D2, class R2>
  constexpr
  typename common_type<strong_counter<D1, R1>, strong_counter<D2, R2> >::type
  operator%(const strong_counter<D1, R1>& x,
        const strong_counter<D2, R2>& y)
  {
    typedef typename common_type<strong_counter<D1, R1>, strong_counter<D2, R2> >::type CD;
    return CD(CD(x).count() % CD(y).count());
  }


  template <class T>
  struct is_strong_counter : std::false_type {};
  template <class D, class R>
  struct is_strong_counter<strong_counter<D,R>> : std::true_type {};

  // strong_counter_cast

  //! Converts a strong_counter to a strong_counter of different type StrongCounterTo.
  //! No implicit conversions are used.
  //! Multiplications and divisions should be avoided where possible.
  //! Computations should be done in the widest type available and converted, as if by static_cast, to the result type only when finished.
  //! The function does not participate in the overload resolution unless StrongCounterTo is an instance of strong_counter.
  //! Casting between strong_counters that don't lost information (e.g. hours to minutes) can be performed implicitly, no strong_counters_cast is needed.

  template <class StrongCounterTo, class DomainFrom, class RepFrom
#if !defined JASEL_DOXYGEN_INVOKED
      , class = typename enable_if<
                  conjunction<
                    is_strong_counter<StrongCounterTo>
                  , detail::is_inter_domain_cast<typename StrongCounterTo::domain, RepFrom, typename StrongCounterTo::rep, DomainFrom>
                  >::value
                >::type
#endif
  >
  constexpr StrongCounterTo strong_counter_cast(strong_counter<DomainFrom, RepFrom> const& sc)
  {
    return StrongCounterTo(detail::inter_domain_cast<typename StrongCounterTo::domain, typename StrongCounterTo::rep>(DomainFrom{}, sc.count()));
  }

  // Rounding

  //! \n<b>Returns:</b><br> the greatest strong_counter t representable in To that is less or equal to d.
  //! The function does not participate in the overload resolution unless To is an instance of strong_counter.

  template <class To, class D, class R
      , class = typename enable_if<is_strong_counter<To>{}>::type
  >
  To strong_counter_floor(const strong_counter<D, R>& d)
  {
    To t = strong_counter_cast<To>(d);
    if (t>d) --t;
    return t;
  }

  //! \n<b>Returns:</b><br> the smallest strong_counter t representable in To that is greater or equal to d.
  //! The function does not participate in the overload resolution unless To is an instance of strong_counter.
  template <class To, class D, class R
      , class = typename enable_if<is_strong_counter<To>{}>::type
  >
  To strong_counter_ceil(const strong_counter<D, R>& d)
  {
      To t = strong_counter_cast<To>(d);
      if (t < d)
          ++t;
      return t;
  }

  //! \n<b>Returns:</b><br> the value t representable in To that is the closest to d.
  //! If there are two such values, returns the even value (that is, the value t such that t % 2 == 0).
  //! The function does not participate in the overload resolution unless To is an instance of strong_counter
  //! and treat_as_floating_point<typename To::rep>{} is false
  template <class To, class D, class R
      , class = typename enable_if<is_strong_counter<To>{}
                && ! treat_as_floating_point<typename To::domain, typename To::rep>{}
                >::type
  >
  To strong_counter_round(const strong_counter<D, R>& d)
  {
      To t0 = strong_counter_cast<To>(d);
      To t1 = t0+ To{1};
      auto diff0 = d - t0;
      auto diff1 = t1 - d;
      if (diff0 == diff1) {
          if (t0.count() & 1) {
              return t1;
          }
          return t0;
      } else if (diff0 < diff1) {
          return t0;
      }
      return t1;
  }

  template <class D, class R
  , class = enable_if_t<
      strong_counter<D, R>::min() < strong_counter<D, R>::zero()>>
  constexpr strong_counter<D, R> abs(strong_counter<D, R> d)
  {
      return d >= d.zero() ? d : -d;
  }

  template <class Domain, class UT>
  constexpr strong_counter<Domain, UT> make_strong_counter(UT x)
  {
    return strong_counter<Domain, UT>(x);
  }

  //! underlying_type specialization for strong_counter
  //! Exposes the type named type, which is the underlying representation of two strong_counter
  template <class Domain, class UT>
  struct underlying_type<strong_counter<Domain,UT>>
  { using type = UT; };

#if 1
  static_assert(std::is_pod<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_counter<bool,int>>::value, "");
  static_assert(std::is_standard_layout<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivial<strong_counter<bool,int>>::value, "");
#endif
//  // stream operators
//
//  //! input function.
//  //! \n<b>Effects:</b> Extracts a T from is and stores it in the passes x.
//  //! @param is the input stream
//  //! @param x the \c strong_int
//  //! \n<b>Returns:</b><br> \c is.
//  // fixme: Shouldn't this depend on Domain?
//  template <class CharT, class Traits, class Domain, class T >
//  std::basic_istream<CharT, Traits>&
//  operator>>(std::basic_istream<CharT, Traits>& is, strong_counter<Domain, T>& x)
//  {
//    T v;
//    is >> v;
//    x = strong_counter<Domain, T>(v);
//    return is;
//  }
//
//  //! output function.
//  //! @param os the output stream
//  //! @param x the \c strong_int
//  //!
//  //! \n<b>Returns:</b><br> the result of the following expression
//  //! @code
//  //! os << x.undelying()
//  //! @endcode
//  // fixme: Shouldn't this depend on Domain?
//  template <class CharT, class Traits, class Domain, class T >
//  std::basic_ostream<CharT, Traits>&
//  operator<<(std::basic_ostream<CharT, Traits>& os, const strong_counter<Domain, T>& x)
//  {
//    return os << x.underlying();
//  }

  namespace ordinal {
    /// A strong_counter is an ordinal type if the underlying type is an ordinal type
    template <class Tag, class T>
    struct traits<strong_counter<Tag, T>> : wrapped_ordinal_traits<strong_counter<Tag, T>>    { };
  }
}
}

  //! common_type specialization
  //! Exposes the type named type, which is the common type of two strong_counter.
  //! It is a strong_counter with the common type of the domains and the representations.
  template <class Domain1, class UT1, class Domain2, class UT2>
  struct common_type<experimental::strong_counter<Domain1,UT1>, experimental::strong_counter<Domain2,UT2>>
  {
      using type =  experimental::strong_counter<
          typename std::common_type<Domain1, Domain2>::type,
          typename std::common_type<UT1, UT2>::type
          >;
  };

  /// Hash specialization forwarding to the hash of underlying type
  template <class Domain, class UT>
  struct hash<experimental::strong_counter<Domain,UT>>
    : experimental::wrapped_hash<experimental::strong_counter<Domain, UT>> {};

#if 0
  // fixme Is strong_counter really a numeric type?
  //! numeric_limits specialization
  template <class Domain, class UT>
  struct numeric_limits<experimental::strong_counter<Domain,UT>> : numeric_limits<UT> {  };
#endif
}
#endif // header
