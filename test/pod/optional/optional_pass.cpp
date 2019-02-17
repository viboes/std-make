// Copyright (C) 2017-2018 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/strong_id.hpp>

#include <experimental/pod/optional.hpp>
#include <memory>
#include <sstream>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

template <class T>
struct show;

static_assert(std::is_pod<stdex::pod::optional<int>>::value, "pod::optional<int> is not a POD");
static_assert(sizeof(stdex::pod::optional<char>) == 2, "sizeof(stdex::pod::optional<char>)  must be 2");
static_assert(sizeof(stdex::pod::optional<char>) == 2, "sizeof(stdex::pod::optional<char>)  must be 2");
static_assert(sizeof(stdex::pod::optional<int>) == 8, "sizeof(stdex::pod::optional<int>)  must be 8");

struct P
{
	short x;
	short y;
};

static_assert(std::is_pod<stdex::pod::optional<P>>::value, "pod::optional<P> is not a POD");
static_assert(std::is_pod<stdex::pod::optional<P>>::value, "pod::optional<P> is not a POD");
static_assert(sizeof(stdex::pod::optional<P>) == 6, "sizeof(stdex::pod::optional<P>)  must be 6");

#include <boost/endian/arithmetic.hpp>
#include <boost/endian/conversion.hpp>

using big_int16_t     = boost::endian::big_int16_t;
using big_int16_buf_t = boost::endian::big_int16_buf_t;
using big_int32_t     = boost::endian::big_int32_t;

static_assert(std::is_pod<stdex::pod::optional<big_int16_t>>::value, "pod::optional<big_int16_t> is not a POD");
static_assert(std::is_pod<stdex::pod::optional<big_int16_buf_t>>::value, "pod::optional<big_int16_buf_t> is not a POD");
static_assert(sizeof(stdex::pod::optional<big_int16_t>) == 3, "sizeof(stdex::pod::optional<char>)  must be 3");

struct BigP
{
	big_int16_t x;
	big_int16_t y;

	BigP() = default;
	BigP(P const &p)
	{
		x = p.x;
		y = p.y;
	}
	P value() const
	{
		return {x.value(), y.value()};
	}
	operator P() const { return value(); }
};
static_assert(std::is_pod<stdex::pod::optional<BigP>>::value, "pod::optional<BigP> is not a POD");

struct BigBufP
{
	big_int16_buf_t x;
	big_int16_buf_t y;

	BigBufP() = default;
	BigBufP(P const &p)
	{
		x = p.x;
		y = p.y;
	}

	P value() const
	{
		//        P res;
		//        res.x = x.value();
		//        res.y = y.value();
		//        return res;
		return {x.value(), y.value()};
	}
};
static_assert(std::is_pod<stdex::pod::optional<BigBufP>>::value, "pod::optional<BigBufP> is not a POD");

namespace std
{
namespace experimental
{
inline namespace fundamental_v3
{
namespace pod
{
stdex::pod::optional<int16_t> to_pod_optional(stdex::pod::optional<big_int16_buf_t> const &x)
{
	if (x)
		return x->value();
	return nullopt;
}
//#if JASEL_HAS_OPTIONAL == 1
stdex::optional<int16_t> to_std_optional(stdex::pod::optional<big_int16_buf_t> const &x)
{
	if (x)
		return x->value();
	return stdex::nullopt;
}
template <>
class optional_buf<big_int16_buf_t> {
public:
	optional_buf()     = default;
	using logical_type = std::experimental::optional<int16_t>;
	explicit optional_buf(logical_type const &x)
	{
		data.m_present = bool(x);
		if (bool(x))
		{
			data.m_value = *x;
		}
	}

	logical_type value() const
	{
		if (data.m_present)
		{
			return data.m_value.value();
		}
		return nullopt;
	}

private:
	optional_data<big_int16_buf_t> data;
};

template <>
class optional_buf<BigBufP> {
public:
	optional_buf()     = default;
	using logical_type = std::experimental::optional<P>;
	explicit optional_buf(logical_type const &op)
	{
		data.m_present = bool(op);
		if (bool(op))
		{
			data.m_value = *op;
		}
	}

	logical_type value() const
	{
		if (data.m_present)
		{
			return data.m_value.value();
		}
		return nullopt;
	}

private:
	optional_data<BigBufP> data;
};
//#endif
} // namespace pod
} // namespace fundamental_v3
} // namespace experimental
} // namespace std

