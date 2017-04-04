/// \file
// JASEL library
//
//  Copyright Vicente J. Botet Escriba 2017
//
//  Use, modification and distribution is subject to the
//  Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//
// Project home: https://github.com/viboes/std_make
//

#ifndef JASEL_VERSION_HPP
#define JASEL_VERSION_HPP

#define JASEL_MAJOR @JASEL_MAJOR@
#define JASEL_MINOR @JASEL_MINOR@
#define JASEL_PATCHLEVEL @JASEL_PATCHLEVEL@

#define JASEL_VERSION (JASEL_MAJOR * 10000 \
                        + JASEL_MINOR * 100 \
                        + JASEL_PATCHLEVEL)

#endif
