#pragma once

#include "BSEnums.hpp"
#include "Gamebryo/NiTArray.hpp"

class MobileObject;

struct ProcessArray {
	NiTPrimitiveArray<MobileObject*>	kMobileObjects;
	uint32_t							uiBeginOffsets[PROCESS_TYPE_COUNT];
	uint32_t							uiEndOffsets[PROCESS_TYPE_COUNT];
	uint32_t							uiStoredBeginOffsets[PROCESS_TYPE_COUNT];
};