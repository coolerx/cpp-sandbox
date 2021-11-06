#include "TestCacheLine.h"
#include "TestThreadLocal.h"
#include "TestIdGen.h"

int main(int, char**) {
    TestCacheLine();
    TestThreadLocal();
    TestIdGen();
}
