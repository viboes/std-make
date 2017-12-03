// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/chrono.hpp>

#include <experimental/chrono.hpp>

namespace stdex = std::experimental;

// Basic frame durations
using frames       = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<10>,  std::chrono::milliseconds::period>::type>;
using subframes    = std::chrono::duration<int32_t,  std::ratio_divide<frames::period,    std::ratio<10>>::type>;
using slots        = std::chrono::duration<int32_t,  std::ratio_divide<subframes::period, std::ratio<2>>::type>;
using symbols      = std::chrono::duration<int32_t,  std::ratio_divide<slots::period,     std::ratio<7>>::type>;

using bi_frames     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<2>,    frames::period>::type>;
using x_frames     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<1024>, frames::period>::type>;
using h_frames     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<1024>, x_frames::period>::type>;
using bi_slots      = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<2>,    slots::period>::type>;

// relative numbers
using h_frame_number    = stdex::chrono::modulo<x_frames,  h_frames,  uint16_t>; //1024
using frame_number      = stdex::chrono::modulo<frames,    x_frames,  uint16_t>; //1024
using x_frame_number    = frame_number;

using hx_frame_number    = stdex::chrono::modulo<frames,  h_frames,  uint16_t>; //1024*1024

using subframe_number   = stdex::chrono::modulo<subframes, frames,     uint8_t>; //10
using slot_number       = stdex::chrono::modulo<slots,     subframes,  uint8_t>; //2
using symbol_number     = stdex::chrono::modulo<symbols,   slots,      uint8_t>; //7
using bi_frame_number    = stdex::chrono::modulo<frames,    bi_frames,   uint8_t>; //2

using h_subframe_number = stdex::chrono::modulo<subframes, h_frames, uint32_t>; // 10*1240*1024
using x_subframe_number = stdex::chrono::modulo<subframes, x_frames, uint16_t>; // 10*1240
using bi_subframe_number = stdex::chrono::modulo<subframes, bi_frames, uint8_t>; // 10*2
using bi_symbol_number = stdex::chrono::modulo<symbols, bi_slots, uint8_t>;      // 7*2


