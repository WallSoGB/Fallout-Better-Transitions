#pragma once

#include "LockFreeMap.hpp"
#include "Gamebryo/NiSmartPointer.hpp"
#include "BSTaskManagerThread.hpp"
#include "BSTask.hpp"

template <typename T>
class BSTaskManager : public LockFreeMap<T, NiPointer<BSTask<T>>> {
public:
	virtual bool	    AddTask(BSTask<T>* apTask);
	virtual void	    RunTask(BSTask<T>* apTask);
	virtual bool	    FinishTask(BSTask<T>* apTask);
	virtual void	    CancelAllTasks();
	virtual uint32_t	GetThreadForTask(BSTask<T>* apTask);

	bool						bExit;
	uint32_t					uiSleepingThreads;
	uint32_t					uiSleepLock;
	uint32_t					uiThreadCount;
	BSTaskManagerThread<T>**	ppThreads;
	uint32_t*					pBucketCounts;
	DWORD						uiBucketCountTotal;
};

ASSERT_SIZE(BSTaskManager<int64_t>, 0x5C);