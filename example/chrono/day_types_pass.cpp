// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/chrono.hpp>

#include <experimental/chrono.hpp>
#include <experimental/ordinal_array.hpp>
#include <experimental/ordinal_range.hpp>
#include <experimental/ordinal_set.hpp>

namespace stdex = std::experimental;

// Basic frame durations
using half_days     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<12>, std::chrono::hours::period>::type>;
using days     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>::type>;
using weeks    = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<7>, days::period>::type>;
using years    = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<146097, 400>, days::period>::type>;
using months   = std::chrono::duration<int32_t,  std::ratio_divide<years::period, std::ratio<12>>::type>;


// relative numbers
using weekday = stdex::chrono::modulo<days,   weeks,    std::uint8_t>; //7
using am_pm   = stdex::chrono::modulo<half_days,   days,    std::uint8_t>; //24
using hour   = stdex::chrono::modulo<std::chrono::hours,   days,    std::uint8_t>; //24
using am_pm_hour   = stdex::chrono::modulo<std::chrono::hours,   half_days,    std::uint8_t>; //12
using minute = stdex::chrono::modulo<std::chrono::minutes, std::chrono::hours,   std::uint8_t>; //60
using second = stdex::chrono::modulo<std::chrono::seconds, std::chrono::minutes, std::uint8_t>; //60

template <class Duration>
using time_of_day = stdex::chrono::modulo<Duration, days, std::int32_t >;


template <class Duration>
std::chrono::hours to_hours(time_of_day<Duration> m)
{
    using ModuloTo = stdex::chrono::modulo<std::chrono::hours,   days,    std::uint8_t>;
    return stdex::chrono::to_modulo<ModuloTo>(m).to_duration();
}

template <class ModuloFrom>
std::chrono::minutes to_minutes(ModuloFrom m)
{
    using MinuteDay = stdex::chrono::modulo<std::chrono::minutes,   days,    std::int32_t>;

    auto _hours =  to_hours(m);
    return stdex::chrono::to_modulo<MinuteDay>(m).to_duration()-_hours;
}

template <class ModuloFrom>
std::chrono::seconds to_seconds(ModuloFrom m)
{
  using SecondDay   = stdex::chrono::modulo<std::chrono::seconds,   days,    std::int32_t>;

  auto _hours =  to_hours(m);
  auto _min =  to_minutes(m);
  return stdex::chrono::to_modulo<SecondDay>(m).to_duration() - _min - _hours;
}

template <class ModuloFrom>
hour to_hour(ModuloFrom m)
{
    return hour(to_hours(m));
}
template <class ModuloFrom>
am_pm to_am_pm(ModuloFrom m)
{
    return stdex::chrono::to_modulo<am_pm>(m);
}
template <class ModuloFrom>
am_pm_hour to_am_pm_hour(ModuloFrom m)
{
    return stdex::chrono::to_modulo<am_pm_hour>(m);
}

template <class ModuloFrom>
minute to_minute(ModuloFrom m)
{
  return minute(to_minutes(m));
}

template <class ModuloFrom>
second to_second(ModuloFrom m)
{
  return second(to_seconds(m));
}
static_assert(am_pm_hour::cardinal  == 12, "12 hours are not a half-day");
static_assert(am_pm::cardinal  == 2, "2 half-days are not a day");
static_assert(hour::cardinal  == 24, "24 hours are not a day");
static_assert(minute::cardinal  == 60, "60 minutes are not an hour");
static_assert(second::cardinal  == 60, "60 seconds are not a minute");
static_assert(hour::min()  == hour{0}, "0 is not the min of hour");
static_assert(hour::max()  == hour{23}, "23 is not the min of hour");

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
  {
                                            //   ms        sec       min  h
    time_of_day<std::chrono::milliseconds> t2{ 234+ 1000*(2 + 60 * ( 4 + 8 * 60 ) )};
    std::cout << "t2 = " << t2.count() << "\n";
      time_of_day<std::chrono::milliseconds> t{ std::chrono::milliseconds(234)+ std::chrono::seconds(2) + std::chrono::minutes(4) + std::chrono::hours(20)};
      std::cout << "t = " << t.count() << "\n";

      std::cout << "am_pm = " << int(to_am_pm(t).count()) << "\n";
      std::cout << "am_pm_hour = " << int(to_am_pm_hour(t).count()) << "\n";
      std::cout << "hour = " << int(to_hours(t).count()) << "\n";
      std::cout << "minute = " << int(to_minutes(t).count()) << "\n";
      std::cout << "second = " << int(to_seconds(t).count()) << "\n";

      BOOST_TEST(to_am_pm(t).count() == 1);
      BOOST_TEST(to_am_pm_hour(t).count() == 8);
      BOOST_TEST(to_hour(t).count() == 20);
      BOOST_TEST_EQ(to_minute(t).count(), 4);
      BOOST_TEST_EQ(to_second(t).count(), 2);
  }
  {
      weekday wd{2};
      BOOST_TEST(wd.count() == 2);
  }
  {
    stdex::ordinal_array<int, weekday> arr;
    BOOST_TEST(arr.size() == 7);
  }
  {
    stdex::ordinal_array<int, weekday> c {{0, 1, 2, 3, 4, 5, 6}};
    BOOST_TEST(c.size() == 7);
    BOOST_TEST(c[weekday{1}] == 1);
    BOOST_TEST(c[weekday{2}] == 2);
    BOOST_TEST(c[weekday{3}] == 3);
  }
  {
    stdex::ordinal_range<weekday> rng;
    auto b = rng.begin();
    BOOST_TEST(b->count()==0);
    for (auto w : rng)
    {
      std::cout << w << "\n";
    }
  }
  {
      stdex::ordinal_set<weekday> os;
      os[weekday{1}]=true;
      BOOST_TEST( 1 == os.count() );
      BOOST_TEST( os.any() );
      BOOST_TEST(os[weekday{1}]);
  }
  return ::boost::report_errors();
}
