#ifndef WordGenerator_HeaderPlusPlus
#define WordGenerator_HeaderPlusPlus

#include "Generator.hpp"

#include <cctype>
#include <deque>
#include <iterator>
#include <map>
#include <random>
#include <set>
#include <string>

struct WordGenerator
: virtual Generator
{
	WordGenerator(std::size_t order)
	: order{order}
	{
	}

private:
	std::size_t order;
	std::map<std::set<std::string>, std::map<std::string, std::size_t>> data;
	std::deque<std::string> recent;
	std::mt19937 rand {std::random_device{}()};

	void add_word(std::string const &w)
	{
		if(w.empty())
		{
			return;
		}
		recent.push_back(w);
		if(recent.size() > order)
		{
			for(auto it = recent.begin(), jt = recent.end()-1; it != jt; ++it)
			{
				std::set<std::string> past {it, jt};
				if(past.size() > 0)
				{
					++data[past][w];
				}
			}
			recent.pop_front();
		}
	}
	bool is_punct_req_space(std::string::value_type c)
	{
		return c == '.' || c == ',' || c == '!' || c == '?';
	}
	virtual std::istream &input(std::istream &s)
	{
		std::string::value_type c;
		std::string current;
		while(s.read(&c, 1))
		{
			unsigned char uc = static_cast<unsigned char>(c);
			if(c == '\n')
			{
				recent.clear();
			}
			else if(std::isspace(uc))
			{
				add_word(current);
				current.clear();
			}
			else if(is_punct_req_space(c))
			{
				add_word(current);
				current.clear();
				add_word(std::string{}+c);
			}
			else
			{
				current += static_cast<std::string::value_type>(std::tolower(uc));
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
				std::set<std::string> past {basis.begin(), basis.end()};
				auto future = data.find(past);
				if(future == data.end())
				{
					basis.pop_front();
					continue;
				}
				auto wordmap = future->second;
				std::size_t max {};
				for(auto const &chance : wordmap)
				{
					max += chance.second;
				}
				std::uniform_int_distribution<std::size_t> dist {1, max};
				auto r = dist(rand);
				for(auto const &chance : wordmap)
				{
					if(chance.second >= r)
					{
						recent.pop_front();
						recent.push_back(chance.first);
						if(!is_punct_req_space(chance.first[0]))
						{
							s << ' ';
						}
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
