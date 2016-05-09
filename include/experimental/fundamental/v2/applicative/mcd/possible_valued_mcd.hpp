//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_APPLICATIVE_MCD_POSSIBLE_VALUED_HPP
#define JASEL_FUNDAMENTAL_V2_APPLICATIVE_MCD_POSSIBLE_VALUED_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type that models the PossibleValued concept can be seen as a model of a Functor
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/applicative.hpp>
#include <experimental/fundamental/v2/functor.hpp>
#include <experimental/fundamental/v2/possible_valued.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class F, class PV>
  //  requires PossibleValued<F> && PossibleValued<PV>
  //  && Function<ValueType<F>, ValueType<PV>>
  //  && Same<meta::TypeConstructor<F>, meta::TypeConstructor<PV>>
  auto ap_custom(possible_value, F const& f, const PV& pv)
    -> meta::invoke<meta::TypeConstructor<PV>, meta::ResultType<meta::ValueType<F>, meta::ValueType<PV>>>
  {
    if (has_value(f))
      return fmap(value(f), pv);
    else
      return not_a_value(f);
  }


}
}
}


#endif // header

