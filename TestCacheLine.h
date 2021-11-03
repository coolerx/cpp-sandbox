#pragma once

#include "Common.h"

// test config values
constexpr auto RunTime = 5s;
constexpr auto LoopTime = 50us;
constexpr int ReaderCount = 16;
constexpr bool bVerbose = false;

void TestCacheLine();

extern std::atomic_int g_tlCallCount;

class ThreadLocal
{
public:
	ThreadLocal() : _order(0)
	{
		_order = std::atomic_fetch_add(&g_tlCallCount, 1) + 1;
		std::printf("ThreadLocal %lld created\n", _order);
	}

	int64 Order() { return _order; }

private:
	int64 _order;
};

// inline static thread_local ThreadLocal g_tl;
extern thread_local ThreadLocal g_tl;
