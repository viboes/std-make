// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/bit_mask.hpp>

#include <experimental/subword.hpp>

#include <boost/detail/lightweight_test.hpp>

#include <array>
#include <list>

namespace stde = std::experimental;

int main()
{
  {
    // First, we create an integer
    using uint_t = unsigned int;
    uint_t intval = 43;                                        // 101011

    std::cout<<intval<<std::endl;

    using nbits = stde::subword_value<2>;

    static_assert(nbits::mask() == 0x3, "");
    using nbits_ref = stde::subword_reference<2>;

    {
    auto x = (intval >> 2u) & stde::bit_ops::up_to<2>();
    std::cout<<x<<std::endl;
    }

    {
    auto x = stde::bit_ops::from_up_to<2>(intval, 2u);
    std::cout<<x<<std::endl;
    }

    // Then we create aligned bit values and a bit references on this integer
    nbits bval0(intval);                      // Creates a bit value from the nbits at position 0 of intval
    assert(bval0.value() == 0x3);
    nbits_ref bref0(intval);                  // Creates a bit reference from the nbits at position 0 of intval
    std::cout<<bref0<<std::endl;              // Prints ??

    // And unaligned nbit2 values and a bit references on this integer
    nbits bval5(intval, 2);                   // Creates a bit value from the nbits at position 5 of intval
    std::cout<<bval5<<std::endl;              // Prints ??
    assert(bval5.value() == 0x02);
    nbits_ref bref5(intval, 2);               // Creates a bit reference from the nbits at position 5 of intval
    assert(bval5 == bref5);
    // Display them
    //std::cout<<bval0<<bref0<<bval5<<bref5<<std::endl;          // Prints ????


    // Prints the location and the corresponding mask of bit references
    std::cout<<bref0.position()<<" " << std::hex<<int(bref0.mask())<<std::dec<<std::endl; // Prints 0 and 1
    std::cout<<bref5.position()<<" "<< std::hex<<int(bref5.mask())<<std::dec<<std::endl; // Prints 5 and 32

    bref0.assign(0u);
    assert(bval0 != bref0);

    bval0.assign(0u);
    assert(bval0 == bref0);
  }
  std::cout<< "--------" <<std::endl;

  {
    // First, we create an array of integers
    using uint_t = unsigned int;
    //using nbits = stde::subword_value<2>;
    using nbits_ref = stde::subword_reference<4>;
    using nbits_ptr = stde::subword_pointer<4>;

    std::array<uint_t, 2> intarr = {{42, 314}};
    std::cout<<std::hex<<intarr[0]<<std::endl;                    // Prints 7 as the bit is now in the second element of the array
    std::cout<<std::hex<<intarr[1]<<std::endl;                    // Prints 7 as the bit is now in the second element of the array

    // Then we create a bit reference and a bit pointer
    nbits_ref bref(intarr[0], 1);            // Creates a bit reference from the bit at position 5 of the first element of the array
    nbits_ptr bptr(intarr.data());              // Creates a bit pointer from the bit at position 0 of the first element of the array
    std::cout<<bptr->position()<<std::endl;                    // Prints 7 as the bit is now in the second element of the array

    // We flip the first nbits, and sets the second one to 1 with two methods
    std::cout<<*bptr<<std::endl;              // Prints ??
    bptr->flip();                                              // Flips the nbits
    std::cout<<std::hex<<intarr[0]<<std::endl;                    // Prints 7 as the bit is now in the second element of the array
    std::cout<<std::hex<<intarr[1]<<std::endl;                    // Prints 7 as the bit is now in the second element of the array
    std::cout<<*bptr<<std::endl;              // Prints ??
    ++bptr;                                                    // Goes to the next nbits
    std::cout<< "--------" <<std::endl;
    std::cout<<bptr->position()<<std::endl;                    // Prints 7 as the bit is now in the second element of the array
    std::cout<<*bptr<<std::endl;              // Prints ??
    //(*bptr).set();                                               // Sets the bit

    // Then we advance the bit pointer by more than 32 bits and display its position
    std::cout<< "--------" <<std::endl;
    bptr += 7;
    std::cout<<bptr->position()<<std::endl;                    // Prints 8 as the bit is now in the second element of the array
    std::cout<<*bptr<<std::endl;              // Prints ??
    bptr--;
    std::cout<<bptr->position()<<std::endl;                    // Prints 8 as the bit is now in the second element of the array
    std::cout<<*bptr<<std::endl;              // Prints ??

    // And finally we set the bit pointer to the position of the bit reference
    bptr = &bref;
    std::cout<<bptr->position()<<std::endl;                    // Prints 8 as the bit is now in the second element of the array
    std::cout<<*bptr<<std::endl;              // Prints ??
  }
//  {
//    // First, we create a list of integers
//    using uint_t = unsigned short int;
//    std::list<uint_t> intlst = {40, 41, 42, 43, 44};
//
//    // Then we create a pair of aligned bit iterators
//    auto bfirst = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::begin(intlst));
//    auto bend = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::end(intlst));
//
//    // Then we count the number of bits set to 1
//    auto result = std::count(bfirst, bend, stde::bit_value(1u));
//    std::cout<<result<<std::endl;                    // Prints
//
//    // We take a subset of the list
//    auto bfirst2 = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::begin(intlst), 5);
//    auto bend2 = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::prev(std::end(intlst)), 2);
//
//    // And we reverse the subset
//    std::reverse(bfirst2, bend2);
//  }

  return ::boost::report_errors();
}
