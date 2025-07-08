#include "NiAVObject.hpp"

// GAME - 0xA59C60
// GECK - 0x80DD60
void NiAVObject::Update(const NiUpdateData& arData) {
	ThisCall<void>(0xA59C60, this, &arData);
}