#include "TestThreadLocal.h"

void ThreadLocalMain(int id)
{
	std::printf("thread %d runs thread local %d %d\n",
	  id, g_tldInline.Order(), g_tldExtern.Order());
}
