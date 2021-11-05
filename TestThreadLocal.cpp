#include "TestThreadLocal.h"

namespace Cfg = TestThreadLocalConfig;

std::atomic_int g_tlCallCountExtern;
std::atomic_int g_tlCallCountInline;
thread_local ThreadLocalData g_tldExtern("extern", &g_tlCallCountExtern);

void TestThreadLocal()
{
    auto start = std::chrono::high_resolution_clock::now();

	std::printf(">>> thread local test with %d threads\n", Cfg::ThreadCount);

    std::thread threads[Cfg::ThreadCount];
    for (int i = 0; i < Cfg::ThreadCount; ++i)
    {
        std::thread t(ThreadLocalMain, i + 1);
        threads[i].swap(t);
    }

    for (int i = 0; i < Cfg::ThreadCount; ++i)
    {
        threads[i].join();
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::printf("executed: %lf ms\n", elapsed.count());
	std::printf("extern created %d thread local storage\n", g_tlCallCountExtern.load());
	std::printf("inline created %d thread local storage\n", g_tlCallCountInline.load());
}
