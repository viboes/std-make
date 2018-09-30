// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2017 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_VALUE_OR_ERROR_HPP
#define JASEL_EXPERIMENTAL_VALUE_OR_ERROR_HPP

// todo: A ValueOrError is nothing more than an unwrapped SuccessOrFailure
// So given a SuccessOrFailure T unwrap_t<T> would be ValueOrError and a SuccessOrFailure

// alternatively we can have both success_or_failure::transform and value_or_error::transform functions,
// the first will not unwrap and the second will unwrap. When unwrap is the identity then both are equivalent
// This will imply that the user customize a type X as a SuccessOrFailure and immediately he obtains both.
// This is not good. A type can not be seen as a Functor with different type T. So expected<T,E>

// This will add a constraint to ValueOrError
// ValueType and ErrorType are different
// expected<T,E> has a
// success_type == value_type == T
// failure_type == unexpected<E> and error_type == E
// SuccessType is different from FailureType
// However Value_type can be the same as ErrorType
// Do we want to support that
// SuccessType is the same as FailureType?
// result<T>


// ValueOrError customization
// as functor
#include <experimental/fundamental/v3/value_or_error/transform.hpp>

// as applicative
#include <experimental/fundamental/v3/value_or_error/ap.hpp>

// as monad
#include <experimental/fundamental/v3/value_or_error/bind.hpp>

// as monad_error
#include <experimental/fundamental/v3/value_or_error/monad_error.hpp>

// as sum_type
#include <experimental/fundamental/v3/value_or_error/visit.hpp>

// as foldable
//#include <experimental/fundamental/v3/value_or_error/fold.hpp>

// as traversable
//#include <experimental/fundamental/v3/value_or_error/traverse.hpp>
//#include <experimental/fundamental/v3/value_or_error/for_each.hpp>

// ValueOrError algos
#include <experimental/fundamental/v3/value_or_error/value_or.hpp>
#include <experimental/fundamental/v3/value_or_error/value_or_throw.hpp>
#include <experimental/fundamental/v3/value_or_error/error_or.hpp>
#include <experimental/fundamental/v3/value_or_error/check_error.hpp>
#include <experimental/fundamental/v3/value_or_error/apply_or.hpp>
#include <experimental/fundamental/v3/value_or_error/resolve.hpp>

// ValueOrError core
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>

// TypeConstructible
#include <experimental/type_constructible.hpp>

#endif // header
