#pragma once

#include "BSTaskletGroupData.hpp"
#include "BSSemaphore.hpp"

class BSWin32TaskletGroupData : public BSTaskletGroupData {
public:
	bool				bComplete;
	BSSemaphore			kCompletionSemaphore;
	CRITICAL_SECTION	kGroupDataLock;
	bool				bAttached;
	bool				bAllowTaskAttach;
	bool				bCancelled;
	uint32_t			uiPriority;
	uint32_t			uiAttached;
	uint32_t			uiCompleted;
};

ASSERT_SIZE(BSWin32TaskletGroupData, 0x3C);