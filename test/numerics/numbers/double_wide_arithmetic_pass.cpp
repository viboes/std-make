// Copyright (C) 2017 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)
// Based on https://github.com/akrzemi1/explicit/blob/master/test/test_explicit.cpp

// <experimental/numerics/v1/numbers/double_wide_arithmetic.hpp>
#include <boost/detail/lightweight_test.hpp>
#include <experimental/numerics/v1/numbers/double_wide_arithmetic.hpp>
#include <string>

namespace stdex = std::experimental;
namespace nmx = std::experimental::numerics;

static_assert(std::is_same<decltype(nmx::wide_neg(int32_t(1))), int64_t>::value, "error");
static_assert(std::is_same<decltype(nmx::wide_neg(uint32_t(1))), uint64_t>::value, "error");

static_assert(std::is_same<decltype(nmx::wide_add(int32_t(1), int32_t(1))), int64_t>::value, "error");
static_assert(std::is_same<decltype(nmx::wide_add(uint32_t(1), uint32_t(1))), uint64_t>::value, "error");

static_assert(std::is_same<decltype(nmx::wide_sub(int32_t(1), int32_t(1))), int64_t>::value, "error");
static_assert(std::is_same<decltype(nmx::wide_sub(uint32_t(1), uint32_t(1))), uint64_t>::value, "error");

