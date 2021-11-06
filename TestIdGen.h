#pragma once

#include "Common.h"

namespace TestIdGenConfig
{
	constexpr int64_t Node = 0;
	constexpr int32_t ThreadCount = 8;
	constexpr auto RunTime = 100ms;
}

// lock-less id generator like twitter snowflake

struct FlakeId
{
	static constexpr int32_t NodeBits = 10;
	static constexpr int32_t TimestampBits = 43;
	static constexpr int32_t SequenceBits = 10;
	static constexpr int32_t SignBits = 1;
	static constexpr int64_t MaxSequence = (1ll << SequenceBits) - 1;

	union
	{
		int64_t value;
		struct
		{
			uint64_t sequence : SequenceBits;
			uint64_t timestamp : TimestampBits;
			uint64_t node : NodeBits;
			uint64_t sign : SignBits;
		};
	};

	FlakeId(uint64_t inNode, uint64_t inTimestamp, uint64_t inSequence)
	: sequence(inSequence)
	, timestamp(inTimestamp)
	, node(inNode)
	{
		sign = 0;
	}

	FlakeId(int64_t inValue) : value(inValue) {}
};

class FlakeIdGen
{
public:
	FlakeIdGen(int64_t node);
	FlakeId Next();

private:
	uint64_t EpochInMs();
	
	int64_t _node;
	std::atomic<int64_t> _lastId;
};

void TestIdGen();
