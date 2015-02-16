#ifndef CharacterGenerator_HeaderPlusPlus
#define CharacterGenerator_HeaderPlusPlus

#include "Generator.hpp"

#include <deque>
#include <iterator>
#include <map>
#include <random>
#include <set>
#include <string>

struct CharacterGenerator
: virtual Generator
{
	CharacterGenerator(std::size_t order)
	: order{order}
	{
	}

private:
	std::size_t order;
	std::map<std::set<std::string::value_type>, std::map<std::string::value_type, std::size_t>> data;
	std::deque<std::string::value_type> recent;
	std::mt19937 rand {std::random_device{}()};

	virtual std::istream &input(std::istream &s)
	{
		std::string::value_type c;
		while(s.read(&c, 1))
		{
			recent.push_back(c);
			if(recent.size() > order)
			{
				for(auto it = recent.begin(), jt = recent.end()-1; it != jt; ++it)
				{
					std::set<std::string::value_type> past {it, jt};
					if(past.size() > 0)
					{
						++data[past][c];
					}
				}
				recent.pop_front();
			}
		}
		return s;
	}
	virtual std::ostream &output(std::ostream &s)
	{
		while(data.size() > 0)
		{
			auto basis = recent;
			while(basis.size() > 0)
			{
				std::set<std::string::value_type> past {basis.begin(), basis.end()};
				auto future = data.find(past);
				if(future == data.end())
				{
					basis.pop_front();
					continue;
				}
				auto charmap = future->second;
				std::size_t max {};
				for(auto const &chance : charmap)
				{
					max += chance.second;
				}
				std::uniform_int_distribution<std::size_t> dist {1, max};
				auto r = dist(rand);
				for(auto const &chance : charmap)
				{
					if(chance.second >= r)
					{
						recent.pop_front();
						recent.push_back(chance.first);
						return s << chance.first;
					}
					else
					{
						r -= chance.second;
					}
				}
			}
			std::uniform_int_distribution<std::size_t> dist {0, data.size()-1};
			auto it = data.begin();
			std::advance(it, dist(rand));
			recent = {it->first.begin(), it->first.end()};
		}
		return s;
	}
};

#endif
