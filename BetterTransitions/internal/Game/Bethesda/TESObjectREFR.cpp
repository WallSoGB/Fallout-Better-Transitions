#include "TESObjectREFR.hpp"

// GAME - 0x43FCD0 - non virtual version of Get3D
// GECK - 0x645930
NiAVObject* TESObjectREFR::DirectGet3D() const {
#ifdef GAME
	return ThisCall<NiAVObject*>(0x43FCD0, this);
#else
	return ThisCall<NiAVObject*>(0x645930, this);
#endif
}

// GAME - 0x8D6F30
TESObjectCELL* TESObjectREFR::GetParentCell() const {
	return pParentCell;
}

// GAME - 0x575D70
// GECK - 0x6418D0
TESWorldSpace* TESObjectREFR::GetWorldSpace() const {
	return ThisCall<TESWorldSpace*>(0x575D70, this);
}

// GAME - 0x569B80
// GECK - 0x63E6D0
TESObjectREFR* TESObjectREFR::GetLinkedRef() const {
#ifdef GAME
	return ThisCall<TESObjectREFR*>(0x569B80, this);
#else
	return ThisCall<TESObjectREFR*>(0x63E6D0, this);
#endif
}
 
// GAME - 0x7AF430
TESBoundObject* TESObjectREFR::GetObjectReference() const {
	return kData.pObjectReference;
}