// Copyright (C) 2016 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/bit_mask.hpp>

#include <experimental/bit.hpp>

#include <boost/detail/lightweight_test.hpp>

#include <array>
#include <list>

namespace stde = std::experimental;

int main()
{
  {
    // First, we create an integer
    using uint_t = unsigned long long int;
    uint_t intval = 42;                                        // 101010

    // Then we create aligned bit values and a bit references on this integer
    stde::bit_value bval0(intval);                              // Creates a bit value from the bit at position 0 of intval
    stde::bit_reference<uint_t> bref0(intval);                  // Creates a bit reference from the bit at position 0 of intval

    // And unaligned bit values and a bit references on this integer
    stde::bit_value bval5(intval, 5);                           // Creates a bit value from the bit at position 5 of intval
    stde::bit_reference<uint_t> bref5(intval, 5);               // Creates a bit reference from the bit at position 5 of intval

    // Display them
    std::cout<<bval0<<bref0<<bval5<<bref5<<std::endl;          // Prints 0011

    // Change their values conditionnally
    if (static_cast<bool>(bval5)) {
      bval0.flip();  // Flips the bit without affecting the integer
      bref5.reset(); // Resets the bit to zero and affects the integer
    }
    std::cout<<bval0<<bref0<<bval5<<bref5<<std::endl;          //  Prints 1010

    // Prints the location and the corresponding mask of bit references
    std::cout<<bref0.position()<<" "<<bref0.mask()<<std::endl; // Prints 0 and 1
    std::cout<<bref5.position()<<" "<<bref5.mask()<<std::endl; // Prints 5 and 32
  }
  {
    // First, we create an array of integers
    using uint_t = unsigned long long int;
    std::array<uint_t, 2> intarr = {{42, 314}};

    // Then we create a bit reference and a bit pointer
    stde::bit_reference<uint_t> bref5(intarr[0], 5);            // Creates a bit reference from the bit at position 5 of the first element of the array
    stde::bit_pointer<uint_t> bptr(intarr.data());              // Creates a bit pointer from the bit at position 0 of the first element of the array
    std::cout<<bptr->position()<<std::endl;                    // Prints 7 as the bit is now in the second element of the array

    // We flip the first bit, and sets the second one to 1 with two methods
    bptr->flip();                                              // Flips the bit
    ++bptr;                                                    // Goes to the next bit
    (*bptr).set();                                               // Sets the bit

    // Then we advance the bit pointer by more than 64 bits and display its position
    bptr += 71;
    std::cout<<bptr->position()<<std::endl;                    // Prints 8 as the bit is now in the second element of the array

    // And finally we set the bit pointer to the position of the bit reference
    bptr = &bref5;
  }
  {
    // First, we create a list of integers
    using uint_t = unsigned short int;
    std::list<uint_t> intlst = {40, 41, 42, 43, 44};

    // Then we create a pair of aligned bit iterators
    auto bfirst = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::begin(intlst));
    auto bend = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::end(intlst));

    // Then we count the number of bits set to 1
    auto result = std::count(bfirst, bend, stde::bit_value(1u));
    std::cout<<result<<std::endl;                    // Prints

    // We take a subset of the list
    auto bfirst2 = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::begin(intlst), 5);
    auto bend2 = stde::bit_iterator<typename std::list<uint_t>::iterator>(std::prev(std::end(intlst)), 2);

    // And we reverse the subset
    std::reverse(bfirst2, bend2);
  }

  return ::boost::report_errors();
}
