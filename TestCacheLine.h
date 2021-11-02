#pragma once

#include "Common.h"

void TestCacheLine();

constexpr auto RunTime = 5s;
constexpr auto LoopTime = 50us;
constexpr int ReaderCount = 16;
constexpr bool bVerbose = false;

extern std::atomic_int g_tlCallCount;

class ThreadLocal
{
public:
	ThreadLocal()
	{
		int prev = std::atomic_fetch_add(&g_tlCallCount, 1);
		std::printf("ThreadLocal() %d %lld\n", prev + 1, _val);
	}

	int64 Val() { return _val; }

private:
	int64 _val;
};

// inline static thread_local ThreadLocal g_tl;
extern thread_local ThreadLocal g_tl;
