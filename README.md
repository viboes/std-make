JASEL: Just a simple experimental library for C++
=================================================

Experimental library for C++11/14/17. 

# Proposals

This code form the basis of several formal proposal to the C++ standard library

* P0196R1 - Generic none() factories for Nullable types   (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0196r0.pdf).
* P0198R0 - Default Swap - (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0198r0.pdf)
* P0199R0 - Default Hash - (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0199r0.pdf)
* P0318R0 - decay_unwrap and unwrap_reference  (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0318r0.pdf).
* P0327R0 - Product types access    (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0327r0.pdf).
* P0338R0 - C++ generic factories  (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r0.pdf).
* P0343R0 - Meta-programming High-Order Functions (httpshttp://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0343r0.pdf)
 
# Development Status

This code is undocumented (other than via the proposals), not fully tested, constantly changing, and generally not fit for any use whatsoever.

# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed the `hana::type` class.

# Supported Compilers

The code is known to work with the following compilers:

* Clang 3.7.0 -std=c++14 -std=c++1y
* Clang 3.9.0 -std=c++14 -std=c++1y
* GCC 5.2.1   -std=c++14 -std=c++1z
* GCC 6.1.1   -std=c++14 -std=c++1z

*Development Status:* The aim of the library is to be portable on comforming C++11/14/17 compilers. 
However some parts require a C++1z, other C++14 and other C++11 :(
Hopefully this will change in te future.

# Header only library

# How to test

Some parts of the code use other repositories as Boost, Optional and Expected.
You will need to set some environment variable to be able to build the test.

Next follows the repositories and the required variable pointing to them:

* Boost: https://github.com/boostorg/boost BOOST_ROOT 
* Optional : https://github.com/viboes/Optional/ STD_OPTIONAL
* Expected : https://github.com/ptal/expected BOOST_EXPECTED

# Acknowledgements 

Many thanks to Agustín K-ballo Bergé for his help. 
