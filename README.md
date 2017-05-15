JASEL: Just a simple experimental library for C++
=================================================

Experimental library for C++11/14/17.

# Proposals

This code form the basis of several formal proposal to the C++ standard library

* P0051R2 - C++ generic overload function (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0051r2.pdf)
* P0196R1 - Generic none() factories for Nullable types (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0196r1.pdf)

* P0318R0 - decay_unwrap and unwrap_reference (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0318r0.pdf
* P0319R0 - Adding Emplace functions for promise<T>/future<T>  (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0319r0.pdf
* P0320R1 - Thread Constructor Attributes (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0320r1.pdf
* P0323R1 - A proposal to add a utility class to represent expected monad (Revision 3) http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0323r1.pdf
* P0327R1 - Product types access (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0327r1.pdf)
* P0338R1 - C++ generic factories (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0338r1.pdf)
* P0343R0 - Meta-programming High-Order Functions (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0343r0.pdf)

# Ongoing new revisions proposals

* D0196R2 - Generic none() factories for Nullable types
* D0319R1 - Adding Emplace functions for `promise<T>`/`future<T>  
* D0323R2 - A proposal to add a utility class to represent expected monad (Revision 4) 
* D0327R2 - Product types access
* D0338R2 - C++ generic factories

# Ongoing New proposals

* DXXXX - Adapting the standard tuple-like function to Product types
* DXXXX - Product types algorithms
* DXXXX - C++ Monadic inteface 
* DXXXX - C++ Ordinal types

# Overrriden proposals

The following proposals have been abandoned and should be replaced by an alternative customization approach on *ProductTypes*

* P0198R0 - Default Swap (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0198r0.pdf)
* P0199R0 - Default Hash (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0199r0.pdf)

# Development Status

This code is undocumented (other than via the proposals), not fully tested, constantly changing, and generally not fit for any use whatsoever.

# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed the `hana::type` class.

# Supported Compilers

The code is known to work with the following compilers:

* Clang 3.7.0 `-std=c++11 -std=c++14 -std=c++1y`
* Clang 3.9.0 `-std=c++11 -std=c++14 -std=c++1y`
* GCC 5.2.1   `-std=c++11 -std=c++14 -std=c++1z`
* GCC 6.1.1   `-std=c++11 -std=c++14 -std=c++1z`

*Development Status:* The aim of the library is to be portable on comforming C++11/14/17 compilers.
However some parts require a C++14 (`product_type` and `swappable`).
Hopefully this will change in the future.

# Header only library

# How to test

Some parts of the code use other repositories as Boost, Optional and Expected.
You will need to set some environment variable to be able to build the test.

Next follows the repositories and the required variable pointing to them:

* Boost: https://github.com/boostorg/boost BOOST_ROOT
* Optional : https://github.com/viboes/Optional STD_OPTIONAL
* Expected : https://github.com/ptal/expected BOOST_EXPECTED

# Acknowledgements

Many thanks to Agustín K-ballo Bergé for his help.
