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
// The interface provided here differs to the one provided in p0355r4
// Missing interfaces
//constexpr weekday_number(const sys_days& dp) noexcept;
//constexpr explicit weekday_number(const local_days& dp) noexcept;
//constexpr explicit operator unsigned() const noexcept;
//constexpr bool ok() const noexcept;
//constexpr weekday_indexed operator[](unsigned index) const noexcept;
//constexpr weekday_last    operator[](last_spec) const noexcept;
// operator<<(OSTREAM) differes

using month_number = stdex::chrono::modulo<months,   years,    std::uint8_t>; //12
using weekday_number = stdex::chrono::modulo<days,   weeks,    std::uint8_t>; //7
using am_pm_number   = stdex::chrono::modulo<half_days,   days,    std::uint8_t>; //2
using hour_number   = stdex::chrono::modulo<std::chrono::hours,   days,    std::uint8_t>; //24
using am_pm_hour   = stdex::chrono::modulo<std::chrono::hours,   half_days,    std::uint8_t>; //12
using minute_number = stdex::chrono::modulo<std::chrono::minutes, std::chrono::hours,   std::uint8_t>; //60
using second_number = stdex::chrono::modulo<std::chrono::seconds, std::chrono::minutes, std::uint8_t>; //60

// todo: time_of_day must be specialized to provide access to hour_number/minute_number/second_number/subseconds depending on Duration
// So the following can be only a representation, not the interface
template <class Duration>
using time_of_day = stdex::chrono::modulo<Duration, days, std::uint32_t >;

// fixme: Should the following conversions work only for time_of_day?
template <class ModuloFrom>
constexpr hour_number to_hour(ModuloFrom m) noexcept
{
    return stdex::chrono::modulo_cast<hour_number, days>(m);
}

template <class ModuloFrom>
constexpr am_pm_number to_am_pm(ModuloFrom m) noexcept
{
  return stdex::chrono::modulo_cast<am_pm_number, days>(m);
}
template <class ModuloFrom>
constexpr am_pm_hour to_am_pm_hour(ModuloFrom m) noexcept
{
  return stdex::chrono::modulo_cast<am_pm_hour, days>(m);
}

// This doesn't works if the time_of_day is in hours
// todo: use SFINAE to remove the overload when the precision of ModuloFrom is less fine grained than minutes
template <class ModuloFrom>
constexpr minute_number to_minute(ModuloFrom m) noexcept
{
  return stdex::chrono::modulo_cast<minute_number, days>(m);
}

template <class ModuloFrom>
constexpr second_number to_second(ModuloFrom m) noexcept
{
  return stdex::chrono::modulo_cast<second_number, days>(m);
}

// todo: add to_subsecond

static_assert(am_pm_hour::cardinal  == 12, "12 hours are not a half-day");
static_assert(am_pm_number::cardinal  == 2, "2 half-days are not a day");
static_assert(hour_number::cardinal  == 24, "24 hours are not a day");
static_assert(minute_number::cardinal  == 60, "60 minutes are not an hour_number");
static_assert(second_number::cardinal  == 60, "60 seconds are not a minute_number");
static_assert(hour_number::min()  == hour_number{0}, "0 is not the min of hour_number");
static_assert(hour_number::max()  == hour_number{23}, "23 is not the min of hour_number");

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
  {
                                            //   ms        sec       min  h
      constexpr time_of_day<std::chrono::milliseconds> t2{ 234+ 1000*(2 + 60 * ( 4 + 8 * 60 ) )};
      std::cout << "t2 = " << t2.count() << "\n";
      time_of_day<std::chrono::milliseconds> t{ std::chrono::milliseconds(234)+ std::chrono::seconds(2) + std::chrono::minutes(4) + std::chrono::hours(20)};
      std::cout << "t = " << t.count() << "\n";

      std::cout << "am_pm_number = " << int(to_am_pm(t).count()) << "\n";
      std::cout << "am_pm_hour = " << int(to_am_pm_hour(t).count()) << "\n";
      std::cout << "hour_number = " << int(to_hour(t).count()) << "\n";
      std::cout << "minute_number = " << int(to_minute(t).count()) << "\n";
      std::cout << "second_number = " << int(to_second(t).count()) << "\n";

      BOOST_TEST(to_am_pm(t).count() == 1);
      BOOST_TEST(to_am_pm_hour(t).count() == 8);
      BOOST_TEST_EQ(to_hour(t).count(), 20);
      BOOST_TEST_EQ(to_minute(t).count(), 4);
      BOOST_TEST_EQ(to_second(t).count(), 2);
  }
  {
      constexpr weekday_number wd{2};
      BOOST_TEST(wd.count() == 2);
  }
  {
    stdex::ordinal_array<int, weekday_number> arr;
    BOOST_TEST(arr.size() == 7);
  }
  {
    constexpr stdex::ordinal_array<int, weekday_number> c {{0, 1, 2, 3, 4, 5, 6}};
    BOOST_TEST(c.size() == 7);
    BOOST_TEST(c[weekday_number{1}] == 1);
    BOOST_TEST(c[weekday_number{2}] == 2);
    BOOST_TEST(c[weekday_number{3}] == 3);
  }
  {
    // fixme:  the type ‘const std::experimental::fundamental_v3::ordinal_range<std::experimental::fundamental_v3::chrono::modulo<std::chrono::duration<int, std::ratio<86400l, 1l> >, std::chrono::duration<int, std::ratio<604800l, 1l> >, unsigned char> >’ of constexpr variable ‘rng’ is not literal
    // Surely it comes from boost::iterator
    //constexpr
    stdex::ordinal_range<weekday_number> rng;
    auto b = rng.begin();
    BOOST_TEST(b->count()==0);
    for (auto w : rng)
    {
      std::cout << w << "\n";
    }
  }
  {
      stdex::ordinal_set<weekday_number> os;
      os[weekday_number{1}]=true;
      BOOST_TEST( 1 == os.count() );
      BOOST_TEST( os.any() );
      BOOST_TEST(os[weekday_number{1}]);
  }
  return ::boost::report_errors();
}
