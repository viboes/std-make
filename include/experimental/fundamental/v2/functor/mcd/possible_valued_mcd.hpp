//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_FUNCTOR_MCD_POSSIBLE_VALUED_HPP
#define JASEL_FUNDAMENTAL_V2_FUNCTOR_MCD_POSSIBLE_VALUED_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type that models the PossibleValued concept can be seen as a model of a Functor
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/functor.hpp>
#include <experimental/fundamental/v2/possible_valued.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class F, class PV, class ...PVs>
  //  requires PossibleValued<PV>
  //  && Function<F, ValueType<PV>>
  auto fmap_custom(possible_value, F&& f, const PV& pv, const PVs& ...pvs)
    -> meta::invoke<meta::TypeConstructor<PV>, meta::ResultType<F, meta::ValueType<PV>, meta::ValueType<PVs>...>>
  {
    if (have_value(pv, pvs...))
      return make<meta::TypeConstructor<PV>>(f(value(pv), value(pvs)...));
    else
      return not_a_value(pv);
  }

}
}
}


#endif // header

