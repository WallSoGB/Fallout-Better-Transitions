#pragma once

#include "BSWin32TaskletData.hpp"
#include "BSTCommonLLMessageQueue.hpp"

// Unused
class MobileObjectTaskletData : public BSWin32TaskletData {
public:
	virtual void EnterCriticalSections(uint32_t);
	virtual void ExecuteTask(void*);
	virtual void LeaveCriticalSections(uint32_t);

	DWORD								dword18;
	BSTCommonLLMessageQueue<uint32_t>	MessageQueue;
	DWORD								dword30;
	DWORD								dword34;
};

ASSERT_SIZE(MobileObjectTaskletData, 0x38);