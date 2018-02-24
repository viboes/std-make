// Copyright (C) 2017-2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_id.hpp>

#include <experimental/pod/optional.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

template <class T> struct show;

static_assert(std::is_pod<stdex::pod::optional<int>>::value, "pod::optional<int> is not a POD");
static_assert(sizeof(stdex::pod::optional<char, std::int8_t>) == 2, "sizeof(stdex::pod::optional<char, std::int8_t>)  must be 2");
static_assert(sizeof(stdex::pod::optional<char>) == 2, "sizeof(stdex::pod::optional<char>)  must be 2");
static_assert(sizeof(stdex::pod::optional<int, std::int8_t>) == 8, "sizeof(stdex::pod::optional<int, std::int8_t>)  must be 8");


struct P {
  short x;
  short y;
};

static_assert(std::is_pod<stdex::pod::optional<P>>::value, "pod::optional<P> is not a POD");
static_assert(std::is_pod<stdex::pod::optional<P, std::int16_t>>::value, "pod::optional<P> is not a POD");
static_assert(sizeof(stdex::pod::optional<P>) == 6, "sizeof(stdex::pod::optional<P>)  must be 6");
static_assert(sizeof(stdex::pod::optional<P, std::int16_t>) == 6, "sizeof(stdex::pod::optional<P, std::int16_t>)  must be 6");

#include <boost/endian/conversion.hpp>
#include <boost/endian/arithmetic.hpp>

using big_int16_t = boost::endian::big_int16_t;

static_assert(std::is_pod<stdex::pod::optional<big_int16_t>>::value, "pod::optional<big_int16_t> is not a POD");
static_assert(sizeof(stdex::pod::optional<big_int16_t>) == 3, "sizeof(stdex::pod::optional<char>)  must be 3");

