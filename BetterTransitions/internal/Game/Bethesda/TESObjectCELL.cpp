#include "TESObjectCELL.hpp"

// GAME - 0x550340
bool TESObjectCELL::LoadAllTempData() {
    return ThisCall<bool>(0x550340, this);
}

// GAME - 0x5571A0
void TESObjectCELL::QueueReferences(bool abHighPriorityOnly) {
	ThisCall(0x5571A0, this, abHighPriorityOnly);
}