inline constexpr h_subframe_number make_h_subframe_number(x_frame_number xfn, frame_number fn, subframe_number sfn) {
       return h_subframe_number(xfn.to_duration() + fn.to_duration() + sfn.to_duration()) ;
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



// what about defining x_subframe_tuple using tuples as underlying type.
// This will allow to define it as a strong type inheriting from all the relational operators

// x_subframe_tuple =  tuple<frame_number, subframe_number>
// x_subframe_tuple =  tuple<modulo<x_frames, frames>, modulo<frames, subframes>>
// todo: define a tuple_modulo where
// tuple_modulo<T1, T2, T3> = tuple<modulo<T1, T2>, modulo<T2, T3>>
//template <class T1, T2, T3>
//using tuple_modulo = std::tuple<modulo<T1, T2>, modulo<T2, T3>>;
//  convertible to T3 and modulo<T1, T3>
//  access to modulo<T1, T2> and modulo<T2, T3>
// tuple_modulo<T1, T2, T3, T4> = tuple<modulo<T1, T2>, modulo<T2, T3>, modulo<T3, T4>>
// x_subframe_tuple must define the same operations than x_subframe_number
struct x_subframe_tuple {
private:
    frame_number fn;
    subframe_number sfn;
public:

#if defined __clang__
    // gcc doesn't accepts this
    constexpr
#endif
    x_subframe_tuple() = default;

    /// pre-condition: fn and sfn are valid
    constexpr x_subframe_tuple(frame_number fn, subframe_number sfn) : fn(fn), sfn(sfn) {}
    /// pre-condition: xsfn is valid
    constexpr x_subframe_tuple(x_subframe_number xsfn) : fn(to_frame_number(xsfn)), sfn(to_subframe_number(xsfn)) {}

    /// pre-condition: fn  isvalid
    constexpr frame_number frame() const {return fn;}
    /// pre-condition: sfn is valid
    constexpr subframe_number subframe() const {return sfn;}
    /// pre-condition: fn and sfn are valid

    constexpr subframes get_duration() const { return frames(fn) + subframes(sfn); }
    constexpr x_subframe_number get_modulo() const { return x_subframe_number(get_duration()); }

    constexpr operator subframes() const { return get_duration(); }
    constexpr x_subframe_number x_subframe() const { return get_modulo(); }
    constexpr operator x_subframe_number() const { return get_modulo(); }

    // pre-condition 0 <= xsfn.frame() + df < 1024
    friend constexpr x_subframe_tuple operator+(x_subframe_tuple const& xsfn, const frames& df) noexcept
    {
        return { xsfn.fn + df, xsfn.sfn };
    }

    // pre-condition 0 <= xsfn.frame() - df < 1024
    friend constexpr x_subframe_tuple operator-(x_subframe_tuple const& xsfn, const frames& df) noexcept
    {
        return { xsfn.fn - df, xsfn.sfn };
    }
    // pre-condition 0 <= xsfn.frame() - df < 1024
    friend constexpr x_subframe_tuple operator+(const frames& df, x_subframe_tuple const& xsfn) noexcept
    {
        return xsfn + df;
    }

    friend constexpr subframes operator-(x_subframe_tuple const& x, const x_subframe_tuple& y) noexcept
    {
        return subframes(x) - subframes(y);
    }

    // pre-condition 0 <= xsfn.subframes() + dsf < 1024*10
    friend JASEL_CXX14_CONSTEXPR x_subframe_tuple operator+(x_subframe_tuple const& xsfn, const subframes& dsf) noexcept
    {
        const int cardinal = subframe_number::cardinal;
        auto dsfni = xsfn.sfn.count() + dsf.count();
        auto dfni = ((dsfni >= 0)  ? dsfni : dsfni-cardinal+1 ) / cardinal;
        dsfni = dsfni - dfni * cardinal;
        return x_subframe_tuple( xsfn.fn + frames(dfni), subframe_number(subframes(dsfni)) );
    }
    // pre-condition 0 <= xsfn.subframes() + dsf < 1024*10
    friend JASEL_CXX14_CONSTEXPR x_subframe_tuple operator+(const subframes& dsf, x_subframe_tuple const& xsfn) noexcept
    {
        return xsfn + dsf;
    }
    // pre-condition 0 <= xsfn.subframes() - dsf < 1024*10
    friend JASEL_CXX14_CONSTEXPR x_subframe_tuple operator-(x_subframe_tuple const& xsfn, const subframes& dsf) noexcept
    {
        return xsfn + -dsf;
    }

    // pre-condition 0 <= this->subframes() + df < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator+=(const frames& df) noexcept
    {
        *this = *this + df;
        return *this;
    }
    // pre-condition 0 <= this->subframes() - df < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator-=(const frames& df) noexcept
    {
        *this = *this - df;
        return *this;
    }

    // pre-condition 0 <= this->subframes() +dsf < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator+=(const subframes& dsf) noexcept
    {
        *this = *this + dsf;
        return *this;
    }
    // pre-condition 0 <= this->subframes() + subframe(1) < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator++() noexcept
    {
        *this = *this + subframes(1);
        return *this;
    }
    // pre-condition 0 <= this->subframes() + subframe(1) < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple operator++(int) noexcept
    {
        x_subframe_tuple tmp (*this);
        ++(*this);
        return tmp;
    }
    // pre-condition 0 <= this->subframes() - dsf < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator-=(const subframes& dsf) noexcept
    {
        *this = *this - dsf;
        return *this;
    }
    // pre-condition 0 <= this->subframes() - subframe(1) < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple& operator--() noexcept
    {
        *this = *this - subframes(1);
        return *this;
    }
    // pre-condition 0 <= this->subframes() - subframe(1) < 1024*10
    JASEL_CXX14_CONSTEXPR x_subframe_tuple operator--(int) noexcept
    {
        x_subframe_tuple tmp (*this);
        --(*this);
        return tmp;
    }
};

// operator/ play the role of a factory as it does on Date library.
// Note that there is no sense to divide a frame_number and subframe_number, we need to first convert to duration
x_subframe_tuple operator/(frame_number fn, subframe_number sfn)
{
    return {fn, sfn};
}

// Should the following be members?
// No because for modulo types it is a non-member function
inline constexpr h_frame_number to_h_frame_number(x_subframe_tuple )
{
    return h_frame_number(0);
}
inline constexpr x_subframe_number to_x_subframe_number(x_subframe_tuple xsfn)
{
    //return xsfn.x_subframe();
    return make_x_subframe_number(xsfn.frame(), xsfn.subframe());
}
inline constexpr frame_number to_frame_number(x_subframe_tuple xsfn)
{
    return xsfn.frame();
}
inline constexpr subframe_number to_subframe_number(x_subframe_tuple xsfn)
{
    return xsfn.subframe();
}
inline constexpr slot_number to_slot_number(x_subframe_tuple )
{
    return slot_number(0);
}
inline constexpr symbol_number to_symbol_number(x_subframe_tuple )
{
    return symbol_number(0);
}

struct h_subframe_tuple {
    x_frame_number xfn;
    frame_number fn;
    subframe_number sfn;
    h_subframe_tuple() = default;
    constexpr h_subframe_tuple(x_frame_number xfn, frame_number fn, subframe_number sfn) : xfn(xfn), fn(fn) , sfn(sfn) {}
    constexpr operator h_subframe_number() const { return make_h_subframe_number(xfn, fn, sfn); }
};

inline constexpr h_subframe_number to_h_frame_number(h_subframe_tuple hsfn)
{
    return h_subframe_number(hsfn);
}
inline constexpr x_frame_number to_x_frame_number(h_subframe_tuple hsfn)
{
    return hsfn.xfn;
}
inline constexpr frame_number to_frame_number(h_subframe_tuple hsfn)
{
    return hsfn.fn;
}
inline constexpr subframe_number to_subframe_number(h_subframe_tuple hsfn)
{
    return hsfn.sfn;
}
inline constexpr slot_number to_slot_number(h_subframe_tuple )
{
    return slot_number(0);
}
inline constexpr symbol_number to_symbol_number(h_subframe_tuple )
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
      slot_number slotn =  to_slot_number(xsfn);
      std::cout << "slotn = " << slotn << "\n";
      subframe_number sfn =  to_subframe_number(xsfn);
      std::cout << "sfn = " << sfn << "\n";
      frame_number fn =  to_frame_number(xsfn);
      std::cout << "fn = " << fn << "\n";
      x_frame_number xfn =  to_x_frame_number(xsfn);
      std::cout << "xfn = " << xfn << "\n";

      {
          x_subframe_number xsfn2 { frames(fn) + subframes(sfn) };
          std::cout << "xsfn2 = " << xsfn2 << "\n";
      }
      {
          auto xsfn2 = make_x_subframe_number( fn, sfn );
          std::cout << "xsfn2 = " << xsfn2 << "\n";
      }
      {
          x_subframe_number xsfn2 = make_x_subframe_number( fn, sfn );
          std::cout << "xsfn2 = " << xsfn2 << "\n";
      }
      {
          auto xsfn2 = fn.to_duration() + sfn.to_duration();
          std::cout << "xsfn2 = " << xsfn2.count() << "\n";
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
  }
  {
      frame_number fn {1};
      subframe_number sfn {2};
      x_subframe_tuple xsfn = fn/sfn;
      BOOST_TEST(xsfn.frame().count()==1);
      BOOST_TEST(xsfn.subframe().count()==2);
      BOOST_TEST(xsfn.x_subframe().count()==12);

      BOOST_TEST(to_frame_number(xsfn).count()==1);
      BOOST_TEST(to_subframe_number(xsfn).count()==2);
      BOOST_TEST(to_x_subframe_number(xsfn).count()==12);

      xsfn += frames(3);
      BOOST_TEST(xsfn.x_subframe().count()==42);
      BOOST_TEST(xsfn.frame().count()==4);

      xsfn += subframes(23);
      std::cout << "xsfn.x_subframe()= " << xsfn.x_subframe() << "\n";
      BOOST_TEST(xsfn.x_subframe().count()==65);

      ++xsfn;
      BOOST_TEST(xsfn.x_subframe().count()==66);

      --xsfn;
      BOOST_TEST(xsfn.x_subframe().count()==65);

      auto x1 = xsfn++;
      BOOST_TEST(x1.x_subframe().count()==65);
      BOOST_TEST(xsfn.x_subframe().count()==66);

      auto x2 = xsfn--;
      BOOST_TEST(x2.x_subframe().count()==66);
      BOOST_TEST(xsfn.x_subframe().count()==65);

  }
  {
      frame_number fn {1};
      subframe_number sfn {2};
      x_subframe_tuple xsfn = fn/sfn;
      auto x = xsfn - xsfn;
      BOOST_TEST(x.count()==0);

  }

  return ::boost::report_errors();
}
