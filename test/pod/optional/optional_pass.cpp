// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_id.hpp>

#include <experimental/pod/optional.hpp>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

static_assert(std::is_pod<stdex::pod::optional<int>>::value, "pod::optional<int> is not a POD");
static_assert(sizeof(stdex::pod::optional<char, std::int8_t>) == 2, "sizeof(stdex::pod::optional<char, std::int8_t>)  must be 2");
static_assert(sizeof(stdex::pod::optional<int, std::int8_t>) == 8, "sizeof(stdex::pod::optional<int, std::int8_t>)  must be 8");

int main()
{
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
      stdex::pod::optional<int> oi;
      oi = stdex::pod::optional<int>{1};
      BOOST_TEST (*oi == 1);

      oi = stdex::pod::nullopt;
      BOOST_TEST (!oi);

      oi = 2;
      BOOST_TEST (*oi == 2);

      oi = {}; // fails !oi - function is called?
      oi.reset();
      BOOST_TEST (!oi);
  }
  {
    stdex::pod::optional<int> oi = {}; // no default constructor
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

    stdex::pod::optional<int> oi;
    auto ooi = stdex::pod::make_optional(oi);
    static_assert( std::is_same<stdex::pod::optional<stdex::pod::optional<int>>, decltype(ooi)>::value, "");

  }

  return ::boost::report_errors();
}
