#pragma once

#include "Gamebryo/NiSmartPointer.hpp"
#include "LockFreeMap.hpp"

class TESWorldSpace;
class ExteriorCellLoaderTask;

class ExteriorCellLoader : public LockFreeMap<uint32_t, NiPointer<ExteriorCellLoaderTask>> {
public:
	static ExteriorCellLoader* GetSingleton();

	static double dTotalTime;

	uint32_t QueuedCellCount() const;

	void QueueCellLoad(TESWorldSpace* apWorld, int32_t aiCellX, int32_t aiCellY);

	void PostProcessCompletedTasks();

	void WaitForTasks();
};

ASSERT_SIZE(ExteriorCellLoader, 0x40);