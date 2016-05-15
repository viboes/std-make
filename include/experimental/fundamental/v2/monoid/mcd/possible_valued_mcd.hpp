//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_MONOID_MCD_POSSIBLE_VALUED_HPP
#define JASEL_FUNDAMENTAL_V2_MONOID_MCD_POSSIBLE_VALUED_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type that models the PossibleValued concept can be seen as a model of a Functor
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/monoid.hpp>
#include <experimental/fundamental/v2/possible_valued.hpp>
#include <experimental/fundamental/v2/type_constructor.hpp>
#include <experimental/fundamental/v2/none.hpp>

namespace std
{
namespace experimental
{
inline namespace fundamental_v2
{

  template <class PV>
  // requires Monoid<ValueType<PV>>
  PV monoid_id_custom(possible_value, meta::id<PV>)
  {
    return none<type_constructor_t<PV>>();
  }

  template <class PV>
  // requires Monoid<ValueType<PV>>
  auto monoid_op_custom(possible_value, PV const& x, PV const& y)
    -> PV
  {
    if (has_value(x) && has_value(y))
      return make<PV>(monoid_op(value(x), value(y)));
    else
      return not_a_value(x);
  }

}
}
}


#endif // header

