#pragma once

#include "MobileObjectTaskletData.hpp"

class TESObjectREFR;

// Unused
class PackageUpdateTaskData : public MobileObjectTaskletData {
public:
	uint32_t unk38;
	uint32_t unk3C;
	uint32_t unk40;
};

ASSERT_SIZE(PackageUpdateTaskData, 0x44);