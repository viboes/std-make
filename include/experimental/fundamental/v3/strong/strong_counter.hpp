// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// Copyright (C) 2017 Vicente J. Botet Escriba

#ifndef JASEL_FUNDAMENTAL_V3_STRONG_STRONG_COUNTER_HPP
#define JASEL_FUNDAMENTAL_V3_STRONG_STRONG_COUNTER_HPP

/**
 * strong_counter<Domain, Rep> is a generalization of std::chrono::duration<Rep, Period>
 *
 * Where Domain is a tag class that has a conversion function between Domains
 *
 * The idea is to be able to define std::chrono::duration<Rep, Period> as an alias of strong_counter<domain::duration<Period>, Rep>.
 *
 * In addition to the underlying() function it provides the count() member function.
 */
#if __cplusplus >= 201402L

#include <experimental/fundamental/v3/strong/tagged.hpp>
#include <experimental/fundamental/v3/strong/underlying_type.hpp>

#include <limits>
#include <functional>
#include <type_traits>

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{

  /**
  `strong_counter` is a strongly type tagged by the tag @c Domain that wraps an integral type @c UT and behaves like an @c UT
  The main goal is to be able to define strong integers that don't mix between them.
  No conversion to the underlying integer type is provided.

  Note that this is not a safe int, it just forbids the conversions between
  different strong integers types.

  Example
  <code>
  using X = strong_counter<XDomain, int>;
  using Y = strong_counter<YDomain, int>;
  void f(X, Y);

  f(X(1), Y(1));

  </code>
  */

  template <class Domain, class UT>
  struct domain_values {
    static constexpr UT zero()  noexcept
        { return UT(0); }
    static constexpr UT min()  noexcept
        { return numeric_limits<UT>::min(); }
    static constexpr UT max()  noexcept
        { return numeric_limits<UT>::max(); }
  };

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

  template <class Domain>
  struct domain_converter : strict_copy_domain<Domain> {};

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

  // fixme wondering if strong_counter should be renamed to strong_quantity, as the representation can be a floating point
  // Note that strong_counter is not by them self a quantity unit, for that we need a Domain that manage units.
  template <class Domain, class UT>
  struct strong_counter final : private_tagged<Domain, UT>
  {
    // fixme do we need this restriction. While we could need it for counters
      static_assert(is_integral<UT>::value, "UT must be integral");
      using domain = Domain;
      using rep = UT;

      using base_type = private_tagged<Domain, UT>;
      using base_type::base_type;

      // constructors
      strong_counter() = default;
      strong_counter(strong_counter const&) = default;
      strong_counter(strong_counter &&) = default;

      //! @par Effects: constructs a strong_counter from its representations
      //! @par Throws: Anything the copy of the conversion throws.
      //! @par Remarks This constructor doesn't participates in overload resolution if the representation is not is_intra_domain_convertible
      template <class UT2>
      explicit strong_counter(UT2 const& r
          , typename enable_if<is_intra_domain_convertible<Domain, UT2, UT>::value>::type* = nullptr
          )
          : base_type(intra_domain_convert<Domain, UT>(r))
      {}
      template <class UT2>
      explicit strong_counter(UT2 const& r
          , typename enable_if<! is_intra_domain_convertible<Domain, UT2, UT>::value>::type* = nullptr
          ) = delete;

      //! @par Effects: constructs a strong_counter from another strong_counter with a different domain and possibly representation
      //! @par Throws: Anything the copy of the conversion throws.
      //! @par Remarks This constructor doesn't participates in overload resolution if the representation is not is_inter_domain_convertible
      template <class Domain2, class UT2>
      strong_counter(strong_counter<Domain2, UT2> const& other
          , typename enable_if<
              is_inter_domain_convertible<Domain, UT2, UT, Domain2>::value
          >::type* = nullptr
          )
            : base_type(inter_domain_convert<Domain,UT>(Domain2{}, other.count()))
      {}

      // assignment
      strong_counter& operator=(strong_counter const&) = default;
      strong_counter& operator=(strong_counter &&) = default;

      // todo add assignment from strong_counter<Domain, UT2>
      // todo add assignment from strong_counter<Domain2, UT2>

      constexpr UT count() const noexcept
          { return this->underlying(); }

      // special counter values
      static constexpr strong_counter zero()  noexcept
          { return strong_counter{domain_values<Domain, UT>::zero()}; }
      static constexpr strong_counter min()  noexcept
          { return strong_counter{domain_values<Domain, UT>::min()}; }
      static constexpr strong_counter max()  noexcept
          { return strong_counter{domain_values<Domain, UT>::max()}; }

      // additive operators
      friend constexpr strong_counter operator+(strong_counter x)  noexcept
          { return x; }
      friend constexpr strong_counter operator+(strong_counter x, strong_counter y)  noexcept
          { return strong_counter(x.count() + y.count()); }
      constexpr strong_counter& operator+=(strong_counter y)  noexcept
          { this->value += y.count(); return *this; }
      constexpr strong_counter operator++()  noexcept
          { return strong_counter(++this->value); }
      constexpr strong_counter operator++(int)  noexcept
          { return strong_counter(this->value++); }

      friend constexpr strong_counter operator-(strong_counter x)  noexcept
          { return strong_counter(-x.count()); }
      friend constexpr strong_counter operator-(strong_counter x, strong_counter y)  noexcept
          { return strong_counter(x.count() - y.count()); }
      constexpr strong_counter& operator-=(strong_counter y)  noexcept
          { this->value -= y.count(); return *this; }
      constexpr strong_counter operator--()  noexcept
          { return strong_counter(--this->value); }
      constexpr strong_counter operator--(int)  noexcept
          { return strong_counter(this->value--); }

      //  Multiplicative operators
      friend constexpr strong_counter operator*(strong_counter x, UT y)  noexcept
          { return strong_counter(x.count() * y); }
      friend constexpr strong_counter operator*(UT x, strong_counter y)  noexcept
          { return strong_counter(x * y.count()); }
      constexpr strong_counter& operator*=(UT y)  noexcept
          { this->value *= y; return *this; }

      friend constexpr UT operator/(strong_counter x, strong_counter y)  noexcept
          { return x.count() / y.count(); }
      friend constexpr strong_counter operator/(strong_counter x, UT y)  noexcept
          { return strong_counter(x.count() / y); }
      constexpr strong_counter& operator/=(UT y)  noexcept
          { this->value /= y; return *this; }

      friend constexpr strong_counter operator%(strong_counter x, strong_counter y)  noexcept
          { return strong_counter(x.count() % y.count()); }
      friend constexpr strong_counter operator%(strong_counter x, UT y)  noexcept
          { return strong_counter(x.count() % y); }
      constexpr strong_counter& operator%=(strong_counter y)  noexcept
          { this->value %= y.count(); return *this; }
      constexpr strong_counter& operator%=(UT y)  noexcept
          { this->value %= y; return *this; }

      // todo add mixed arithmetic for strong_counter

      // relational operators
      friend constexpr bool operator==(strong_counter x, strong_counter y)  noexcept
           { return x.count() == y.count(); }
      friend constexpr bool operator!=(strong_counter x, strong_counter y)  noexcept
          { return x.count() != y.count(); }
      friend constexpr bool operator<(strong_counter x, strong_counter y)  noexcept
          { return x.count() < y.count(); }
      friend constexpr bool operator>(strong_counter x, strong_counter y)  noexcept
          { return x.count() > y.count(); }
      friend constexpr bool operator<=(strong_counter x, strong_counter y)  noexcept
          { return x.count() <= y.count(); }
      friend constexpr bool operator>=(strong_counter x, strong_counter y)  noexcept
          { return x.count() >= y.count(); }

      // todo add mixed relational operators for strong_counter

  };


  // strong_counter_cast

  template <class ModuloTo, class DomainFrom, class RepFrom>
  constexpr ModuloTo strong_counter_cast(strong_counter<DomainFrom, RepFrom> const& sc)
  {
    return ModuloTo(inter_domain_cast<typename ModuloTo::domain, typename ModuloTo::rep>(DomainFrom{}, sc.count()));
  }

  // todo see what floor, round, ceil, abs could mean for strong_counter

  template <class Domain, class UT>
  constexpr strong_counter<Domain, UT> make_strong_counter(UT x)
  {
    return strong_counter<Domain, UT>(x);
  }

  //! underlying_type specialization for strong_counter
  template <class Domain, class UT>
  struct underlying_type<strong_counter<Domain,UT>>
  { using type = UT; };

  static_assert(std::is_pod<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivially_default_constructible<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivially_copyable<strong_counter<bool,int>>::value, "");
  static_assert(std::is_standard_layout<strong_counter<bool,int>>::value, "");
  static_assert(std::is_trivial<strong_counter<bool,int>>::value, "");
}
}

  template <class Domain1, class UT1, class Domain2, class UT2>
  struct common_type<experimental::strong_counter<Domain1,UT1>, experimental::strong_counter<Domain2,UT2>>
  {
      using type =  experimental::strong_counter<
          typename std::common_type<Domain1, Domain2>::type,
          typename std::common_type<UT1, UT2>::type
          >;
  };

  template <class Domain, class UT>
  struct hash<experimental::strong_counter<Domain,UT>>
    : experimental::wrapped_hash<experimental::strong_counter<Domain, UT>> {};

  template <class Domain, class UT>
  struct numeric_limits<experimental::strong_counter<Domain,UT>> : numeric_limits<UT> {  };


}
#endif
#endif // header
