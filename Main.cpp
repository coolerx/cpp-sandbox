#include "TestCacheLine.h"
#include "TestThreadLocal.h"
#include "TestIdGen.h"
#include "TestScope.h"
#include "TestString.h"

int main(int, char**) {
    TestCacheLine();
    TestThreadLocal();
    TestIdGen();
    TestScope();
    TestString();
}
