#include <set>
#include <map>
#include "TestIdGen.h"

namespace Cfg = TestIdGenConfig;

static FlakeIdGen g_gen(Cfg::Node);
static std::set<int64_t> g_threadIssuedIds[Cfg::ThreadCount];

static void PrintFlakeId(const FlakeId& id)
{
	std::printf("%lld : node %llu timestamp %llu sequence %llu\n", id.value,
#if PLATFORM_COMPILER_MSVC
	  id.node, id.timestamp, id.sequence);
#else
	  static_cast<uint64_t>(id.node), id.timestamp, static_cast<uint64_t>(id.sequence));
#endif
}

FlakeIdGen::FlakeIdGen(int64_t node) : _node(node)
{
	FlakeId initial(_node, EpochInMs(), 0ull);
	_lastId.store(initial.value);
}

FlakeId FlakeIdGen::Next()
{
	FlakeId old(_lastId.load());

	while (true)
	{
		uint64_t curTimestamp = EpochInMs();

		// try issue with new timestamp
		if (curTimestamp > old.timestamp)
		{
			FlakeId next(_node, curTimestamp, 0ull);
			if (_lastId.compare_exchange_weak(old.value, next.value))
			{
				return next;
			}
			continue;
		}

		// try issue with same timestamp
		if (old.sequence < FlakeId::MaxSequence)
		{
			
			FlakeId next(_node, old.timestamp, old.sequence + 1);
			if (_lastId.compare_exchange_weak(old.value, next.value))
			{
				return next;
			}
			continue;
		}

		// sleep minimum and try again
		std::this_thread::yield();
		old.value = _lastId.load();
	}
}

uint64_t FlakeIdGen::EpochInMs()
{
	auto duration = std::chrono::system_clock::now().time_since_epoch();
	auto durationInMs = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
	return static_cast<uint64_t>(durationInMs.count());
}

static void IdGenMain(int threadId)
{
	auto& myIssuedIds = g_threadIssuedIds[threadId - 1];
	
	auto start = std::chrono::high_resolution_clock::now();
	auto end = start + Cfg::RunTime;

	int32_t duplicateCount = 0;
	int32_t tryCount = 0;

	do
	{
		++tryCount;
		FlakeId id = g_gen.Next();

		const auto [it, bSuccess] = myIssuedIds.insert(id.value);
		if (!bSuccess)
		{
			std::printf("<thread %d> duplicate id %d try\n", threadId, tryCount);
			PrintFlakeId(id);
			++duplicateCount;
		}
	} while (std::chrono::high_resolution_clock::now() < end);

	std::printf("<thread %d> issued %zu ids %d duplicates\n", threadId, myIssuedIds.size(), duplicateCount);
}

void TestIdGen()
{
	auto start = std::chrono::high_resolution_clock::now();

	std::printf(">>> id generator test with %d threads\n", Cfg::ThreadCount);

	std::thread threads[Cfg::ThreadCount];
	for (int i = 0; i < Cfg::ThreadCount; ++i)
	{
		std::thread t(IdGenMain, i + 1);
		threads[i].swap(t);
	}

	for (int i = 0; i < Cfg::ThreadCount; ++i)
	{
		threads[i].join();
	}

	std::map<int64_t, int> issuedIdToThreads;
	std::map<int64_t, int64_t> timestampToCounts;

	for (int i = 0; i < Cfg::ThreadCount; ++i)
	{
		for (auto& id : g_threadIssuedIds[i])
		{
			if (const auto [it, bSuccess] = issuedIdToThreads.emplace(id, i + 1); !bSuccess)
			{
				std::printf("<thread %d> duplicate id with <thread %d>\n", i + 1, it->second);
			}
			FlakeId flakeId(id);
			int64_t timestamp = flakeId.timestamp;
			const auto [it, bSuccess] = timestampToCounts.emplace(timestamp, 0);
			(it->second)++;
		}
	}

	int64_t minCount = 0;
	int64_t maxCount = 0;
	int64_t totalCount = 0;
	for (auto& elem : timestampToCounts)
	{
		maxCount = std::max(elem.second, maxCount);
		if (minCount == 0)
		{
			minCount = elem.second;
		}
		else
		{
			minCount = std::min(elem.second, minCount);
		}
		totalCount += elem.second;
	}
	int64_t totalTimestamps = static_cast<int64_t>(timestampToCounts.size());
	int64_t avgCount = totalCount / totalTimestamps;

	auto end = std::chrono::high_resolution_clock::now();
	std::chrono::duration<double, std::milli> elapsed = end - start;

	std::printf("executed: %lf ms\n", elapsed.count());
	std::printf("timestamps %lld ids %lld: avg %lld min %lld max %lld\n",
	  totalTimestamps, totalCount, avgCount, minCount, maxCount);
}
