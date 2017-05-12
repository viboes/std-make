// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/chrono.hpp>

#include <experimental/chrono.hpp>

namespace stdex = std::experimental;

// Basic frame durations
using frames       = std::chrono::duration<uint32_t,  std::ratio_multiply<std::ratio<10>,  std::chrono::milliseconds::period>::type>;
using subframes    = std::chrono::duration<uint32_t,  std::ratio_divide<frames::period,    std::ratio<10>>::type>;
using slots        = std::chrono::duration<uint32_t,  std::ratio_divide<subframes::period, std::ratio<2>>::type>;
using symbols      = std::chrono::duration<uint32_t,  std::ratio_divide<slots::period,     std::ratio<7>>::type>;

using bi_frames     = std::chrono::duration<uint32_t,  std::ratio_multiply<std::ratio<2>,    frames::period>::type>;
using x_frames     = std::chrono::duration<uint32_t,  std::ratio_multiply<std::ratio<1024>, frames::period>::type>;
using h_frames     = std::chrono::duration<uint32_t,  std::ratio_multiply<std::ratio<1024>, x_frames::period>::type>;
using bi_slots      = std::chrono::duration<uint32_t,  std::ratio_multiply<std::ratio<2>,    slots::period>::type>;

// relative numbers
using h_frame_number    = stdex::chrono::modulo<x_frames,  h_frames,  uint16_t>; //1024
using frame_number      = stdex::chrono::modulo<frames,    x_frames,  uint16_t>; //1024
using x_frame_number    = frame_number;

using hx_frame_number    = stdex::chrono::modulo<frames,  h_frames,  uint16_t>; //1024*1024

using subframe_number   = stdex::chrono::modulo<subframes, frames,     uint8_t>; //10
using slot_number       = stdex::chrono::modulo<slots,     subframes,  uint8_t>; //2
using symbol_number     = stdex::chrono::modulo<symbols,   slots,      uint8_t>; //7
using bi_frame_number    = stdex::chrono::modulo<frames,    bi_frames,   uint8_t>; //2

using h_subframe_number = stdex::chrono::modulo<subframes, h_frames, uint16_t>; // 10*1240*1024
using x_subframe_number = stdex::chrono::modulo<subframes, x_frames, uint16_t>; // 10*1240
using bi_subframe_number = stdex::chrono::modulo<subframes, bi_frames, uint8_t>; // 10*2
using bi_symbol_number = stdex::chrono::modulo<symbols, bi_slots, uint8_t>;      // 7*2


inline constexpr h_subframe_number make_h_subframe_number(h_frame_number hfn, x_frame_number xfn, subframe_number sfn) {
       return h_subframe_number(hfn.to_duration() + xfn.to_duration() + sfn.to_duration()) ;
}


inline constexpr x_subframe_number make_x_subframe_number(frame_number fn, subframe_number sfn) {
       return x_subframe_number(fn.to_duration() + sfn.to_duration()) ;
}

inline constexpr bi_subframe_number make_bi_subframe_number(bi_frame_number fn, subframe_number sfn) {
       return bi_subframe_number(fn.to_duration() + sfn.to_duration()) ;
}

inline constexpr bi_symbol_number make_bi_subframe_number(slot_number sn, symbol_number syn) {
       return bi_symbol_number(sn.to_duration() + syn.to_duration()) ;
}

template <class SubDuration, class Duration, class Rep>
constexpr x_frame_number to_x_frame_number(stdex::chrono::modulo<SubDuration, Duration, Rep> m)
{
  return stdex::chrono::modulo_cast<x_frame_number, h_frames>(m);

}

template <class SubDuration, class Duration, class Rep>
constexpr frame_number to_frame_number(stdex::chrono::modulo<SubDuration, Duration, Rep> m)
{
    return stdex::chrono::modulo_cast<frame_number, h_frames>(m);
}

template <class SubDuration, class Duration, class Rep>
constexpr subframe_number to_subframe_number(stdex::chrono::modulo<SubDuration, Duration, Rep> m)
{
  return stdex::chrono::modulo_cast<subframe_number, h_frames>(m);
}

template <class SubDuration, class Duration, class Rep>
constexpr slot_number to_slot_number(stdex::chrono::modulo<SubDuration, Duration, Rep> m)
{
  return stdex::chrono::modulo_cast<slot_number, h_frames>(m);
}

template <class SubDuration, class Duration, class Rep>
constexpr symbol_number to_symbol_number(stdex::chrono::modulo<SubDuration, Duration, Rep> m)
{
  return stdex::chrono::modulo_cast<symbol_number, h_frames>(m);
}

struct x_subframe_numbers {
    frame_number fn;
    subframe_number sfn;
    x_subframe_numbers() = default;
    constexpr x_subframe_numbers(frame_number fn, subframe_number sfn) : fn(fn), sfn(sfn) {}
    constexpr operator x_subframe_number() { return make_x_subframe_number(fn, sfn); }
    //explicit operator bi_subframe_number() { return make_bi_subframe_number(fn, sfn); }
};

