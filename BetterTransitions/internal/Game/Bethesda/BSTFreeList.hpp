#pragma once

template <typename T_Data>
class BSTFreeListElem {
public:
	BSTFreeListElem();
	~BSTFreeListElem();

	uint8_t						kRawElem[sizeof(T_Data)];
	BSTFreeListElem<T_Data>*	pNext;
};

template <typename T_Data>
class BSTFreeList {
public:
	BSTFreeList();
	virtual ~BSTFreeList();

	uint32_t					uiLock;
	BSTFreeListElem<T_Data>*	pFree;
};