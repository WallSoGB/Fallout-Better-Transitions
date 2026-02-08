#include "TESWorldSpace.hpp"

// GAME - 0x5875A0
TESObjectCELL* TESWorldSpace::GetCellFromCellCoord(int32_t aiX, int32_t aiY) const {
	return ThisCall<TESObjectCELL*>(0x5875A0, this, aiX, aiY);
}

// GAME - 0x587550
TESObjectCELL* TESWorldSpace::GetCellAtPos(const NiPoint3& arPos) const {
	return GetCellFromCellCoord(int32_t(arPos.x) >> 12, int32_t(arPos.y) >> 12);
}