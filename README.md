JASEL: Just a simple experimental library for C++
=================================================

Experimental library for C++11/14/17.

# Proposals

This code forms the basis of several formal proposal to the C++ standard library

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

* D0196R2 - Generic none() factories for Nullable types - https://github.com/viboes/std-make/blob/master/doc/proposal/nullable/D0196R3.md
* D0319R1 - Adding Emplace functions for `promise<T>`/`future<T>  - https://github.com/viboes/std-make/blob/master/doc/proposal/future/d0319r1.md
* D0323R2 - A proposal to add a utility class to represent expected monad (Revision 4) - https://github.com/viboes/std-make/blob/master/doc/proposal/expected/d0323r2.md
* D0327R2 - Product types access - https://github.com/viboes/std-make/blob/master/doc/proposal/reflection/d0327r2.md
* D0338R2 - C++ generic factories - https://github.com/viboes/std-make/blob/master/doc/proposal/factories/d0338r2.md

# Ongoing New proposals

* DXXXX - Adapting the standard tuple-like function to Product types - https://github.com/viboes/std-make/blob/master/doc/proposal/reflection/TupleLikeToProductType.md
* DXXXX - Product types algorithms - https://github.com/viboes/std-make/blob/master/doc/proposal/reflection/ProductTypeAlgorithms.md
* DXXXX - C++ Monadic inteface - https://github.com/viboes/std-make/blob/master/doc/proposal/monads/Monads.md
* DXXXX - Sum types
* DXXXX - C++ Ordinal types
* DXXXX - C++ Strong types and mixins
* DXXXX - `chrono::modulo types

# Overrriden proposals

The following proposals have been abandoned and should be replaced by an alternative customization approach on *ProductTypes*

* P0198R0 - Default Swap (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0198r0.pdf)
* P0199R0 - Default Hash (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0199r0.pdf)

# Development Status

This code is undocumented (other than via the proposals), not fully tested, constantly changing, and generally not fit for any use whatsoever.

# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed a lot from Hana original editions, before itbecomes Boost.Hana. A lot of code is inspired from the library Meta.

# Supported Compilers

The code is known to work with the following compilers:

* Clang 3.9.0 `-std=c++11 -std=c++14 -std=c++1y`
* Clang 4.4.0 `-std=c++11 -std=c++14 -std=c++1y`
* GCC 6.1.1   `-std=c++11 -std=c++14 -std=c++1z`


Other compiler and older versions could works as well, but are not tested regularly

*Development Status:* The aim of the library is to be portable on comforming C++11/14/17 compilers.
However some modules require C++14:

* product_type
* sum_type, 
* functional/overload
* swappable

Hopefully this will change in the future.

# Header only library

# Dependencies

Some parts of the code use other repositories as Boost, Optional and Expected.
You will need to set some environment variable to be able to build the test.

* BOOST_ROOT : https://github.com/boostorg
* BOOST_EXPECTED : https://github.com/ptal/expected/
* STD_OPTIONAL : https://github.com/viboes/Optional/include

Next follows the repositories and the required variable pointing to them:

* Boost: https://github.com/boostorg/boost BOOST_ROOT
* Optional : https://github.com/viboes/Optional STD_OPTIONAL
* Expected : https://github.com/ptal/expected BOOST_EXPECTED

# How to test

You can either use CMAKE or Boost.Build.

## Boost.Build

You need to have installed The 3 dependent repositories and define the variable described above. In order to run the tests go to the test folder and do

```
b2 toolset=<your tool set>
``` 

## CMAKE

The CMAKE build doesn't use the additional repositories Optional and Expected, and so there are less test.

You need to install Boost and define the variable described above.

Do as you will do with CMAKE, make a build directory associated to your tool set and

```
mkdir build
cd build
cmake -C <toolset-file> <jasel-root-directory>
``` 
 

Then you can build the tests

```
make -j8
``` 

and run the test

```
make -j8 test
``` 


You can as well use CTEST.


# Acknowledgements

Many thanks to Agustín K-ballo Bergé, David Sankel for his help, and pertinent feedback.

