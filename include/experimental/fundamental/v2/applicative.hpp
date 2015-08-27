//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_APPLICATIVE_HPP
#define JASEL_FUNDAMENTAL_V2_APPLICATIVE_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type M is model of a Applicative if it is a model of Functor and
///
///   ap(F const &, M const &) -> Apply<TypeConstructor<M>, ResultType<ValueType<F>, ValueType<M>>>
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/fundamental/v2/functor.hpp>
#include <experimental/fundamental/v2/applicative/applicative_tag.hpp>
#include <experimental/fundamental/v2/applicative/ap.hpp>

#endif // header
