#include "NiTimeController.hpp"

// GAME - 0x47AA40
void NiTimeController::SetActive(bool abActive) {
	m_usFlags.Set(Flags::ACTIVE_MASK, abActive);
}

// GAME - 0x4F05A0
bool NiTimeController::GetActive() const {
	return m_usFlags.Get(Flags::ACTIVE_MASK);
}