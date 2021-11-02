#include "main.h"

std::atomic_int g_tlCallCount(0);
thread_local ThreadLocal g_tl;

struct UnCache
{
    char dummy[68];
    int64* val;

    UnCache()
    {
        val = nullptr;
        uint64 address = reinterpret_cast<uint64>(&dummy[0]);

        for (uint64 i = 0; i < sizeof(dummy); i += 4)
        {
            uint64 startCacheline = (address + i) / 64;
            uint64 endCacheline = (address + i + 7) / 64;
            bool bSameCachelien = startCacheline == endCacheline;
            if (!bSameCachelien)
            {
                val = reinterpret_cast<int64*>(address + i);
                std::printf("succeeded to set test address %llu %p dummy: %hhd\n", i, val, dummy[0]);
            }
        }
        if (val == nullptr)
        {
            std::printf("failed to set test address\n");
            val = reinterpret_cast<int64*>(&dummy[0]);
        }
        *val = 0;
    }

    int64 Val() { return *val; }
    void SetVal(int64 inVal) { *val = inVal; }
};

UnCache g_uc;

template<class TRep, class TPeriod>
void SleepLittle(const std::chrono::duration<TRep, TPeriod>& duration)
{
    auto start = std::chrono::high_resolution_clock::now();
    auto end = start + duration;
    do
    {
        std::this_thread::yield();
    } while (std::chrono::high_resolution_clock::now() < end);
}

void WriterMain()
{
    while (!g_bGo);

    auto start = std::chrono::high_resolution_clock::now();
    int64 val = 0;

    while (g_bGo)
    {
        val++;
        g_uc.SetVal(val << 32 | val);
        SleepLittle(LoopTime);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> elapsed = end - start;

    std::printf("writer last global value: %lld %lld\n", g_uc.Val(), val);
    std::printf("writer executed: %lf ms\n", elapsed.count());
    std::printf("writer thread local value: %lld", g_tl.Val());
}

void ReaderMain(int id)
{
    while (!g_bGo);

    int64 prevVal = -1;
    int64 errCount = 0;
    int64 loopCount = 0;
    int64 lastErrVal = -1;

    while (g_bGo)
    {
        int64 curVal = g_uc.Val();
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

        SleepLittle(LoopTime);
    }

    std::printf("reader<%d> last global value: %lld \n", id, prevVal);
    std::printf("reader<%d> error count: %lld \n", id, errCount);
    if (errCount > 0)
    {
        std::printf("reader<%d> last error value: %lld %lld\n", id, (lastErrVal >> 32), (lastErrVal & 0x00000000FFFFFFFF));
    }
    std::printf("reader<%d> thread local value: %lld", id, g_tl.Val());
}

void ReportThreadTest()
{
    std::printf("sizeof UnCache: %zu\n", sizeof(UnCache));
    uint64 address = reinterpret_cast<uint64>(g_uc.val);
    uint64 startCacheline = address / 64;
    uint64 endCacheline = (address + 7) / 64;
    bool bSameCachelien = startCacheline == endCacheline;
    std::printf("offsetof val: %zu same caheline %c\n", offsetof(UnCache, val), bSameCachelien ? 'y' : 'n');
}
