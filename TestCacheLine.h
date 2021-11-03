#pragma once

#include "Common.h"

// test config values
constexpr auto RunTime = 5s;
constexpr auto LoopTime = 50us;
constexpr int ReaderCount = 16;

void TestCacheLine();
