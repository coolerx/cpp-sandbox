#pragma once

#include <atomic>
#include <cstdio>
#include <thread>
#include <chrono>

using namespace std::chrono_literals;

using int64 = signed long long;
using uint64 = unsigned long long;

#define is_aligned(POINTER, BYTE_COUNT) \
    (((uintptr_t)(const void *)(POINTER)) % (BYTE_COUNT) == 0)
