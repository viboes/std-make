// Copyright (C) 2019 Vicente J. Botet Escriba
//
//  Distributed under the Boost Software License, Version 1.0. (See accompanying
//  file LICENSE_1_0.txt or copy at http://www.boost.org/LICENSE_1_0.txt)

// <experimental/status_value.hpp>

#include <experimental/flat_map.hpp>
#include <limits>

#include <boost/detail/lightweight_test.hpp>

namespace stdex = std::experimental;

int main()
{
	{
		stdex::flat_map<int, int> fm;
		BOOST_TEST(fm.empty());
		BOOST_TEST_EQ(fm.size(), 0);
		//BOOST_TEST_EQ(fm.max_size(), std::numeric_limits<unsigned long int>::max());
		auto bit = fm.begin();
		auto eit = fm.end();
		BOOST_TEST(bit == eit);
	}
	{
		std::vector<int>          keys   = {1, 2, 3};
		std::vector<int>          values = {1, 2, 3};
		stdex::flat_map<int, int> fm(stdex::sorted_unique_t{}, std::move(keys), std::move(values));
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		std::vector<int> keys2   = {1, 2, 3};
		std::vector<int> values2 = {1, 2, 3};
		BOOST_TEST(fm.keys() == keys2);
		BOOST_TEST(fm.values() == values2);
	}
	{
		std::vector<int>          keys   = {1, 2, 3};
		std::vector<int>          values = {2, 4, 6};
		stdex::flat_map<int, int> fm(stdex::sorted_unique_t{}, std::move(keys), std::move(values));
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		auto it  = fm.begin();
		auto it2 = ++it;
		BOOST_TEST((*it2).first == 2);
		BOOST_TEST((*it2).second == 4);
	}
	{
		std::vector<int>          keys   = {1, 3, 2};
		std::vector<int>          values = {1, 2, 3};
		stdex::flat_map<int, int> fm(std::move(keys), std::move(values));
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		auto it  = fm.begin();
		auto it2 = ++it;
		BOOST_TEST(it2->first == 2);
		BOOST_TEST(it2->second == 3);

		auto cit = fm.cbegin();
		BOOST_TEST(cit->first == 1);
		BOOST_TEST(cit->second == 1);

		//it2->first  = 2; // compile fails as expected
		it2->second = 2;
	}
	{
		std::vector<int>          keys   = {1, 3, 2};
		std::vector<int>          values = {1, 2, 3};
		stdex::flat_map<int, int> fm(std::move(keys), std::move(values));
		{
			stdex::flat_map<int, int> const &cfm = fm;
			auto                             cit = cfm.begin();
			BOOST_TEST(cit->first == 1);
			BOOST_TEST(cit->second == 1);
		}
	}
	{
		std::vector<int>          keys   = {1, 3, 2};
		std::vector<int>          values = {1, 2, 3};
		stdex::flat_map<int, int> fm(std::move(keys), std::move(values));
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		std::vector<int> keys2   = {1, 2, 3};
		std::vector<int> values2 = {1, 3, 2};
		BOOST_TEST(fm.keys() == keys2);
		BOOST_TEST(fm.values() == values2);
	}
	{
		std::vector<std::pair<int, int>> cont = {std::make_pair(1, 2), std::make_pair(2, 4), std::make_pair(3, 6)};
		stdex::flat_map<int, int>        fm(stdex::sorted_unique_t{}, std::move(cont));
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		std::vector<int> keys2   = {1, 2, 3};
		std::vector<int> values2 = {2, 4, 6};
		BOOST_TEST(fm.keys() == keys2);
		BOOST_TEST(fm.values() == values2);
	}
	{
		stdex::flat_map<int, int> fm = {stdex::sorted_unique_t{}, {std::make_pair(1, 2), std::make_pair(2, 4), std::make_pair(3, 6)}};
		BOOST_TEST(!fm.empty());
		BOOST_TEST_EQ(fm.size(), 3);
		std::vector<int> keys2   = {1, 2, 3};
		std::vector<int> values2 = {2, 4, 6};
		BOOST_TEST(fm.keys() == keys2);
		BOOST_TEST(fm.values() == values2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(2, short(4)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               bit = fm.begin();
		auto                               eit = fm.end();
		BOOST_TEST(bit != eit);
		auto f = *bit;
		BOOST_TEST_EQ(f.first, 1);
		BOOST_TEST_EQ(f.second, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(2, short(4)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               r = fm[1];
		BOOST_TEST_EQ(r, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(2, short(4)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               bit = fm.begin();
	 	auto 							   r1   = bit[1];
	 	BOOST_TEST_EQ(r1.first, 2);
	 	BOOST_TEST_EQ(r1.second, 4);
	 	auto 							   r2   = *(bit+2);
	 	BOOST_TEST_EQ(r2.first, 3);
	 	BOOST_TEST_EQ(r2.second, 6);
		bit += 1;
	 	BOOST_TEST_EQ((*bit).first, 2);
	 	BOOST_TEST_EQ(bit->second, 4);
		bit -= 1;
	 	BOOST_TEST_EQ((*bit).first, 1);
	 	BOOST_TEST_EQ(bit->second, 2);
		bit   = bit + 2 ;
		bit   = bit - 1 ;
	 	BOOST_TEST_EQ((*bit).first, 2);
	 	BOOST_TEST_EQ(bit->second, 4); 
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i = 1;
		auto                               r = fm[i];
		BOOST_TEST_EQ(r, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               r = fm[5] = 42;
		BOOST_TEST_EQ(r, 42);
		BOOST_TEST(fm.contains(1));
		BOOST_TEST(fm.contains(5));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		short                              i = 1;
		auto                               r = fm[i];
		BOOST_TEST_EQ(r, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               r = fm.at(1);
		BOOST_TEST_EQ(r, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		try
		{
			auto r = fm.at(2);
			(void)r;
			BOOST_TEST(false);
		}
		catch (std::out_of_range &ex)
		{
			BOOST_TEST(std::string("flat_map bad key") == ex.what());
		}
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 1;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit != fm.end());
		auto f = *bit;
		BOOST_TEST_EQ(f.first, 1);
		BOOST_TEST_EQ(f.second, 2);
	}
	{
		std::vector<std::pair<int, short>> cont;
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 1;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 1;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit != fm.end());
		BOOST_TEST(bit == fm.find(1));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 2;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 0;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit == fm.find(1));
	}

	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = -1;
		auto                               bit = fm.upper_bound(i);
		BOOST_TEST(bit != fm.end());
		BOOST_TEST(bit == fm.find(1));
	}
	{
		std::vector<std::pair<int, short>> cont;
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 1;
		auto                               bit = fm.upper_bound(i);
		BOOST_TEST(bit == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               bit = fm.upper_bound(5);
		BOOST_TEST(bit == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               bit = fm.upper_bound(2);
		BOOST_TEST(bit == fm.find(3));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 0;
		auto                               bit = fm.lower_bound(i);
		BOOST_TEST(bit == fm.find(1));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               rng = fm.equal_range(1);
		BOOST_TEST(rng.first == fm.find(1));
		BOOST_TEST(rng.second == fm.find(3));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               rng = fm.equal_range(-1);
		BOOST_TEST(rng.first == fm.find(1));
		BOOST_TEST(rng.second == fm.find(1));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               rng = fm.equal_range(3);
		BOOST_TEST(rng.first == fm.find(3));
		BOOST_TEST(rng.second == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(3, short(6))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		auto                               rng = fm.equal_range(5);
		BOOST_TEST(rng.first == fm.end());
		BOOST_TEST(rng.second == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		int                                i   = 1;
		auto                               bit = fm.find(i);
		BOOST_TEST(bit != fm.end());
		auto f = *bit;
		BOOST_TEST_EQ(f.first, 1);
		BOOST_TEST_EQ(f.second, 2);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		BOOST_TEST(fm.find(2) == fm.end());
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		BOOST_TEST(fm.contains(1));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		//BOOST_TEST( fm.end() ==fm.find(2) );
		BOOST_TEST(!fm.contains(2));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		BOOST_TEST(1 == fm.count(1));
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2))};
		stdex::flat_map<int, short>        fm(stdex::sorted_unique_t{}, std::move(cont));
		BOOST_TEST_EQ(0, fm.count(2));
	}
	{
		stdex::flat_map<int, int> fm;
		fm.emplace(1, 2);
		BOOST_TEST_EQ(1, fm.size());
		BOOST_TEST(1 == (*fm.find(1)).first);
		BOOST_TEST(2 == (*fm.find(1)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		auto                        it = static_cast<stdex::flat_map<int, short> const &>(fm).find(1);
		fm.emplace_hint(it, 3, 4);
		BOOST_TEST_EQ(2, fm.size());
		BOOST_TEST_EQ(3, (*fm.find(3)).first);
		BOOST_TEST_EQ(4, (*fm.find(3)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		auto                        it = static_cast<stdex::flat_map<int, short> const &>(fm).find(1);
		fm.emplace_hint(it, 1, 3);
		BOOST_TEST_EQ(1, fm.size());
		BOOST_TEST_EQ(1, (*fm.find(1)).first);
		BOOST_TEST_EQ(2, (*fm.find(1)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		fm.emplace(1, 3);
		BOOST_TEST_EQ(1, fm.size());
		BOOST_TEST_EQ(1, (*fm.find(1)).first);
		BOOST_TEST_EQ(2, (*fm.find(1)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		fm.insert(std::make_pair(1, short(2)));
		BOOST_TEST_EQ(1, fm.size());
		BOOST_TEST_EQ(1, (*fm.find(1)).first);
		BOOST_TEST_EQ(2, (*fm.find(1)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		fm.insert(std::make_pair(-3, short(4)));
		BOOST_TEST_EQ(2, fm.size());
		BOOST_TEST_EQ(-3, (*fm.find(-3)).first);
		BOOST_TEST_EQ(4, (*fm.find(-3)).second);
	}
	{
		stdex::flat_map<int, short> fm(stdex::sorted_unique_t{}, {std::make_pair(1, short(2))});
		auto                        it = static_cast<stdex::flat_map<int, short> const &>(fm).find(1);
		fm.insert(it, std::make_pair(3, short(4)));
		BOOST_TEST_EQ(2, fm.size());
		BOOST_TEST_EQ(3, (*fm.find(3)).first);
		BOOST_TEST_EQ(4, (*fm.find(3)).second);
	}
	{
		std::vector<std::pair<int, short>> cont = {std::make_pair(1, short(2)), std::make_pair(2, short(3))};
		stdex::flat_map<int, short>        fm;
		fm.insert(cont.begin(), cont.end());
		BOOST_TEST_EQ(2, fm.size());
	}

	{
		const int                     i = 0;
		int                           j = 0;
		std::pair<int const &, int &> r(i, j);
		BOOST_TEST_EQ(&i, &r.first);
		BOOST_TEST_EQ(&j, &r.second);
		r.second = 2;
		//r.first = 2;
		BOOST_TEST_EQ(&j, &r.second);
		BOOST_TEST(j == 2);
		j = 3;
		BOOST_TEST(r.second == 3);
		BOOST_TEST(r.first == 0);
		int k    = 9;
		r.second = k;
		BOOST_TEST_EQ(&i, &r.first);
		BOOST_TEST_EQ(&j, &r.second);
		BOOST_TEST(r.second == 9);
		k = 8;
		BOOST_TEST_EQ(r.second, 9);
		r.second = 1;
		BOOST_TEST_NE(&k, &r.second);
		BOOST_TEST_EQ(k, 8);
		//r = std::make_pair(i, 22);
	}

	return ::boost::report_errors();
}
