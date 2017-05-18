// Distributed under the Boost Software License, Version 1.0. (See
// accompanying file LICENSE_1_0.txt or copy at
// http://www.boost.org/LICENSE_1_0.txt)
//
// (C) Copyright 2016 Vicente J. Botet Escriba

#ifndef JASEL_EXPERIMENTAL_NULLABLE_HPP
#define JASEL_EXPERIMENTAL_NULLABLE_HPP


// Nullable customizations
// as functor
#include <experimental/fundamental/v3/nullable/transform.hpp>

// as applicative
#include <experimental/fundamental/v3/nullable/ap.hpp>

// as monad
#include <experimental/fundamental/v3/nullable/bind.hpp>

// as monad_error
#include <experimental/fundamental/v3/nullable/monad_error.hpp>

// as sum_type
#include <experimental/fundamental/v3/nullable/visit.hpp>

// as foldable
//#include <experimental/fundamental/v3/nullable/fold.hpp>

// as traversable
//#include <experimental/fundamental/v3/nullable/traverse.hpp>
//#include <experimental/fundamental/v3/nullable/for_each.hpp>

// Nullable algos
#include <experimental/fundamental/v3/nullable/value_or.hpp>
#include <experimental/fundamental/v3/nullable/error_or.hpp>
#include <experimental/fundamental/v3/nullable/has_error.hpp>
#include <experimental/fundamental/v3/nullable/apply_or.hpp>
#include <experimental/fundamental/v3/nullable/resolve.hpp>

// Nullable core
#include <experimental/fundamental/v3/nullable/none.hpp>

// TypeConstructible
#include <experimental/type_constructible.hpp>

#endif // header
