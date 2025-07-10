#include "NiMemObject.hpp"

// GAME - 0xAA13E0
// GECK - 0x851B00
void* NiMemObject::operator new(size_t stSize) {
    return CdeclCall<void*>(0xAA13E0, stSize);
}

// GAME - 0xAA1420
// GECK - 0x851B40
void* NiMemObject::operator new[](size_t stSize) {
	return CdeclCall<void*>(0xAA1420, stSize);
}

// GAME - 0xAA1460
// GECK - 0x851B80
void NiMemObject::operator delete(void* pvMem, size_t stElementSize) {
	CdeclCall(0xAA1460, pvMem, stElementSize);
}

// GAME - 0xAA1490
// GECK - 0x851BB0
void NiMemObject::operator delete[](void* pvMem, size_t stElementSize) {
	CdeclCall(0xAA1490, pvMem, stElementSize);
}
