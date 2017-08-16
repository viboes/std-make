// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_NULLABLE_HPP
#define JASEL_EXPERIMENTAL_NULLABLE_HPP


// Nullable customizations
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

// Nullable algos
//#include <experimental/fundamental/v3/value_or_error/value_or.hpp>
//#include <experimental/fundamental/v3/value_or_error/error_or.hpp>
#include <experimental/fundamental/v3/value_or_error/has_error.hpp>
//#include <experimental/fundamental/v3/value_or_error/apply_or.hpp>
#include <experimental/fundamental/v3/value_or_error/resolve.hpp>

// Nullable core
#include <experimental/fundamental/v3/value_or_error/value_or_error.hpp>

// TypeConstructible
#include <experimental/type_constructible.hpp>

#endif // header
