#pragma once

#include "BSMemObject.hpp"
#include "InterfacedClass.hpp"

template <typename T>
class ThreadSpecificInterfaceManager {
public:
	struct ThreadSpecificInterface {
		uint32_t	uiThreadID;
		T*			pData;
	};

	uint32_t					uiMaxThreads;
	uint32_t					uiTLSIndex;
	ThreadSpecificInterface*	pInterfaces;
	uint32_t					uiThreadCount;
};

ASSERT_SIZE(ThreadSpecificInterfaceManager<void>, 0x10)
ASSERT_SIZE(ThreadSpecificInterfaceManager<void>::ThreadSpecificInterface, 0x8)