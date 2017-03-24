// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/chrono.hpp>
#if __cplusplus >= 201402L

#include <experimental/chrono.hpp>

namespace stdex = std::experimental;

// Basic frame durations
using days     = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<24>, std::chrono::hours::period>>;
using weeks    = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<7>, days::period>>;
using years    = std::chrono::duration<int32_t,  std::ratio_multiply<std::ratio<146097, 400>, days::period>>;
using months   = std::chrono::duration<int32_t,  std::ratio_divide<years::period, std::ratio<12>>>;


// relative numbers
using weekday = stdex::chrono::modulo<days,   weeks,    std::uint8_t>; //7
using hour   = stdex::chrono::modulo<std::chrono::hours,   days,    std::uint8_t>; //24
using minute = stdex::chrono::modulo<std::chrono::minutes, std::chrono::hours,   std::uint8_t>; //60
using second = stdex::chrono::modulo<std::chrono::seconds, std::chrono::minutes, std::uint8_t>; //60

template <class Duration>
using time_of_day = stdex::chrono::modulo<Duration, days, std::int32_t >;

template <class ModuloFrom>
hour to_hour(ModuloFrom m)
{
    return stdex::chrono::to_modulo<hour>(m);
}

template <class ModuloFrom>
minute to_minute(ModuloFrom m)
{
    return stdex::chrono::to_modulo<minute>(m);
}

template <class ModuloFrom>
second to_second(ModuloFrom m)
{
    return stdex::chrono::to_modulo<second>(m);
}

static_assert(hour::period  == 24, "24 hours are not a day");
static_assert(minute::period  == 60, "60 minutes are not an hour");
static_assert(second::period  == 60, "60 seconds are not a minute");
static_assert(hour::min()  == hour{0}, "0 is not the min of hour");
static_assert(hour::max()  == hour{23}, "23 is not the min of hour");

#include <boost/detail/lightweight_test.hpp>
#include <iostream>

int main()
{
  {
      time_of_day<std::chrono::milliseconds> t{ 234+ 1000*(2 + 60 * ( 4 + 60 * 8) )};
      std::cout << "hour = " << int(to_hour(t).count()) << "\n";
      std::cout << "minute = " << int(to_minute(t).count()) << "\n";
      std::cout << "second = " << int(to_second(t).count()) << "\n";
  }
  {
      weekday wd{2};
  }
  return 1+::boost::report_errors();
}

#else
int main() { return 0; }
#endif
