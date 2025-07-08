#include "NiControllerManager.hpp"

// GAME - 0x47A520
NiControllerSequence* NiControllerManager::GetSequenceByName(const NiFixedString& arName) const {
	NiControllerSequence* pOut = nullptr;
	m_kIndexMap.GetAt(arName.m_kHandle, pOut);
	return pOut;
}

// GAME - 0x47AAB0
// GECK - 0x40F7F0
bool NiControllerManager::ActivateSequence(NiControllerSequence* apSequence, char acPriority, bool abStartOver, float afWeight, float afEaseInTime, NiControllerSequence* apTimeSyncSeq) {
	return ThisCall<bool>(0x47AAB0, this, apSequence, acPriority, abStartOver, afWeight, afEaseInTime, apTimeSyncSeq);
}

// GAME - 0x47B220
// GECK - 0x40F820
bool NiControllerManager::DeactivateSequence(NiControllerSequence* apSequence, float afEaseOutTime) {
	return ThisCall<bool>(0x47B220, this, apSequence, afEaseOutTime);
}