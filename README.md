JASEL: Just a simple experimental library for C++
=================================================

Experimental library for C++11/14/17.

# LWG Proposals

* P0051R3 - C++ generic overload function (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0051r3.pdf)
* P0318R1 - `decay_unwrap` and `unwrap_reference` (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0318r1.pdf
* P0323R6 - `std::expected` http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0323r6.pdf

# Ongoing Proposals

* P0050R0 - C++ generic match function  (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2015/p0050r0.pdf)
* P0196R5 - Generic none() factories for Nullable types (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0196r5.pdf)
* P0319R2 - Adding Emplace functions for `promise<T>`/`future<T>`  (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0319r2.pdf
* P0320R1 - Thread Constructor Attributes (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0320r1.pdf
* P0327R3 - Product types access (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0327r3.pdf)

# No yet reviewed yet Proposals

This code forms the basis of several formal proposal to the C++ standard library

* P0338R3 - C++ generic factories (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0338r3.pdf)
* P0343R1 - Meta-programming High-Order Functions (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0343r1.pdf)
* P0648R0 - Extending Tuple-like algorithms to Product-Types - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0648r0.pdf
* P0649R0 - Other Product-Type algorithms - http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2017/p0649r0.pdf
* P0650R2 - C++ Monadic interface (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0650r2.pdf)
* P0786R1 - ValuedOrError and ValueOrNone types (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p0786r1.pdf)
* P1051R0 - `std::experimental::expected` LWG design issues (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2018/p1051r0.pdf)


# New revisions proposals

* D0051R4 - C++ generic overload function (https://github.com/viboes/tags/blob/master/doc/proposal/overload/d0051R4.md)
* D0323R7 - `std::expected` - https://github.com/viboes/std-make/blob/master/doc/proposal/expected/d0323r7.md
* D0338R4 - C++ generic factories - https://github.com/viboes/std-make/blob/master/doc/proposal/factories/d0338r4.md
 
# Draft R0 proposals

* DXXXXR0 - Sum types - https://github.com/viboes/std-make/blob/master/doc/proposal/sum_type/SumType.md
* DXXXXR0 - C++ Ordinal types - https://github.com/viboes/std-make/blob/master/doc/proposal/ordinal/Ordinal.md
* DXXXXR0 - `chrono::modulo` types - https://github.com/viboes/std-make/blob/master/doc/proposal/chrono/DurationModulo.md
* DXXXXR0 - C++ Strong types and mixins

# Withdrawal proposals

The following proposals have been abandoned and should be replaced by an alternative customization approach on *ProductTypes*

* P0198R0 - Default Swap (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0198r0.pdf)
* P0199R0 - Default Hash (http://www.open-std.org/jtc1/sc22/wg21/docs/papers/2016/p0199r0.pdf)

# Development Status

This code is undocumented (other than via the proposals), not fully tested, constantly changing, and generally not fit for any use whatsoever.

# License

Most of the source code in this project are mine, and those are under the Boost Software License. I have borrowed a lot from Hana original editions, before it becomes Boost.Hana. A lot of code is inspired from the library Meta.

# Supported Compilers

The code is known to work with the following compilers:

* Clang 5.0.0 `-std=c++11 -std=c++14 -std=c++1y`
* Clang 6.0.0 `-std=c++11 -std=c++14 -std=c++1y -std=c++2a`
* Clang 7.0.0 `-std=c++11 -std=c++14 -std=c++1y -std=c++2a`
* GCC 6.1.1   `-std=c++11 -std=c++14 -std=c++1z`
* GCC 7.3.0   `-std=c++11 -std=c++14 -std=c++1z`

Other compiler and older versions could works as well, but are not tested regularly

*Development Status:* The aim of the library is to be portable on conforming C++11/14/17 compilers. However some modules require 

* C++14:
    * product_type
    * sum_type, 
    * functional/overload
    * swappable

* C++17
    * expected
 
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

