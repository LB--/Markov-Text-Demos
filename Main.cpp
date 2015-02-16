#include "CharacterGenerator.hpp"
#include "WordGenerator.hpp"

#include <fstream>
#include <iostream>
#include <limits>
#include <memory>
#include <sstream>
#include <string>

int main(int, char const *const *)
{
	std::string line;
	do
	{
		std::cout
			<< "Available Markov-chain generators:" << std::endl
			<< "1 - simple character-based" << std::endl
			<< "2 - simple word-based" << std::endl;
		int num;
		while((std::cout << "Pick one: ") && std::getline(std::cin, line) && (!(std::istringstream{line} >> num) || (num != 1 && num != 2)))
		{
		}
		int order;
		while((std::cout << "Order of generator: ") && std::getline(std::cin, line) && (!(std::istringstream{line} >> order) || order <= 0))
		{
		}
		auto gp = std::unique_ptr<Generator>{nullptr};
		if(num == 1)
		{
			gp.reset(new CharacterGenerator{static_cast<std::size_t>(order)});
		}
		else if(num == 2)
		{
			gp.reset(new WordGenerator{static_cast<std::size_t>(order)});
		}
		Generator &g = *gp;
		std::cout << "Enter some text, a filename, or leave blank for default:" << std::endl;
		std::getline(std::cin, line);
		if(line.empty())
		{
			std::istringstream
			{
				"the quick brown fox jumped over the lazy dog"
				" and also ate some cheese while rollerskating along the driveway"
				" at several miles per hour but not at the speed of light"
			} >> g;
		}
		else if(std::ifstream in {line})
		{
			in >> g;
		}
		else
		{
			std::istringstream{line} >> g;
		}
		do for(std::size_t i = 0; i < 80; ++i)
		{
			std::cout << g;
		} while(std::getline(std::cin, line) && line.empty());
		std::cout << "Press enter to try something else" << std::endl;
	} while(std::getline(std::cin, line) && line.empty());
	std::cout << "End of program";
	std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}