int main()
{
	{
		stdex::pod::optional<int> o1 = {};
		BOOST_TEST(!o1);
	}
	{
		stdex::pod::optional<big_int16_t> o1 = {};
		BOOST_TEST(!o1);
	}
	{
		stdex::pod::optional<big_int16_buf_t> o1 = {};
		BOOST_TEST(!o1);
	}
	{
		stdex::pod::optional<P> o1 = {};
		BOOST_TEST(!o1);
	}
	{
		stdex::pod::optional<BigP> o1{};
		BOOST_TEST(!o1);
	}
	{
		stdex::pod::optional<BigBufP> o1{};
		BOOST_TEST(!o1);
	}
	{
		unsigned char arr[] = {1, 2, 3, 4};
		static_assert(sizeof(arr) == sizeof(stdex::pod::optional<short>), "");
		stdex::pod::optional<short> *po1 = new (&arr) stdex::pod::optional<short>;
		BOOST_TEST(arr[0] == 1);
		*po1 = std::experimental::nullopt;
		BOOST_TEST(!*po1);
		BOOST_TEST(arr[0] == 0);
	}
	{
		unsigned char                arr[] = {1, 2, 3, 4};
		stdex::pod::optional<short> *po1   = new (&arr) stdex::pod::optional<short>{};
		BOOST_TEST(arr[0] == 0);
		BOOST_TEST(!*po1);
	}

	{
		stdex::pod::optional<int> o1;
		o1 = stdex::nullopt;

		stdex::pod::optional<int> o2 = stdex::nullopt;
		BOOST_TEST(!o2);

		stdex::pod::optional<int> o3;
		o3 = o2;
		BOOST_TEST(!o3);

		BOOST_TEST(o1 == stdex::pod::nullopt);
		BOOST_TEST(o1 == stdex::pod::optional<int>{});
		BOOST_TEST(!o1);
		BOOST_TEST(bool(o1) == false);

		BOOST_TEST(o2 == stdex::pod::nullopt);
		BOOST_TEST(o2 == stdex::pod::optional<int>{});
		BOOST_TEST(!o2);
		BOOST_TEST(bool(o2) == false);

		BOOST_TEST(o3 == stdex::pod::nullopt);
		BOOST_TEST(o3 == stdex::pod::optional<int>{});
		BOOST_TEST(!o3);
		BOOST_TEST(bool(o3) == false);

		BOOST_TEST(o1 == o2);
		BOOST_TEST(o2 == o1);
		BOOST_TEST(o1 == o3);
		BOOST_TEST(o3 == o1);
		BOOST_TEST(o2 == o3);
		BOOST_TEST(o3 == o2);
	}
	{
		stdex::pod::optional<big_int16_t> o1;
		o1 = stdex::nullopt;

		stdex::pod::optional<big_int16_t> o2 = stdex::nullopt;
		BOOST_TEST(!o2);

		stdex::pod::optional<big_int16_t> o3;
		o3 = o2;
		BOOST_TEST(!o3);

		BOOST_TEST(o1 == stdex::pod::nullopt);
		BOOST_TEST(o1 == stdex::pod::optional<big_int16_t>{});
		BOOST_TEST(!o1);
		BOOST_TEST(bool(o1) == false);

		BOOST_TEST(o2 == stdex::pod::nullopt);
		BOOST_TEST(o2 == stdex::pod::optional<big_int16_t>{});
		BOOST_TEST(!o2);
		BOOST_TEST(bool(o2) == false);

		BOOST_TEST(o3 == stdex::pod::nullopt);
		BOOST_TEST(o3 == stdex::pod::optional<big_int16_t>{});
		BOOST_TEST(!o3);
		BOOST_TEST(bool(o3) == false);

		BOOST_TEST(o1 == o2);
		BOOST_TEST(o2 == o1);
		BOOST_TEST(o1 == o3);
		BOOST_TEST(o3 == o1);
		BOOST_TEST(o2 == o3);
		BOOST_TEST(o3 == o2);
	}
	{
		// fixme: some operations on pod::optional<endian_buffer> don't work as endian_buffer is not regular
		// we need to extract a pod::optional<native>.
		// We can do it with non-member function and ADL or a trait
		// We could as well define a pod::optional_transform<endian_buffer> that returns the nested value_type of its parameter
		// However this pod::optional_transform couldn't return references.
		stdex::pod::optional<big_int16_buf_t> o1;
		o1 = stdex::nullopt;

		stdex::pod::optional<big_int16_buf_t> o2 = stdex::nullopt;
		BOOST_TEST(!o2);

		stdex::pod::optional<big_int16_buf_t> o3;
		o3 = o2;
		BOOST_TEST(!o3);

		BOOST_TEST(o1 == stdex::pod::nullopt);
		//BOOST_TEST (o1 == stdex::pod::optional<big_int16_buf_t>{});
		BOOST_TEST(!o1);
		BOOST_TEST(bool(o1) == false);

		BOOST_TEST(o2 == stdex::pod::nullopt);
		//BOOST_TEST (o2 == stdex::pod::optional<big_int16_buf_t>{});
		BOOST_TEST(!o2);
		BOOST_TEST(bool(o2) == false);

		BOOST_TEST(o3 == stdex::pod::nullopt);
		//BOOST_TEST (o3 == stdex::pod::optional<big_int16_buf_t>{});
		BOOST_TEST(!o3);
		BOOST_TEST(bool(o3) == false);

		//BOOST_TEST (o1 == o2);
		//BOOST_TEST (o2 == o1);
		//BOOST_TEST (o1 == o3);
		//BOOST_TEST (o3 == o1);
		//BOOST_TEST (o2 == o3);
		//BOOST_TEST (o3 == o2);
	}

	{
		stdex::pod::optional<int> oi;
		oi = stdex::pod::optional<int>{1};
		BOOST_TEST(*oi == 1);

		oi = stdex::pod::nullopt;
		BOOST_TEST(!oi);

		oi = 2;
		BOOST_TEST(oi);
		BOOST_TEST(*oi == 2);

		oi.reset();
		BOOST_TEST(!oi);
		oi = 2;
		BOOST_TEST(oi);
		oi = {}; // fixme: fails !oi - function is called?
		BOOST_TEST(!bool(oi));
		BOOST_TEST(!oi);
	}
#if JASEL_HAS__STD_OPTIONAL == 1
	{
		std::optional<big_int16_t> oi;
		oi = std::optional<big_int16_t>{1};
		BOOST_TEST(*oi == 1);

		oi = std::nullopt;
		BOOST_TEST(!oi);

		oi = 2;
		BOOST_TEST(*oi == 2);
		oi = {};
		BOOST_TEST(!oi);
		oi = 2;
		BOOST_TEST(oi);
		oi.reset();
		BOOST_TEST(!oi);
	}
#endif
	{
		stdex::optional<int> oi;
		oi = stdex::optional<int>{1};
		BOOST_TEST(*oi == 1);

		oi = stdex::nullopt;
		BOOST_TEST(!oi);

		oi = 2;
		BOOST_TEST(*oi == 2);
#if defined __clang__
		// fixme gcc
		oi = {};
		BOOST_TEST(!oi);
		oi = 2;
		BOOST_TEST(oi);
		oi.reset();
		BOOST_TEST(!oi);
#endif
	}
#if 0
  // fixme clang
  {
      stdex::optional<big_int16_t> oi;
      oi = stdex::optional<big_int16_t>{1};
      BOOST_TEST (*oi == 1);

      oi = stdex::nullopt;
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
		BOOST_TEST(*oi == 1);

		oi = stdex::pod::nullopt;
		BOOST_TEST(!oi);

		oi = 2;
		BOOST_TEST(*oi == 2);
		oi = {};
		BOOST_TEST(!oi);
		oi = 2;
		BOOST_TEST(oi);
		oi.reset();
		BOOST_TEST(!oi);
	}
	{
		stdex::pod::optional<big_int16_buf_t> oi;
		oi = stdex::pod::optional<big_int16_buf_t>{1};
		//BOOST_TEST (*oi == 1);
		BOOST_TEST(oi->value() == 1);

		oi = stdex::pod::nullopt;
		BOOST_TEST(!oi);

		oi = 2;
		//BOOST_TEST (*oi == 2);
		BOOST_TEST(oi->value() == 2);
		oi = {};
		BOOST_TEST(!oi);
		oi = 2;
		BOOST_TEST(oi);
		oi.reset();
		BOOST_TEST(!oi);
	}
	//#if defined JASEL_STD_HAVE_OPTIONAL
	{
		stdex::pod::optional_buf<big_int16_buf_t> obi{stdex::optional<int16_t>{1}};
		stdex::optional<int16_t>                  oi = obi.value();
		BOOST_TEST(oi);
		BOOST_TEST(*oi == 1);
	}
	{
		stdex::pod::optional_buf<BigBufP> ob{stdex::optional<P>{P{1, 2}}};
		stdex::optional<P>                o = ob.value();
		BOOST_TEST(o);
		BOOST_TEST(o->x == 1);
		BOOST_TEST(o->y == 2);
	}
	//#endif
	{
		constexpr stdex::pod::optional<int> oi = {}; // no default constructor
		BOOST_TEST(!oi);
	}
	{
		constexpr stdex::pod::optional<big_int16_t> oi = {}; // no default constructor
		BOOST_TEST(!oi);
	}
	{
		stdex::pod::optional<int> oi = {}; // no default constructor
		stdex::pod::optional<int> oj = oi;

		BOOST_TEST(!oj);
		BOOST_TEST(oj == oi);
		BOOST_TEST(oj == stdex::pod::nullopt);
		BOOST_TEST(!bool(oj));

		oi                           = 1;
		stdex::pod::optional<int> ok = oi;
		BOOST_TEST(!!ok);
		BOOST_TEST(bool(ok));
		BOOST_TEST(ok == oi);
		BOOST_TEST(ok != oj);
		BOOST_TEST(*ok == 1);

		stdex::pod::optional<int> ol = std::move(oi);
		BOOST_TEST(!!ol);
		BOOST_TEST(bool(ol));
		BOOST_TEST(ol == oi);
		BOOST_TEST(ol != oj);
		BOOST_TEST(*ol == 1);
	}
	{
		stdex::pod::optional<int> oi{stdex::pod::in_place};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(*oi == 0);
		BOOST_TEST(bool(oi));
	}
	{
		stdex::pod::optional<big_int16_t> oi{stdex::pod::in_place};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(*oi == 0);
		BOOST_TEST(bool(oi));
	}
	{
		stdex::pod::optional<int> oi{stdex::pod::in_place, 1};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(bool(oi));
	}
	{
		stdex::pod::optional<big_int16_t> oi{stdex::pod::in_place, 1};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(bool(oi));
	}
	{
		stdex::pod::optional<P> oi{stdex::pod::in_place};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(bool(oi));
		BOOST_TEST(oi->x == 0);
		BOOST_TEST(oi->y == 0);
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
		stdex::pod::optional<BigP> oi{stdex::pod::in_place};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(bool(oi));
		BOOST_TEST(oi->x == 0);
		BOOST_TEST(oi->y == 0);
	}
	{
		stdex::pod::optional<BigBufP> oi{stdex::pod::in_place};
		BOOST_TEST(oi != stdex::pod::nullopt);
		BOOST_TEST(bool(oi));
		BOOST_TEST(oi->value().x == 0);
		BOOST_TEST(oi->value().y == 0);
	}
	{
		stdex::pod::optional<stdex::pod::optional<int>> oi1 = stdex::pod::nullopt;
		BOOST_TEST(oi1 == stdex::pod::nullopt);
		BOOST_TEST(!oi1);

		{
			stdex::pod::optional<stdex::pod::optional<int>> oi2{stdex::pod::in_place};
			BOOST_TEST(oi2 != stdex::pod::nullopt);
			BOOST_TEST(bool(oi2));
			BOOST_TEST(*oi2 == stdex::pod::nullopt);
		}
		{
			stdex::pod::optional<stdex::pod::optional<P>> oi2{stdex::pod::in_place};
			BOOST_TEST(oi2 != stdex::pod::nullopt);
			BOOST_TEST(bool(oi2));
			BOOST_TEST(*oi2 == stdex::pod::nullopt);
		}

		{
			stdex::pod::optional<stdex::pod::optional<int>> oi2{stdex::pod::in_place, stdex::pod::nullopt};
			BOOST_TEST(oi2 != stdex::pod::nullopt);
			BOOST_TEST(bool(oi2));
			BOOST_TEST(*oi2 == stdex::pod::nullopt);
			BOOST_TEST(!*oi2);
		}

		{
			stdex::pod::optional<stdex::pod::optional<int>> oi2{stdex::pod::optional<int>{}};
			BOOST_TEST(oi2 != stdex::pod::nullopt);
			BOOST_TEST(bool(oi2));
			BOOST_TEST(*oi2 == stdex::pod::nullopt);
			BOOST_TEST(!*oi2);
		}

		constexpr stdex::pod::optional<int> oi  = {};
		constexpr auto                      ooi = stdex::pod::make_optional(oi);
		static_assert(std::is_same<const stdex::pod::optional<stdex::pod::optional<int>>, decltype(ooi)>::value, "");

		{
		}
	}

	return ::boost::report_errors();
}
