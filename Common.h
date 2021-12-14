#pragma once

#include "Predef.h"

#include <atomic>
#include <cstdio>
#include <thread>
#include <chrono>
#include <set>
#include <map>
#include <ctime>
#include <iostream>
#include <iomanip>

#include <time.h>
#if PLATFORM_WINDOWS
#define timegm _mkgmtime
#endif

// using
using namespace std::chrono_literals;

// macro
#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)

// global constants
constexpr bool bVerbose = false;

// inline helpers
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