// Should the following be members?
// No because for modulo types it is a non-member function
inline constexpr h_frame_number to_h_frame_number(x_subframe_numbers xsfn)
{
    return h_frame_number(0);
}
inline constexpr frame_number to_frame_number(x_subframe_numbers xsfn)
{
    return xsfn.fn;
}
inline constexpr subframe_number to_subframe_number(x_subframe_numbers xsfn)
{
    return xsfn.sfn;
}
inline constexpr slot_number to_slot_number(x_subframe_numbers xsfn)
{
    return slot_number(0);
}
inline constexpr symbol_number to_symbol_number(x_subframe_numbers xsfn)
{
    return symbol_number(0);
}

struct h_subframe_numbers {
    h_frame_number hfn;
    x_frame_number xfn;
    subframe_number sfn;
    h_subframe_numbers() = default;
    constexpr h_subframe_numbers(h_frame_number hfn, x_frame_number xfn, subframe_number sfn) : hfn(hfn), xfn(xfn) , sfn(sfn) {}
    constexpr operator h_subframe_number() { return make_h_subframe_number(hfn, xfn, sfn); }
};

inline constexpr h_frame_number to_h_frame_number(h_subframe_numbers hsfn)
{
    return hsfn.hfn;
}
inline constexpr frame_number to_frame_number(h_subframe_numbers hsfn)
{
    return hsfn.xfn;
}
inline constexpr subframe_number to_subframe_number(h_subframe_numbers hsfn)
{
    return hsfn.sfn;
}
inline constexpr slot_number to_slot_number(h_subframe_numbers hsfn)
{
    return slot_number(0);
}
inline constexpr symbol_number to_symbol_number(h_subframe_numbers hsfn)
{
    return symbol_number(0);
}

static_assert(frame_number::cardinal  == 1024, "1024 frames are not a x_frame");
static_assert(subframe_number::cardinal  == 10, "10 sub frames are not a frame");
static_assert(bi_subframe_number::cardinal  == 20, "20 sub frames are not a bi_frame");

static_assert(slots::period::num  == 1, "");
static_assert(slots::period::den  == 2000, "");
static_assert(symbols::period::num  == 1, "");
static_assert(symbols::period::den  == 7*2000, "");

//! Auxiliary functions
inline constexpr bool parity(bi_subframe_number x) {
  return to_frame_number(x) == frame_number{0};
}

// todo Add literals
// h_frames -> _hfs
// frames -> _xfs
// subframes -> _sfs

// frame_number -> _fn
// subframe_number -> _sfn
// slot_number -> _sn
// symbol_number -> _syn



#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
  {
    constexpr   x_subframe_number xsfn =  make_x_subframe_number(frame_number{5}, subframe_number{3});
    static_assert(xsfn.count()  == 53, "");

  }
  subframes num_of_sfs{4};
  num_of_sfs++;
  frames num_of_fs{2};
  num_of_fs++;
  auto x = num_of_fs + num_of_sfs;
  std::cout << "sfn = " << x.count() << "\n";
  {
      x_subframe_number xsfn {x};
      std::cout << "xsfn = " << xsfn << "\n";
      slot_number slotn =  to_slot_number(xsfn);
      std::cout << "slotn = " << slotn << "\n";
      subframe_number sfn =  to_subframe_number(xsfn);
      std::cout << "sfn = " << sfn << "\n";
      frame_number fn =  to_frame_number(xsfn);
      std::cout << "fn = " << fn << "\n";
      x_frame_number xfn =  to_x_frame_number(xsfn);
      std::cout << "xfn = " << xfn << "\n";

      {
          x_subframe_number x { frames(fn) + subframes(sfn) };
          std::cout << "x = " << x << "\n";
      }
      {
          auto x = make_x_subframe_number( fn, sfn );
          std::cout << "x = " << x << "\n";
      }

      {
          x_subframe_number x = make_x_subframe_number( fn, sfn );
          std::cout << "x = " << x << "\n";
      }

      {
          auto x = fn.to_duration() + sfn.to_duration();
          std::cout << "x = " << x.count() << "\n";
      }

  }

  subframe_number sfn (num_of_sfs) ;
  frame_number fn (num_of_fs) ;
  bi_frame_number bfn (num_of_fs) ;
  bi_subframe_number bsfn = make_bi_subframe_number(bfn,sfn) ;
  std::cout << "bsfn = " << bsfn << "\n";

  std::cout << "==================\n";

  x_subframe_number  x2 = make_x_subframe_number( fn, sfn );
  std::cout << "xsfn2= " << x2 << "\n";
  bi_subframe_number bsfn2 = stdex::chrono::modulo_cast<bi_subframe_number, h_frames>(x2) ;
      std::cout << "bsfn2= " << bsfn2 << "\n";

  bi_frame_number bfn2 = stdex::chrono::modulo_cast<bi_frame_number, h_frames>(x2) ;
  std::cout << "bfn2= " << bfn2 << "\n";

  x_subframe_number x3 = stdex::chrono::modulo_cast<x_subframe_number, h_frames>(bfn2) ;
  std::cout << "x3= " << x3 << "\n";

  x_subframe_number x4 = stdex::chrono::modulo_cast<x_subframe_number, h_frames>(bsfn2) ;
  std::cout << "x4= " << x4 << "\n";


  return ::boost::report_errors();
}
