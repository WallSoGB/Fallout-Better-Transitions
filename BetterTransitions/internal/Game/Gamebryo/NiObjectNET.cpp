#include "NiObjectNET.hpp"
#include "NiTimeController.hpp"

// GAME - 0xA5C570
// GECK - 0x818D30
NiTimeController* NiObjectNET::GetController(const NiRTTI* apRTTI) const {
    NiTimeController* pControllers = m_spControllers;
    if (!pControllers)
		return nullptr;

	while (pControllers) {
		if (pControllers->IsExactKindOf(apRTTI))
			return pControllers;

		pControllers = pControllers->m_spNext;
	}

	return nullptr;
}

NiTimeController* NiObjectNET::GetController(const NiRTTI& arRTTI) const {
	return GetController(&arRTTI);
}