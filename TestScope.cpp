#include "TestScope.h"

CtorDtorPrinter::CtorDtorPrinter(const char* name) : _name(name)
{
	std::printf("%s: Constructor called\n", _name);
}

CtorDtorPrinter::~CtorDtorPrinter()
{
	std::printf("%s: Destructor called\n", _name);
}

static void PrintScopeObject(int x)
{
	std::printf("object value: %d\n", x);
}

static void TestTempObjectScope()
{
	CtorDtorPrinter("1st");
	CtorDtorPrinter("2nd");

	ObjAccess a;
	int x = a->X() + a->X();
	std::printf("object direct value: %d\n", x);

	PrintScopeObject(a->X());
}

void TestScope()
{
	TestTempObjectScope();
}
