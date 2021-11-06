#include "TestCacheLine.h"

namespace Cfg = TestCacheLineConfig;

struct SharedData
{
    char dummy[68];
    int64_t* val;

    SharedData()
    {
        val = nullptr;
        uint64_t address = reinterpret_cast<uint64_t>(&dummy[0]);

        for (uint64_t i = 0; i < sizeof(dummy); i += 4)
        {
            uint64_t startCacheline = (address + i) / 64;
            uint64_t endCacheline = (address + i + 7) / 64;
            bool bSameCachelien = startCacheline == endCacheline;
            if (!bSameCachelien)
            {
                val = reinterpret_cast<int64_t*>(address + i);
                if (bVerbose)
                {
                    std::printf("succeeded to set test address %llu %p dummy: %hhd\n",
                      i, val, dummy[0]);
                }
            }
        }
        if (val == nullptr)
        {
            std::printf("failed to set test address\n");
            val = reinterpret_cast<int64_t*>(&dummy[0]);
        }
        *val = 0;
    }

    int64_t Val() { return *val; }
    void SetVal(int64_t inVal) { *val = inVal; }
};

static volatile bool g_bGo = false;
static SharedData g_sd;

void WriterMain()
{
    while (!g_bGo);

    auto start = std::chrono::high_resolution_clock::now();
    int64_t val = 0;

    while (g_bGo)
    {
        val++;
        g_sd.SetVal(val << 32 | val);
        SleepLittle(Cfg::LoopTime);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::printf("writer last global value: %lld %lld\n", g_sd.Val(), val);
    std::printf("writer executed: %lf ms\n", elapsed.count());
}

void ReaderMain(int id)
{
    while (!g_bGo);

    int64_t prevVal = -1;
    int64_t errCount = 0;
    int64_t loopCount = 0;
    int64_t lastErrVal = -1;

    while (g_bGo)
    {
        int64_t curVal = g_sd.Val();
        if ((curVal >> 32) != (curVal & 0x00000000FFFFFFFF))
        {
            lastErrVal = curVal;
            errCount++;
        } else
        {
            curVal = curVal >> 32;
            if (curVal < prevVal)
            {
                lastErrVal = curVal;
                errCount++;
            } else if (curVal > prevVal)
            {
                prevVal = curVal;
            }
        }

        if (bVerbose && loopCount % 500 == 0)
        {
            std::printf("<%03d> current value: %lld\n", id, curVal);
        }
        loopCount++;

        SleepLittle(Cfg::LoopTime);
    }

    if (bVerbose)
    {
        std::printf("reader<%d> last global value: %lld \n", id, prevVal);
    }
    if (errCount > 0)
    {
        std::printf("reader<%d> error count: %lld last error value: %lld %lld\n", 
          id, errCount, (lastErrVal >> 32), (lastErrVal & 0x00000000FFFFFFFF));
    }
}

void TestCacheLine()
{
    auto start = std::chrono::high_resolution_clock::now();

	std::printf(">>> cache line test with %d readers\n", Cfg::ReaderCount);

    std::thread readers[Cfg::ReaderCount];
    for (int i = 0; i < Cfg::ReaderCount; ++i)
    {
        std::thread reader(ReaderMain, i + 1);
        readers[i].swap(reader);
    }

    std::thread writer(WriterMain);

    g_bGo = true;

    std::this_thread::sleep_for(Cfg::RunTime);

    g_bGo = false;

    writer.join();
    for (int i = 0; i < Cfg::ReaderCount; ++i)
    {
        readers[i].join();
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::printf("executed: %lf ms\n", elapsed.count());
    std::printf("hardware threads: %u\n", std::thread::hardware_concurrency());
    if (bVerbose)
    {
        std::printf("sizeof SharedData: %zu\n", sizeof(SharedData));
        uint64_t address = reinterpret_cast<uint64_t>(g_sd.val);
        uint64_t startCacheline = address / 64;
        uint64_t endCacheline = (address + 7) / 64;
        bool bSameCachelien = startCacheline == endCacheline;
        std::printf("offsetof val: %zu same caheline %c\n",
        offsetof(SharedData, val), bSameCachelien ? 'y' : 'n');
    }
}
