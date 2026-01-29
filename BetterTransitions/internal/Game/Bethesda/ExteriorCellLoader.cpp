#include "ExteriorCellLoader.hpp"

ExteriorCellLoader* ExteriorCellLoader::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<ExteriorCellLoader**>(0x11C9618);
#else
	return *reinterpret_cast<ExteriorCellLoader**>(0xEDCDEC);
#endif
}

// GAME - 0x5285A0
uint32_t ExteriorCellLoader::QueuedCellCount() const {
	return GetCount();
}

// GAME - 0x5283C0
void ExteriorCellLoader::QueueCellLoad(TESWorldSpace* apWorld, int32_t aiCellX, int32_t aiCellY) {
	ThisCall(0x5283C0, this, apWorld, aiCellX, aiCellY);
}

// GAME - 0x528110
void ExteriorCellLoader::PostProcessCompletedTasks() {
	ThisCall(0x528110, this);
}

// GAME - 0x5281F0
uint32_t ExteriorCellLoader::TryCancelTasks() {
	return ThisCall(0x5281F0, this);
}

// GAME - 0x5282D0
void ExteriorCellLoader::WaitForTasks() {
	ThisCall(0x5282D0, this);
}