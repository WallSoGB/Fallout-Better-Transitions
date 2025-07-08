#include "NiObject.hpp"

// GAME - 0x6532C0
bool NiObject::IsKindOf(const NiRTTI& apRTTI) const {
	return GetRTTI()->IsKindOf(apRTTI);
}

// GAME - 0x6532C0
bool NiObject::IsKindOf(const NiRTTI* const apRTTI) const {
	return GetRTTI()->IsKindOf(apRTTI);
}

// GAME - 0x45BAF0
bool NiObject::IsExactKindOf(const NiRTTI* const apRTTI) const {
	return GetRTTI()->IsExactKindOf(apRTTI);
}

// GAME - 0x45BAF0
bool NiObject::IsExactKindOf(const NiRTTI& apRTTI) const {
	return GetRTTI()->IsExactKindOf(apRTTI);
}

// GAME - 0x45BAD0
bool NiObject::IsExactKindOf(const NiRTTI& apRTTI, const NiObject* apObject) {
	return apObject && apObject->IsExactKindOf(apRTTI);
}

// GAME - 0x45BAD0
bool NiObject::IsExactKindOf(const NiRTTI* const apRTTI, const NiObject* apObject) {
	return apObject && apObject->IsExactKindOf(apRTTI);
}