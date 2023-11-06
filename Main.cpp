#include "TestCacheLine.h"
#include "TestThreadLocal.h"
#include "TestIdGen.h"
#include "TestScope.h"
#include "TestString.h"
#include "TestStrongId.h"

int main(int, char**) {
    TestCacheLine();
    TestThreadLocal();
    TestIdGen();
    TestScope();
    TestString();
    TestStrongId();
}
