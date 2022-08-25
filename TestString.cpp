#include "TestString.h"

static void TestStringCompare()
{
	std::string a = ("9");
	std::string b = ("10");

	if (a < b)
	{
		std::cout << a << " < " << b << '\n';
	}
	else
	{
		std::cout << a << " >= " << b << '\n';
	}
}

void TestString()
{
	TestStringCompare();
}
