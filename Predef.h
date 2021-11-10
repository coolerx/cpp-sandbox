#pragma once

// https://sourceforge.net/p/predef/wiki/Home/

#define PLATFORM_COMPILER_CLANG 0
#define PLATFORM_COMPILER_MSVC 0
#define PLATFORM_COMPILER_GCC 0

#if defined(_MSC_VER)
	#undef PLATFORM_COMPILER_MSVC
	#define PLATFORM_COMPILER_MSVC 1
#elif defined(__clang__)
	#undef PLATFORM_COMPILER_CLANG
	#define PLATFORM_COMPILER_CLANG 1
#elif defined(__GNUC__)
	#undef PLATFORM_COMPILER_GCC
	#define PLATFORM_COMPILER_GCC 1
#else
	#error 'Unsupported Compiler!'
#endif

#define PLATFORM_WINDOWS 0
#define PLATFORM_APPLE 0
#define PLATFORM_LINUX 0

#if defined(_WIN32)
	#undef PLATFORM_WINDOWS
	#define PLATFORM_WINDOWS 1
#elif defined(__APPLE__) && defined(__MACH__)
	#undef PLATFORM_APPLE
	#define PLATFORM_APPLE 1
#elif defined(__linux__)
	#undef PLATFORM_LINUX
	#define PLATFORM_LINUX 1
#else
	#error 'Unsupported OS!'
#endif

#define PLATFORM_CPU_X86_FAMILY 0
#define PLATFORM_CPU_ARM_FAMILY 0

#if defined(_M_IX86) || defined(__i386__) || defined(_M_X64) || defined(__amd64__)
	#undef PLATFORM_CPU_X86_FAMILY
	#define PLATFORM_CPU_X86_FAMILY	1
#elif defined(__arm__) || defined(_M_ARM) || defined(__aarch64__) || defined(_M_ARM64)
	#undef PLATFORM_CPU_ARM_FAMILY
	#define PLATFORM_CPU_ARM_FAMILY	1
#else
	#error 'Unsupported CPU!'
#endif

#define PLATFORM_LITTLE_ENDIAN 1
#define PLATFORM_BIG_ENDIAN 0
