#pragma once

#include "Gamebryo/NiPoint3.hpp"

class TESObjectREFR;

class DoorTeleportData {
public:
	TESObjectREFR*	pLinkedDoor;
	NiPoint3		kPos;
	NiPoint3		kRot;
	Bitfield8		ucFlags;
};

ASSERT_SIZE(DoorTeleportData, 0x20);