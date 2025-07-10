#pragma once

#include "BSTMessageQueue.hpp"

template <class T_Data> 
class BSTCommonMessageQueue : public BSTMessageQueue<T_Data> {
public:
	uint32_t uiLock;
};

ASSERT_SIZE(BSTCommonMessageQueue<uint32_t>, 0x8);