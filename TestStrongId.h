#pragma once

#include "Common.h"

template <typename TValue, typename TTag>
struct FStrongId
{
	TValue V{};

	explicit FStrongId(const TValue InV) : V(InV) {}
	FStrongId() {}
	FStrongId(const FStrongId& Other) : V(Other.V) {}

	FStrongId& operator=(const FStrongId& Rhs) { V = Rhs.V; return *this; }
	FStrongId& operator=(const TValue& Rhs) { V = Rhs; return *this; }
	explicit operator const TValue& () const { return V; }
	explicit operator TValue& () { return V; }

	bool operator==(const FStrongId& Rhs) const
	{
		return V == Rhs.V; 
	}

	bool operator<(const FStrongId& Rhs) const { return V < Rhs.V; }
};

void TestStrongId();
