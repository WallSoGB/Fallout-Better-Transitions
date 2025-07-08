#pragma once

#include "ThreadSpecificInterfaceManager.hpp"
#include "InterfacedClass.hpp"
#include "BSSpinLock.hpp"

template<class T>
class LockFreeQueue : public InterfacedClass {
public:
	LockFreeQueue(uint32_t auiMaxThreads, uint32_t auiDeleteBatchSize);
	~LockFreeQueue() override;

	virtual void		IncrementCount();
	virtual void		DecrementCount();
	virtual uint32_t	GetCount() const;

	void* AllocateInterface(uint32_t auiThread) const override;

	struct LockFreeQueueNode {
		LockFreeQueueNode*	pNext = nullptr;
		T					Data;
	};

	class LockFreeQueueInterface {
	public:
		LockFreeQueueInterface(LockFreeQueue* apOwner, LockFreeQueueNode** apNode0, LockFreeQueueNode** apNode1) : pOwner(apOwner), pReferencedNodes(apNode0, apNode1) {}

		LockFreeQueue*		pOwner			= nullptr;
		LockFreeQueueNode** pReferencedNodes[2] = {};
		uint32_t			uiDeleteCount	= 0;
		LockFreeQueueNode*	pDeleteHead		= nullptr;

		LockFreeQueue* GetOwner() const {
			return pOwner;
		}
	};

	typedef ThreadSpecificInterfaceManager<LockFreeQueueInterface> Manager;

	LockFreeQueueNode*		pHead;
	LockFreeQueueNode*		pTail;
	uint32_t				uiDeleteBatchSize;
	LockFreeQueueNode**		pReferencedNodes;
	Manager*				pManager;
	uint32_t				uiCount;
	DWORD					dword1C;
	BSSpinLock				kLock;
};

ASSERT_SIZE(LockFreeQueue<char const*>::LockFreeQueueInterface, 0x14);
ASSERT_SIZE(LockFreeQueue<char const*>, 0x40);