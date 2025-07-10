#pragma once

#include "BSTCommonMessageQueue.hpp"
#include "BSTFreeList.hpp"

template <class T_Data>
class BSTCommonLLMessageQueue : public BSTCommonMessageQueue<T_Data> {
public:
	BSTFreeList<T_Data>*	pMessageList;
	uint32_t				unk0C;
	uint32_t				unk010;
};

ASSERT_SIZE(BSTCommonLLMessageQueue<void>, 0x14);