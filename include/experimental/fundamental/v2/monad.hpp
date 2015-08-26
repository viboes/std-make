//////////////////////////////////////////////////////////////////////////////
//
// (C) Copyright Vicente J. Botet Escriba 2015.
// Distributed under the Boost Software License, Version 1.0.
// (See accompanying file // LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
//
//////////////////////////////////////////////////////////////////////////////

#ifndef JASEL_FUNDAMENTAL_V2_MONAD_HPP
#define JASEL_FUNDAMENTAL_V2_MONAD_HPP

///////////////////////////////////////////////////////////////////////////////////////
///
/// A type M is model of a Monad if
///
///   std::experimental::TypeConstructor<M>
///   std::experimental::ValueType<M>
///
///   make<TypeConstructor<M>>(const ValueType<M>& v) -> M
///   bind(const M &, F&&) -> ResultType<F, ValueType<M>>
///
///////////////////////////////////////////////////////////////////////////////////////

#include <experimental/meta/v1/type_constructor.hpp>
#include <experimental/meta/v1/value_type.hpp>

#include <experimental/fundamental/v2/make.hpp>
#include <experimental/fundamental/v2/monad/monad_tag.hpp>
#include <experimental/fundamental/v2/monad/bind.hpp>

#endif // header
