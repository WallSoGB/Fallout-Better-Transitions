#pragma once

#include "InterfacedClass.hpp"

class BSThread {
public:
	BSThread();
	virtual			~BSThread();
	virtual DWORD	ThreadProc();

	CRITICAL_SECTION	kCriticalSection;
	HANDLE				hCreatedThread;
	HANDLE				hCreatorThread;
	unsigned long		uiThreadID;
	unsigned long		uiCreatorThreadID;
	bool				bIsInitialized;

	void Initialize(SIZE_T dwStackSize, const char* apName, bool abSuspended);
	void SetThreadPriority(int aiPriority);
	void Exit();
	void SetPreferredProcessor(uint32_t auiProcessor) const;
};

ASSERT_SIZE(BSThread, 0x30);