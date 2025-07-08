#include "TESObjectCELL.hpp"

// GAME - 0x550340
bool TESObjectCELL::LoadAllTempData() {
    return ThisCall<bool>(0x550340, this);
}