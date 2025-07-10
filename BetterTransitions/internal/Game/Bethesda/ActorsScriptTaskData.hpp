#pragma once

#include "MobileObjectTaskletData.hpp"

class Actor;

// Unused
class ActorsScriptTaskData : public MobileObjectTaskletData {
public:
	uint32_t unk38;
	uint32_t uiRunCount;
};

ASSERT_SIZE(ActorsScriptTaskData, 0x40);