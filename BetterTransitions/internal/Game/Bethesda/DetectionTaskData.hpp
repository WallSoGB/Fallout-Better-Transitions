#pragma once

#include "MobileObjectTaskletData.hpp"

class Actor;

// Unused
class DetectionTaskData : public MobileObjectTaskletData {
public:
	float		flt38;
	uint32_t	uiRunCount;
};

ASSERT_SIZE(DetectionTaskData, 0x40);