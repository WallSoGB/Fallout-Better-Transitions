#pragma once

class hkMemoryRouter;

class bhkThreadMemoryRouter {
public:
	hkMemoryRouter* pHavokMemoryRouter	= nullptr;
	void*			pHavokAlloc			= nullptr;
	DWORD			uiOwnerID			= 0;
};

ASSERT_SIZE(bhkThreadMemoryRouter, 0xC);