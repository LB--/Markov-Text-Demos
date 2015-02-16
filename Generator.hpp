#ifndef Generator_HeaderPlusPlus
#define Generator_HeaderPlusPlus

#include <iostream>

struct Generator
{
	virtual ~Generator() = default;

private:
	virtual std::istream &input(std::istream &) = 0;
	virtual std::ostream &output(std::ostream &) = 0;

	friend std::istream &operator>>(std::istream &s, Generator &g)
	{
		return g.input(s);
	}
	friend std::ostream &operator<<(std::ostream &s, Generator &g)
	{
		return g.output(s);
	}
};

#endif
