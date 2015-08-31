//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_MONAD_MCD_POSSIBLE_VALUED_HPP
#define JASEL_FUNDAMENTAL_V2_MONAD_MCD_POSSIBLE_VALUED_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type that models the PossibleValued concept can be seen as a model of a Functor
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/monad.hpp>
#include <experimental/fundamental/v2/possible_valued.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class PV, class F>
  //  requires PossibleValued<PV>
  //  && Same<TypeConstructor<ResultType<F, ValueType<PV>>>, TypeConstructor<PV>>
  auto mbind_custom(possible_value, const PV& pv, F&& f)
    -> meta::ResultType<F, meta::ValueType<PV>>
  {
    if (has_value(pv))
      return f(value(pv));
    else
      return novalue(pv);
  }

}
}
}


#endif // header

