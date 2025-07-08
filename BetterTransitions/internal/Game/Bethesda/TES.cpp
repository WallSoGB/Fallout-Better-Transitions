#include "TES.hpp"

TES* TES::GetSingleton() {
#ifdef GAME
	return *reinterpret_cast<TES**>(0x11DEA10);
#else
	return *reinterpret_cast<TES**>(0xECF93C);
#endif
}

// GAME - 0x4FD3E0
TESWorldSpace* TES::GetWorldSpace() const {
	return pWorldSpace;
}

// GAME - 0x5F36F0
TESObjectCELL* TES::GetInterior() const {
	return pInteriorCell;
}
// GAME - 0x4511E0
// GECK - 0x4C7C30
bool TES::IsCellLoaded(const TESObjectCELL* apCell, bool abIgnoreBuffered) const {
    return ThisCall<bool>(0x4511E0, this, apCell, abIgnoreBuffered);
}

// GAME - 0x459A00
// GECK - N/A
void TES::UpdateFadeNodesForAttachedCells() const {
    ThisCall(0x459A00, this);
}