int main()
{

  BOOST_TEST_EQ(nmx::wide_neg(1), -1);
  BOOST_TEST_EQ(nmx::wide_neg(1u), uint64_t(-1));

  BOOST_TEST_EQ(nmx::wide_add(1, 2), int64_t(3));
  BOOST_TEST_EQ(nmx::wide_add(uint32_t(1), uint32_t(2)), uint64_t(3));
  BOOST_TEST_EQ(nmx::wide_add(int8_t(127), int8_t(127)), int16_t(254));
  BOOST_TEST_EQ(nmx::wide_add(int8_t(-128), int8_t(-128)), int16_t(-256));


  BOOST_TEST_EQ(nmx::wide_sub(1, 2), int64_t(-1));
  BOOST_TEST_EQ(nmx::wide_sub(int8_t(127), int8_t(-128)), int16_t(255));
  BOOST_TEST_EQ(nmx::wide_sub(int8_t(-128), int8_t(127)), int16_t(-255));

  BOOST_TEST_EQ(nmx::wide_sub(uint8_t(132), uint8_t(100)), uint16_t(32));
  BOOST_TEST_EQ(nmx::wide_sub(uint8_t(100), uint8_t(101)), uint16_t(-1));

  BOOST_TEST_EQ(nmx::wide_add2(int8_t(100), int8_t(100), int8_t(100)), int16_t(300));

  BOOST_TEST_EQ(nmx::wide_sub2(int8_t(-100), int8_t(100), int8_t(100)), int16_t(-300));

  BOOST_TEST_EQ(nmx::wide_lsh(uint8_t(0x12), 4), uint16_t(0x120));
  BOOST_TEST_EQ(nmx::wide_lsh(int16_t(0x1234), 4), int32_t(0x12340));

  BOOST_TEST_EQ(nmx::wide_lshadd(int16_t(0x1234), 16, int16_t(0x1234)), int32_t(0x12341234));

  BOOST_TEST_EQ(nmx::wide_mul(int8_t(-2), int8_t(100)), int16_t(-200));
  BOOST_TEST_EQ(nmx::wide_mul(uint8_t(2), uint8_t(100)), uint16_t(200));

  BOOST_TEST_EQ(nmx::wide_muladd(int8_t(-2), int8_t(100), int8_t(100)), int16_t(-100));
  BOOST_TEST_EQ(nmx::wide_muladd(int8_t(2), int8_t(100), int8_t(100)), int16_t(300));

  BOOST_TEST_EQ(nmx::wide_muladd(uint8_t(2), uint8_t(100), uint8_t(100)), uint16_t(300));

  BOOST_TEST_EQ(nmx::wide_muladd2(int8_t(2), int8_t(100), int8_t(100), int8_t(100)), int16_t(400));
  BOOST_TEST_EQ(nmx::wide_mulsub2(int8_t(2), int8_t(100), int8_t(100), int8_t(100)), int16_t(0));

  BOOST_TEST_EQ(nmx::wide_divn(int16_t(0x1024), int8_t(2)), int8_t(0x12));
  BOOST_TEST_EQ(nmx::wide_divw(int16_t(200), int8_t(5)), int16_t(40));

  {
    uint8_t l;
    static_assert(std::is_same<uint8_t, std::make_unsigned_t<int8_t>>::value, "UT must be make_unsigned_t<T>");

    uint8_t h = nmx::split_neg(&l, int8_t(1));
    BOOST_TEST_EQ(l, uint8_t(0xff));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, int8_t(1));
    BOOST_TEST_EQ(l, uint8_t(0xff));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, int8_t(127));
    BOOST_TEST_EQ(l, uint8_t(0x81));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, int8_t(-128));
    BOOST_TEST_EQ(l, uint8_t(0x80));
    BOOST_TEST_EQ(h, uint8_t(0x00));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, uint8_t(-1));
    BOOST_TEST_EQ(l, uint8_t(0x01));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    std::cout << std::hex << short(-uint8_t(-1)) << std::endl;
    uint64_t l;
    uint64_t h = nmx::split_neg(&l, uint64_t(-1));
    BOOST_TEST_EQ(l, uint64_t(0x01));
    BOOST_TEST_EQ(h, uint64_t(-1));
  }

  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, int8_t(-1));
    BOOST_TEST_EQ(l, uint8_t(0x01));
    BOOST_TEST_EQ(h, uint8_t(0x00));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_neg(&l, uint8_t(2));
    BOOST_TEST_EQ(l, uint8_t(0xfe));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_add(&l, uint8_t(1), uint8_t(2));
    BOOST_TEST_EQ(l, uint8_t(0x03));
    BOOST_TEST_EQ(h, uint8_t(0x00));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_add(&l, uint8_t(0xff), uint8_t(0x01));
    BOOST_TEST_EQ(l, uint8_t(0x00));
    BOOST_TEST_EQ(h, uint8_t(0x01));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_add(&l, uint8_t(0x01), uint8_t(0x01));
    BOOST_TEST_EQ(l, uint8_t(0x02));
    BOOST_TEST_EQ(h, uint8_t(0x00));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_add(&l, uint8_t(0xff), uint8_t(0x02));
    BOOST_TEST_EQ(l, uint8_t(0x01));
    BOOST_TEST_EQ(h, uint8_t(0x01));
  }
  {
    uint8_t l;
    uint8_t h = nmx::split_sub(&l, uint8_t(0x00), uint8_t(0x01));
    BOOST_TEST_EQ(l, uint8_t(0xff));
    BOOST_TEST_EQ(h, uint8_t(0xff));
  }
  {
    uint32_t l;
    uint32_t h = nmx::split_add(&l, uint32_t(-1), uint32_t(0x01));
    BOOST_TEST_EQ(l, uint32_t(0x00));
    BOOST_TEST_EQ(h, uint32_t(0x01));
  }
  {
    uint32_t l;
    uint32_t h = nmx::split_add(&l, int32_t(-1), int32_t(0x01));
    BOOST_TEST_EQ(l, uint32_t(0x00));
    BOOST_TEST_EQ(h, uint32_t(0x00));
  }
  {
    uint64_t l;
    uint64_t h = nmx::split_add(&l, uint64_t(1), uint64_t(0x01));
    BOOST_TEST_EQ(l, uint64_t(0x02));
    BOOST_TEST_EQ(h, uint64_t(0x00));
  }
  {
    uint64_t l;
    uint64_t h = nmx::split_add(&l, uint64_t(-1), uint64_t(0x01));
    BOOST_TEST_EQ(l, uint64_t(0x00));
    BOOST_TEST_EQ(h, uint64_t(0x01));
  }
  {
    uint64_t l;
    uint64_t h = nmx::split_add(&l, int64_t(-1), int64_t(0x01));
    BOOST_TEST_EQ(l, uint64_t(0x00));
    BOOST_TEST_EQ(h, uint64_t(0x00));
  }

  return ::boost::report_errors();
}

