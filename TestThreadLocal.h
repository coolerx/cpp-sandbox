#pragma once

#include "Common.h"

// test config values
namespace TestThreadLocalConfig
{
	constexpr int ThreadCount = 8;
}

extern std::atomic_int g_tlCallCountExtern;
extern std::atomic_int g_tlCallCountInline;

class ThreadLocalData
{
public:
	ThreadLocalData(const char* name, std::atomic_int* counter)
	: _name(name)
	{
		_order = std::atomic_fetch_add(counter, 1) + 1;
		if (bVerbose)
		{
			std::printf("ThreadLocal %s %d created\n", _name, _order);
		}
	}

	int Order() const { return _order; }
	const char* Name() const { return _name; }

private:
	const char* _name;
	int _order;
};

inline static thread_local ThreadLocalData g_tldInline("inline", &g_tlCallCountInline);
extern thread_local ThreadLocalData g_tldExtern;

void ThreadLocalMain(int id);
void TestThreadLocal();
