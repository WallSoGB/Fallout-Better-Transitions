#pragma once

#include "BSTaskletData.hpp"
#include "BSWin32TaskletGroupData.hpp"

class BSWin32TaskletData : public BSTaskletData {
public:
	BSWin32TaskletData();
	virtual ~BSWin32TaskletData();

	BSWin32TaskletGroupData*	pGroupData;
	bool						bRunOnStartup;
	uint32_t					uiTaskDataLock;
	BSWin32TaskletData*			pLink;
};

ASSERT_SIZE(BSWin32TaskletData, 0x18);