int main()
{
  {
    stdex::pod::optional<int> o1 = {};
    BOOST_TEST (!o1);
  }
  {
    stdex::pod::optional<big_int16_t> o1 = {};
    BOOST_TEST (!o1);
  }
  {
    stdex::pod::optional<P> o1 = {};
    BOOST_TEST (!o1);
  }
  {
    stdex::pod::optional<P> o1{};
    BOOST_TEST (!o1);
  }
  {
    stdex::pod::optional<int> o1;
    o1 = std::nullopt;

    stdex::pod::optional<int> o2 = std::nullopt;
    BOOST_TEST (!o2);

    stdex::pod::optional<int> o3;
    o3 = o2;
    BOOST_TEST (!o3);

    BOOST_TEST (o1 == stdex::pod::nullopt);
    BOOST_TEST (o1 == stdex::pod::optional<int>{});
    BOOST_TEST (!o1);
    BOOST_TEST (bool(o1) == false);

    BOOST_TEST (o2 == stdex::pod::nullopt);
    BOOST_TEST (o2 == stdex::pod::optional<int>{});
    BOOST_TEST (!o2);
    BOOST_TEST (bool(o2) == false);

    BOOST_TEST (o3 == stdex::pod::nullopt);
    BOOST_TEST (o3 == stdex::pod::optional<int>{});
    BOOST_TEST (!o3);
    BOOST_TEST (bool(o3) == false);

    BOOST_TEST (o1 == o2);
    BOOST_TEST (o2 == o1);
    BOOST_TEST (o1 == o3);
    BOOST_TEST (o3 == o1);
    BOOST_TEST (o2 == o3);
    BOOST_TEST (o3 == o2);

  }
  {
    stdex::pod::optional<big_int16_t> o1;
    o1 = std::nullopt;

    stdex::pod::optional<big_int16_t> o2 = std::nullopt;
    BOOST_TEST (!o2);

    stdex::pod::optional<big_int16_t> o3;
    o3 = o2;
    BOOST_TEST (!o3);

    BOOST_TEST (o1 == stdex::pod::nullopt);
    BOOST_TEST (o1 == stdex::pod::optional<big_int16_t>{});
    BOOST_TEST (!o1);
    BOOST_TEST (bool(o1) == false);

    BOOST_TEST (o2 == stdex::pod::nullopt);
    BOOST_TEST (o2 == stdex::pod::optional<big_int16_t>{});
    BOOST_TEST (!o2);
    BOOST_TEST (bool(o2) == false);

    BOOST_TEST (o3 == stdex::pod::nullopt);
    BOOST_TEST (o3 == stdex::pod::optional<big_int16_t>{});
    BOOST_TEST (!o3);
    BOOST_TEST (bool(o3) == false);

    BOOST_TEST (o1 == o2);
    BOOST_TEST (o2 == o1);
    BOOST_TEST (o1 == o3);
    BOOST_TEST (o3 == o1);
    BOOST_TEST (o2 == o3);
    BOOST_TEST (o3 == o2);

  }

  {
      stdex::pod::optional<int> oi;
      oi = stdex::pod::optional<int>{1};
      BOOST_TEST (*oi == 1);

      oi = stdex::pod::nullopt;
      BOOST_TEST (!oi);

      oi = 2;
      BOOST_TEST (oi);
      BOOST_TEST (*oi == 2);

      oi.reset();
      BOOST_TEST (!oi);
      oi = 2;
      BOOST_TEST (oi);
      oi = {}; // fixme: fails !oi - function is called?
      BOOST_TEST (!bool(oi));
      BOOST_TEST (!oi);
  }
#if defined JASEL_STD_HAVE_OPTIONAL
  {
      std::optional<int> oi;
      oi = std::optional<int>{1};
      BOOST_TEST (*oi == 1);

      oi = std::nullopt;
      BOOST_TEST (!oi);

      oi = 2;
      BOOST_TEST (*oi == 2);
      oi = {};
      BOOST_TEST (!oi);
      oi = 2;
      BOOST_TEST (oi);
      oi.reset();
      BOOST_TEST (!oi);
  }
  {
      std::optional<big_int16_t> oi;
      oi = std::optional<big_int16_t>{1};
      BOOST_TEST (*oi == 1);

      oi = std::nullopt;
      BOOST_TEST (!oi);

      oi = 2;
      BOOST_TEST (*oi == 2);
      oi = {};
      BOOST_TEST (!oi);
      oi = 2;
      BOOST_TEST (oi);
      oi.reset();
      BOOST_TEST (!oi);
  }
#endif
  {
      stdex::pod::optional<big_int16_t> oi;
      oi = stdex::pod::optional<big_int16_t>{1};
      BOOST_TEST (*oi == 1);

      oi = stdex::pod::nullopt;
      BOOST_TEST (!oi);

      oi = 2;
      BOOST_TEST (*oi == 2);
      oi = {};
      BOOST_TEST (!oi);
      oi = 2;
      BOOST_TEST (oi);
      oi.reset();
      BOOST_TEST (!oi);
  }
  {
    constexpr stdex::pod::optional<int> oi = {}; // no default constructor
    BOOST_TEST (!oi);
  }
  {
    constexpr stdex::pod::optional<big_int16_t> oi = {}; // no default constructor
    BOOST_TEST (!oi);
  }
  {
    stdex::pod::optional<int> oi = {}; // no default constructor
    stdex::pod::optional<int> oj = oi;

    BOOST_TEST (!oj);
    BOOST_TEST (oj == oi);
    BOOST_TEST (oj == stdex::pod::nullopt);
    BOOST_TEST (!bool(oj));

    oi = 1;
    stdex::pod::optional<int> ok = oi;
    BOOST_TEST (!!ok);
    BOOST_TEST (bool(ok));
    BOOST_TEST (ok == oi);
    BOOST_TEST (ok != oj);
    BOOST_TEST (*ok == 1);

    stdex::pod::optional<int> ol = std::move(oi);
    BOOST_TEST (!!ol);
    BOOST_TEST (bool(ol));
    BOOST_TEST (ol == oi);
    BOOST_TEST (ol != oj);
    BOOST_TEST (*ol == 1);
  }
  {
    stdex::pod::optional<int> oi {stdex::pod::in_place};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (*oi == 0);
    BOOST_TEST (bool(oi));
  }
  {
    stdex::pod::optional<big_int16_t> oi {stdex::pod::in_place};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (*oi == 0);
    BOOST_TEST (bool(oi));
  }
  {
    stdex::pod::optional<int> oi {stdex::pod::in_place, 1};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (bool(oi));
  }
  {
    stdex::pod::optional<big_int16_t> oi {stdex::pod::in_place, 1};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (bool(oi));
  }
  {
    stdex::pod::optional<P> oi {stdex::pod::in_place};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (bool(oi));
  }
#if 0
  // fixme: needs a different in_place that does aggregate initialization
  {
    stdex::pod::optional<P> oi {stdex::pod::in_place, 1, 2};
    BOOST_TEST (oi != stdex::pod::nullopt);
    BOOST_TEST (bool(oi));
  }
#endif
  {
    stdex::pod::optional<stdex::pod::optional<int>> oi1 = stdex::pod::nullopt;
    BOOST_TEST (oi1 == stdex::pod::nullopt);
    BOOST_TEST (!oi1);

    {
    stdex::pod::optional<stdex::pod::optional<int>> oi2 {stdex::pod::in_place};
    BOOST_TEST (oi2 != stdex::pod::nullopt);
    BOOST_TEST (bool(oi2));
    BOOST_TEST (*oi2 == stdex::pod::nullopt);
    }
    {
    stdex::pod::optional<stdex::pod::optional<P>> oi2 {stdex::pod::in_place};
    BOOST_TEST (oi2 != stdex::pod::nullopt);
    BOOST_TEST (bool(oi2));
    BOOST_TEST (*oi2 == stdex::pod::nullopt);
    }

    {
    stdex::pod::optional<stdex::pod::optional<int>> oi2 {stdex::pod::in_place, stdex::pod::nullopt};
    BOOST_TEST (oi2 != stdex::pod::nullopt);
    BOOST_TEST (bool(oi2));
    BOOST_TEST (*oi2 == stdex::pod::nullopt);
    BOOST_TEST (!*oi2);
    }

    {
    stdex::pod::optional<stdex::pod::optional<int>> oi2 {stdex::pod::optional<int>{}};
    BOOST_TEST (oi2 != stdex::pod::nullopt);
    BOOST_TEST (bool(oi2));
    BOOST_TEST (*oi2 == stdex::pod::nullopt);
    BOOST_TEST (!*oi2);
    }

    constexpr stdex::pod::optional<int> oi = {};
    constexpr auto ooi = stdex::pod::make_optional(oi);
    static_assert( std::is_same<const stdex::pod::optional<stdex::pod::optional<int>>, decltype(ooi)>::value, "");

    {


    }
  }

  return ::boost::report_errors();
}
