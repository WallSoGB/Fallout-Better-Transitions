#pragma once

#include "BSSemaphore.hpp"
#include "bhkThreadMemoryRouter.hpp"

class BSTaskThread {
public:
	BSTaskThread(const char* apThreadName);
	virtual ~BSTaskThread();
	virtual void ThreadUpdate() = 0;

	HANDLE					hThread;
	DWORD					uiThreadID;
	BSSemaphore				kTaskSemaphore;
	BSSemaphore				kPauseSemaphore;
	bhkThreadMemoryRouter	kHavokData;
};

ASSERT_SIZE(BSTaskThread, 0x30);