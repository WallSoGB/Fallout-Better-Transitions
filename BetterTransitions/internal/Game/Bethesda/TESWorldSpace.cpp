#include "TESWorldSpace.hpp"

// GAME - 0x587550
TESObjectCELL* TESWorldSpace::GetCellAtPos(const NiPoint3& arPos) const {
	return ThisCall<TESObjectCELL*>(0x587550, this, &arPos);
}