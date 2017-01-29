// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/chrono.hpp>
#if __cplusplus >= 201402L

#include <experimental/chrono.hpp>

namespace stdex = std::experimental;

// Basic frame durations
using x_frames     = std::chrono::duration<uint32_t,  std::ratio<1024*10,      1000>>;
using frames       = std::chrono::duration<uint32_t,  std::ratio<10,           1000>>;
using biframes     = std::chrono::duration<uint32_t,  std::ratio<2*10,         1000>>;
using subframes    = std::chrono::duration<uint32_t,  std::ratio<1,            1000>>;
using slots        = std::chrono::duration<uint32_t,  std::ratio<1,          2*1000>>;
using bislots      = std::chrono::duration<uint32_t,  std::ratio<2*1,        2*1000>>;
using symbols      = std::chrono::duration<uint32_t,  std::ratio<1,        7*2*1000>>;

// relative numbers
using frame_number      = stdex::chrono::modulo<frames,    x_frames,  uint16_t>; //1024
using subframe_number   = stdex::chrono::modulo<subframes, frames,     uint8_t>; //10
using slot_number       = stdex::chrono::modulo<slots,     subframes,  uint8_t>; //2
using symbol_number     = stdex::chrono::modulo<symbols,   slots,      uint8_t>; //7
using biframe_number    = stdex::chrono::modulo<frames,    biframes,   uint8_t>; //2



using x_subframe_number = stdex::chrono::modulo<subframes, x_frames, uint16_t>; // 10240

x_subframe_number make_x_subframe_number(frame_number fn, subframe_number sfn) {
       return x_subframe_number(fn+sfn) ;
}

frame_number to_frame_number(x_subframe_number xsfn) {
    return frame_number(std::chrono::duration_cast<frames>(subframes(xsfn)));
}

subframe_number to_subframe_number(x_subframe_number xsfn) {
    return subframe_number(subframes(xsfn));
}

using bi_subframe_number = stdex::chrono::modulo<subframes, biframes, uint8_t>; // 2

bi_subframe_number make_bi_subframe_number(biframe_number fn, subframe_number sfn) {
       return bi_subframe_number(subframes(fn+sfn)) ;
}

biframe_number to_frame_number(bi_subframe_number bsfn) {
    return biframe_number(std::chrono::duration_cast<biframes>(subframes(bsfn)));
}

subframe_number to_subframe_number(bi_subframe_number bsfn) {
    return subframe_number(subframes(bsfn));
}

struct x_subframe_numbers {
    frame_number fn;
    subframe_number sfn;
    x_subframe_numbers() {}
    x_subframe_numbers(frame_number fn, subframe_number sfn) : fn(fn), sfn(sfn) {}
    explicit operator x_subframe_number() { return make_x_subframe_number(fn, sfn); }
    //explicit operator bi_subframe_number() { return make_bi_subframe_number(fn, sfn); }
};


using bi_symbol_number = stdex::chrono::modulo<symbols, bislots, uint8_t>; // 2

bi_symbol_number make_bi_subframe_number(slot_number sn, symbol_number syn) {
       return bi_symbol_number(symbols(sn + syn)) ;
}

slot_number to_slot_number(bi_symbol_number bsn) {
    return slot_number(std::chrono::duration_cast<slots>(symbols(bsn)));
}

symbol_number to_symbol_number(bi_symbol_number bsn) {
    return symbol_number(symbols(bsn));
}

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

static_assert(frame_number::period  == 1024, "1024 frames are not a x_frame");
static_assert(subframe_number::period  == 10, "10 sub frames are not a frame");
static_assert(bi_subframe_number::period  == 20, "20 sub frames are not a biframe");

static_assert(slots::period::num  == 1, "");
static_assert(slots::period::den  == 2000, "");
static_assert(symbols::period::num  == 1, "");
static_assert(symbols::period::den  == 7*2000, "");

int main()
{
  subframes num_of_sfs{4};
  num_of_sfs++;
  frames num_of_fs{2};
  num_of_fs++;
  auto x = num_of_fs + num_of_sfs;
  std::cout << "sfn = " << x.count() << "\n";
  {
      x_subframe_number xsfn {x};
      std::cout << "xsfn = " << xsfn << "\n";
      subframe_number sfn =  to_subframe_number(xsfn);
      std::cout << "sfn = " << sfn << "\n";
      frame_number fn =  to_frame_number(xsfn);
      std::cout << "fn = " << fn << "\n";

      {
          x_subframe_number x { frames(fn) + subframes(sfn) };
          std::cout << "x = " << x << "\n";
      }
      {
          auto x = fn + sfn;
          std::cout << "x = " << x << "\n";
      }

      {
          x_subframe_number x { fn + sfn };
          std::cout << "x = " << x << "\n";
      }

      {
          auto x = fn.get_duration() + sfn.get_duration();
          std::cout << "x = " << x.count() << "\n";
      }

  }

  subframe_number sfn (num_of_sfs) ;
  frame_number fn (num_of_fs) ;
  biframe_number bfn (num_of_fs) ;
  bi_subframe_number bsfn = make_bi_subframe_number(bfn,sfn) ;
  std::cout << "bsfn = " << bsfn << "\n";

  std::cout << "==================\n";

  x_subframe_number  x2 { fn + sfn};
  bi_subframe_number bsfn2 = stdex::chrono::modulo_cast<bi_subframe_number>(x2) ;
      std::cout << "bsfn2= " << bsfn2 << "\n";

  biframe_number bfn2 = stdex::chrono::modulo_cast<biframe_number>(x2) ;
  std::cout << "bfn2= " << bfn2 << "\n";

  x_subframe_number x3 = stdex::chrono::modulo_cast<x_subframe_number>(bfn2) ;
  std::cout << "x3= " << x3 << "\n";

  x_subframe_number x4 = stdex::chrono::modulo_cast<x_subframe_number>(bsfn2) ;
  std::cout << "x4= " << x4 << "\n";


  return ::boost::report_errors();
}

#else
int main() { return 0; }
#endif
