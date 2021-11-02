#include "main.h"

volatile bool g_bGo = false;

int main(int, char**) {
    auto start = std::chrono::high_resolution_clock::now();

    std::thread readers[ReaderCount];
    for (int i = 0; i < ReaderCount; ++i)
    {
        std::thread reader(ReaderMain, i + 1);
        readers[i].swap(reader);
    }

    std::thread setter(WriterMain);

    g_bGo = true;

    std::this_thread::sleep_for(RunTime);

    g_bGo = false;

    setter.join();
    for (int i = 0; i < ReaderCount; ++i)
    {
        readers[i].join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::printf("executed: %lf ms %d readers\n", elapsed.count(), ReaderCount);
    std::printf("hardware threads: %u\n", std::thread::hardware_concurrency());
    std::printf("thread local value: %lld", g_tl.Val());
    ReportThreadTest();
}
