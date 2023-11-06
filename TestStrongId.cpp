#include "TestStrongId.h"

void TestStrongId() 
{
	std::printf(">>> strong id test\n");

	using FItemId = FStrongId<int, struct ItemIdTag>;
	using FStageId = FStrongId<int, struct StageIdTag>;

	FItemId ItemA(1);
	FItemId ItemB(2);

	if (ItemA < ItemB)
	{
		std::printf("ItemA(%d) < ItemB(%d)\n", ItemA.V, ItemB.V);
	}
	else
	{
		std::printf("Unexpected! ItemA(%d) >= ItemB(%d)\n", ItemA.V, ItemB.V);
	}

	ItemA = ItemB;
	if (ItemA == ItemB)
	{
		std::printf("ItemA(%d) == ItemB(%d)\n", ItemA.V, ItemB.V);
	}
	else
	{
		std::printf("Unexpected! ItemA(%d) != ItemB(%d)\n", ItemA.V, ItemB.V);
	}

	FItemId ItemC;
	std::printf("ItemC(%d)\n", int(ItemC));

	FStageId StageA(5);

	ItemA.V = StageA.V;  // this works naturally
	ItemA = (int)StageA; // ItemA = StageA can not be compiled
	std::printf("ItemA(%d) StageA(%d)\n", ItemA.V, StageA.V);

	if (int(ItemA) == int(StageA)) // ItemA == StageA can not be compiled
	{
		std::printf("ItemA(%d) == StageA(%d)\n", ItemA.V, StageA.V);
	}
}
